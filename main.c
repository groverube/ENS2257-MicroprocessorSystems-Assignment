/* 
ENS2257 - Assignment One
Members: Jordan, Nicole, Ruben

File: Main.c
Purpose: Read a potentiometer using assembly and update a servo motors location.
Hardware: ATMega328

PIN DEFINITIONS:
Potentiometer = PC0
    - Using ADC0 for Analog to Digital Conversion.
    - No pull up resistor required as not a Digital Input Pin.
    - Alternatively, can use Ports C0 through C5.
Servo Motor = PB1 
    - Using OC1A for PWM signal.
    - Interrupt Pin PCINT1 is not required for the current purpose.
    - Alternatively, PB1 through PB3 can be used.
    - Must use TC1 for PWM as it is the only 16 bit timer.
*/

// Set the CPU clock speed in Microseconds.
#define F_CPU 16000000UL
#define PWM_PRESCALER 8

// Import required dependency libraries. Some are not used in this file but will be required later.
//#include <stdio.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include <stdint.h>
#include "lcd.h"

// Import function files.
#include "servo.h"
#include "keypad.h"
#include "menu.h"


// Declare external assembly subroutines for the potentiometer.
extern void pot_Init_ASM(void);
extern uint16_t pot_read_ASM(void);

// Define global variables.
// Using 16 bit integers to prevent overflow for values over 255.
uint16_t servoMinTicks;
uint16_t servoMaxTicks;
volatile uint16_t potReadingInt; // Volatile variable as the ASM code may update it at any time. Minimum of 10 bits required.

// Define general variables.
char lastKeyPressed;
uint16_t animationStepCount = 0;
uint16_t animationOneArray[10] = {0, 5, 10, 15, 20, 25, 30, 35, 40, 45};
uint16_t animationTwoArray[15] = {180, 0, 180, 0, 180, 0, 180, 0, 180, 0, 180, 0, 180, 0, 180};
uint16_t animationThreeArray[20] = {0, 15, 30, 45, 60, 75, 90, 105, 120, 135, 150, 165, 180, 165, 150, 135, 120, 105, 90, 75};

// Servo motor restraints.
uint8_t servoFrequencyHz = 50;
float servoMinPulseMs = 0.6;
float servoMaxPulseMs = 2.2;

// Servo animation definitions.
#define maxAnimationSteps 20
#define msDelayPerAnimation 200
#define totalRecordingTimeMs 5000
#define recordingIntervalMs 250
#define maximumRecordingSteps (totalRecordingTimeMs / recordingIntervalMs)
uint16_t recordingArray[maximumRecordingSteps];



// Method to update the servo motor through each animation step.
// Expects the array name.
void play_servo_animation_method(uint16_t* animationArray, uint16_t numberAnimationSteps) {
    // Iterate through each step and move the servo as required.
    for (uint16_t i = 0; i < numberAnimationSteps; i++) {
        //Exit the animation early if a key is pressed.
        lastKeyPressed = keypad_read_method();
        if (lastKeyPressed) return;

        // Update servo position.
        update_servo_pos_method(animationArray[i]);
        _delay_ms(msDelayPerAnimation);
    }
}



// Main function declaration. Executed at program runtime. 
int main (void){
    // Initialise the Micro Controller Unit (MCU), Potentiometer, Servo motor, keypad and LCD screen.
    // atmel_start_init();
	lcd_init();
	lcd_home();
    pot_Init_ASM();
    servo_init_method();
    keypad_init_method();


    // Load the welcome screen then display the menu selection.
    welcome_screen_method();

    while (1) {
        uint8_t menuSelection = display_menu_selection_method();

        switch(menuSelection) {
            // Function 1 - Switch the angle between three preset positions.
            case 1:
                lcd_line_one();
                lcd_write_string(PSTR("*********************"));
                lcd_line_two();
                lcd_write_string(PSTR("Press 1, 2, 3 to move"));
                lcd_line_three();
                lcd_write_string(PSTR("        #=Exit       "));
                lcd_line_four();
                lcd_write_string(PSTR("*********************"));
                _delay_ms(3000);            
                do {
                    lastKeyPressed = keypad_read_method();
                    switch(lastKeyPressed) {
                        case '1': 
                            lcd_line_three();
                            lcd_write_string(PSTR("     1 Degree     "));
                            _delay_ms(3000);   
                            update_servo_pos_method(determine_servo_ticks_method(1)); 
                        break;
                        case '2': 
                            lcd_line_three();
                            lcd_write_string(PSTR("     90 Degrees    "));
                            _delay_ms(3000);   
                            update_servo_pos_method(determine_servo_ticks_method(90)); 
                        break;
                        case '3': 
                            lcd_line_three();
                            lcd_write_string(PSTR("    180 Degrees   "));
                            _delay_ms(3000);   
                            update_servo_pos_method(determine_servo_ticks_method(180)); 
                        break;
                        case '#': break; // Used to prevent error message from displaying.
                        default:
                            lcd_line_two();
                            lcd_write_string(PSTR("Incorrect Selection"));
                            lcd_line_three();
                            lcd_write_string(PSTR(" Please try again "));
                            _delay_ms(3000);
                            lcd_line_two();
                            lcd_write_string(PSTR("Moving to position"));
                            _delay_ms(3000);
                        break;
                    }
                } while (lastKeyPressed != '#');
            break;

            // Function 2 - Read potentiometer value and store the result in the potReadingInt variable then move the servo motor to the read position.
            case 2:
                lcd_line_one();
                lcd_write_string(PSTR("*********************"));
                lcd_line_two();
                lcd_write_string(PSTR("     Pot to servo    "));
                lcd_line_three();
                lcd_write_string(PSTR(" return with any key "));
                lcd_line_four();
                lcd_write_string(PSTR("*********************"));
                _delay_ms(3000);  
                do {
                    lastKeyPressed = keypad_read_method();
                    potReadingInt = pot_read_ASM();
                    update_servo_pos_method(potReadingInt);
                } while (lastKeyPressed == '$'); // Requested that any keypress exits. 
            break;

            // Function 3 - Play predefined animations.
            case 3:
                lcd_line_one();
                lcd_write_string(PSTR("*********************"));
                lcd_line_two();
                lcd_write_string(PSTR("   Select Animation  "));
                lcd_line_three();
                lcd_write_string(PSTR("    1 2 3  #=Exit    "));
                lcd_line_four();
                lcd_write_string(PSTR("*********************"));
                _delay_ms(3000);  
                do {
                    lastKeyPressed = keypad_read_method();
                    switch(lastKeyPressed) {
                        case '1': 
                            lcd_line_two();
                            lcd_write_string(PSTR("Playing Animation 1"));
                            animationStepCount = sizeof(animationOneArray) / sizeof(animationOneArray[0]); // Dynamically determine the size of the array.
                            play_servo_animation_method(animationOneArray, animationStepCount); 
                            break;
                        case '2': 
                            lcd_line_two();
                            lcd_write_string(PSTR("Playing Animation 2"));
                            animationStepCount = sizeof(animationTwoArray) / sizeof(animationTwoArray[0]); // Dynamically determine the size of the array.
                            play_servo_animation_method(animationTwoArray, animationStepCount); 
                            break;
                        case '3': 
                            lcd_line_two();
                            lcd_write_string(PSTR("Playing Animation 3"));
                            animationStepCount = sizeof(animationThreeArray) / sizeof(animationThreeArray[0]); // Dynamically determine the size of the array.
                            play_servo_animation_method(animationThreeArray, animationStepCount); 
                            break;
                        case '#': break; // Used to prevent error message from displaying.
                        default:
                            lcd_line_two();
                            lcd_write_string(PSTR("Invalid key"));
                            _delay_ms(1000);
                            lcd_line_two();
                            lcd_write_string(PSTR("   Select Animation  "));
                            lcd_line_three();
                            lcd_write_string(PSTR("    1 2 3  #=Exit    "));
                            _delay_ms(1000);
                        break;
                    }
                } while (lastKeyPressed != '#');
            break;

            // Function 4 - Record and playback custom animations.
            case 4:
                lcd_line_one();
                lcd_write_string(PSTR("*********************"));
                lcd_line_two();
                lcd_write_string(PSTR(" 1=Record 2=PlayBack "));
                lcd_line_three();
                lcd_write_string(PSTR("       #=Exit         "));
                lcd_line_four();
                lcd_write_string(PSTR("*********************"));
                _delay_ms(3000);
                
                do {
                    lastKeyPressed = function4_menu_method();
                    switch(lastKeyPressed) {
                        // Record user input.
                        case '1':
                            lcd_line_two();
                            lcd_write_string(PSTR("Recording"));
                            animationStepCount = 0;

                            for (uint16_t i = 0; i < maximumRecordingSteps; i++) {
                                lastKeyPressed = keypad_read_method();
                                // Exit early if required.
                                if (lastKeyPressed != '$') {
                                    update_servo_pos_method(0);
                                    break;
                                }

                                // Read potentiometer and update servo.
                                potReadingInt = pot_read_ASM();
                                update_servo_pos_method(potReadingInt);
                                
                                // Store read value in array.
                                recordingArray[i] = potReadingInt;
                                animationStepCount++;
                                _delay_ms(recordingIntervalMs);
                            }
                        break;
                        
                        // Playback user input.
                        case '2':
                            lcd_line_two();
                            lcd_write_string(PSTR("Playing back"));
                            for (uint16_t i = 0; i < animationStepCount; i++) {
                                lastKeyPressed = keypad_read_method();
                                // Exit early if required.
                                if (lastKeyPressed != '$') {
                                    update_servo_pos_method(0);
                                    break;
                                }

                                // Update the servo position from the arrays value.
                                update_servo_pos_method(recordingArray[i]);
                                _delay_ms(recordingIntervalMs);
                            }
                        break;

                        case '#': break;

                        default:
                            lcd_line_two();
                            lcd_write_string(PSTR("Invalid key"));
                            _delay_ms(1000);
                        break;
                    }
                } while (lastKeyPressed != '#');
            break;

            default: break;
        }
    }
    return 0;
}
