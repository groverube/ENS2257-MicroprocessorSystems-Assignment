/*
ENS2257 - Assignment One
Members: Jordan, Nicole, Ruben

File: servo.h
Purpose: Declare functions and variables for the servo C file.
Hardware: ATMega328
*/


#ifndef SERVO_H
#define SERVO_H

#include <stdint.h>

void servo_init_method(void);
void update_servo_pos_method(uint16_t potentiometerReading);
uint16_t determine_servo_ticks_method(float inputAngleDegrees);

#endif
