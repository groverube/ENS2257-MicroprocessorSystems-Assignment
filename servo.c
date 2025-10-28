/* 
ENS2257 - Assignment One
Members: Jordan, Nicole, Ruben

File: servo.c
Purpose: Initialise and update servo motor position.
Hardware: ATMega328
*/

// Import required dependency libraries.
#include <avr/io.h>
#include <util/delay.h>
#include <math.h>
#include <stdint.h>
#include <string.h>

// Define global variables by referencing those declared in main.c.
extern uint16_t servoMinTicks;
extern uint16_t servoMaxTicks;
extern uint8_t servoFrequencyHz;
extern float servoMinPulseMs;
extern float servoMaxPulseMs;
uint16_t servoTickRange;
#define PWM_PRESCALER 8

// Method to initialise the servo motor controls using Timer Mode Waveform Generation Mode 14. Utilises the Servo motor Frequency and Prescaler defined above.
// Expects F_CPU, PWM_PRESCALER, servoFrequencyHz, servoMinPulseMs and servoMaxPulseMs.
void servo_init_method(void){
    // Reset registers for PWM setup.
    TCCR1A = 0;
    TCCR1B = 0;

    // Assign OC1A to non-inverting PWM.
    TCCR1A |= (1 << COM1A1);

    // Setup Timer1 for Fast PWM mode using ICR1 as TOP (Mode 14).
    // WGM13 (B) = 1, WGM12 (B) = 1, WGM11 (A) = 1, WGM10 (A) = 0
    TCCR1A |= (1 << WGM11);
    TCCR1B |= (1 << WGM13) | (1 << WGM12);

    // Set clock prescaler to 8. Therefore, 0.5us per tick with 16 MHz clock frequency.
    TCCR1B |= (1 << CS11);

    // Calculations to determine the restraints and set the reset (TOP) value of OC1A before assigning it to the output pin of PB1. Using 16 bit integers to prevent overflow.
    uint16_t servoPeriod = (F_CPU / PWM_PRESCALER) / servoFrequencyHz;
    ICR1 = servoPeriod - 1;
    DDRB |= (1 << PB1);

    // Determine the max and minimum pulse widths in ticks. Divide by 20 to convert from milliseconds to half second ticks then multiply by servo period. Store to global variable for recall in update method. Includes 5% lower and 95% upper limits. 
    servoMinTicks = (uint16_t)(((servoMinPulseMs / 20.0) * servoPeriod) * 1.05f);
    servoMaxTicks = (uint16_t)(((servoMaxPulseMs / 20.0) * servoPeriod) * 0.95f);

    // Determine the total range of movement in ticks. Store to global variable for recall in update method.
    servoTickRange = servoMaxTicks - servoMinTicks;
}



// Method to update the Servo motor on demand via OCR1A using the global variables defined above. Requires 16 bit int type potentiometer reading.
// Expects servoMinTicks, potentiometerReading.
void update_servo_pos_method(uint16_t potentiometerReading){
    // Use the minimum ticks to set the lowest possible position then add the calculated movement ticks. Include 20ms delay to ensure movement is complete before next action.
    uint16_t potToServo = servoMinTicks + ((float)potentiometerReading * servoTickRange) / 1023;
    // Clamping the output at the servos operating limits.
    if (potToServo < servoMinTicks) potToServo = servoMinTicks;
    if (potToServo > servoMaxTicks) potToServo = servoMaxTicks;
    OCR1A = potToServo;
    _delay_ms(20);
}



// Method to determine the ticks required for a given angle change.
// Expects the movement angle in degrees.
// Returns the calculated servo ticks.
uint16_t determine_servo_ticks_method(float inputAngleDegrees) {
    float movementFraction = inputAngleDegrees / 180.0f;
    uint16_t servoOutputTicks = servoMinTicks + (uint16_t)(movementFraction * servoTickRange);
    // Clamping the output at the servos operating limits.
    if (servoOutputTicks < servoMinTicks) servoOutputTicks = servoMinTicks;
    if (servoOutputTicks > servoMaxTicks) servoOutputTicks = servoMaxTicks;
    return servoOutputTicks;
}
