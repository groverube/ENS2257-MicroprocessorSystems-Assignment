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
    - Alternatively can use Ports C0 through C5.
Servo Motor = PB1 
    - Using OC1A for PWM signal.
    - Interrupt Pin PCINT1 is not required for the current purpose.
    - Alternatively PB1 through PB3 can be used.
    - Must use TC1 for PWM as it is the only 16 bit timer.
*/

// Set the CPU clock speed in Microseconds.
#define F_CPU 16000000UL
#define PWM_PRESCALER 8

// Import required dependency libraries. Some are not used in this file but will be required later.
#include <atmel_start.h>
#include <stdint-gcc.h>
#include <stdio.h>
#include <avr/pgmspace.h>
#include <avr/iom328pb.h>
#include <util/delay.h>
#include <math.h>

// Declare external assembly subroutines for the potentiometer.
extern uint8_t pot_Init_ASM(void);
extern uint8_t pot_read_ASM(void);

// Define global variables.
// Using 16 bit integers to prevent overflow for values over 255.
uint16_t servoMinTicks;
uint16_t servoMaxTicks;
uint16_t servoTickRange;
// Volatile variable as the ASM code may update it at any time.
volatile uint16_t potReadingInt; // Minimum of 10 bits required.

// Servo motor restraints.
uint8_t servoFrequencyHz = 50;
float servoMinPulseMs = 0.6;
float servoMaxPulseMs = 2.2;



// Method to initialise the servo motor controls using Timer Mode Waveform Generation Mode 14. Utilises the Servo motor Frequency and Prescaler defined above.
int servo_init_method(void){
    // Reset registers for PWM setup.
    TCCR1A = 0;
    TCCR1B = 0;

    // Assign OC1A to non-inverting PWM.
    TCCR1A |= (1 << COM1A1);

    // Setup Timer1 for Fast PWM mode using ICR1 as TOP (Mode 14).
    // WGM13 (B) = 1, WGM12 (B) = 1, WGM11 (A) = 1, WGM10 (A) = 0
    TCCR1A |= (1 << WGM11);
    TCCR1B |= (1 << WGM13) | (1 << WGM12);

    // Set clock prescaler to 8. 0.5us per tick with 16 MHz clock frequency.
    TCCR1B |= (1 << CS11);

    // Calculations to determine the restraints and set the reset (TOP) value of OC1A before assigning it to the output pin of PB1. Using 16 bit integers to prevent overflow.
    uint16_t servoPeriod = (F_CPU / PWM_PRESCALER) / servoFrequencyHz;
    ICR1 = servoPeriod;
    DDRB |= (1 << PB1);

    // Determine the max and minimum pulse widths in ticks. Divide by 1000 to convert from milliseconds then multiply by servo period. Store to global variable for recall in update method.
    servoMinTicks = (uint16_t)((servoMinPulseMs / 1000.0) * servoPeriod);
    servoMaxTicks = (uint16_t)((servoMaxPulseMs / 1000.0) * servoPeriod);

    // Determine the total range of movement in ticks. Store to global variable for recall in update method.
    servoTickRange = servoMaxTicks - servoMinTicks;

    return 0;
}



// Method to update the Servo motor on demand via OCR1A using the global variables defined above. Requires 16 bit int type potentiometer reading.
void update_servo_pos_method(uint16_t potentiometerReading){
    // Use the minimum ticks to set the lowest possible position then add the calculated movement ticks.
    uint16_t potToServo = servoMinTicks + ((float)potentiometerReading * servoTickRange) / 1023;
    OCR1A = potToServo;
}



// Main function declaration. Executed at program runtime. 
int main (void){
// Initialise the Micro Controller Unit (MCU), Potentiometer and Servo motor.
atmel_start_init();
pot_Init_ASM();
servo_init_method();

    // Continuously read the potentiometer value and update the servo motor after each delay period.
    while(1){
        // Read potentiometer value and store the result in the potReadingInt variable.
        potReadingInt = pot_read_ASM();

        // Move the servo motor to the read potentiometer position.
        update_servo_pos_method(potReadingInt);

        _delay_ms(20);
    }

return 0;

}
