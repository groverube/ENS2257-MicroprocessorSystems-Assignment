/*
ENS2257 - Assignment One
Members: Jordan, Nicole, Ruben

File: keypad.h
Purpose: Declare functions and variables for the keypad C file.
Hardware: ATMega328
*/


#ifndef KEYPAD_H
#define KEYPAD_H

#include <stdint.h>

void keypad_init_method(void);
char keypad_read_method(void);

#endif
