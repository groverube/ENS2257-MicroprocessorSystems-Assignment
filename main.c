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
#include <math.h>
#include <stdint.h>
#include <string.h>
#include <lcd.h>

// Import function files.
#include "servo.c"
#include "keypad.c"
#include "menu.c"


// Declare external assembly subroutines for the potentiometer.
extern void pot_Init_ASM(void);
extern uint16_t pot_read_ASM(void);

// Define global variables.
// Using 16 bit integers to prevent overflow for values over 255.
uint16_t servoMinTicks;
uint16_t servoMaxTicks;
volatile uint16_t potReadingInt; // Volatile variable as the ASM code may update it at any time. Minimum of 10 bits required.

// Servo motor restraints.
uint8_t servoFrequencyHz = 50;
float servoMinPulseMs = 0.6;
float servoMaxPulseMs = 2.2;

// Main function declaration. Executed at program runtime. 
int main (void){
    // Initialise the Micro Controller Unit (MCU), Potentiometer, Servo motor, keypad and LCD screen.
    atmel_start_init();
    pot_Init_ASM();
    servo_init_method();
    keypad_init_method();
    lcd_init();
    lcd_home();

    // Load the welcome screen then display the menu selection.
    welcome_screen_method();

    while (1) {
        uint8_t menuSelection = display_menu_selection_method();

        switch(menuSelection) {
            // Function 1
            case 1:

            break;
            // Function 2 - Read potentiometer value and store the result in the potReadingInt variable then move the servo motor to the read position.
            case 2:
                potReadingInt = pot_read_ASM();
                update_servo_pos_method(potReadingInt);
            break;
            // Function 3
            case 3:
            break;
            // Function 4
            case 4:
            break;
            default:
                display_menu_selection_method();
            break;
        }
    }
    

        // Continuously read the potentiometer value and update the servo motor after each delay period.
        while(1){

        }

    return 0;
}
