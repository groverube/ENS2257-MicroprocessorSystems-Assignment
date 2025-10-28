/*
ENS2257 - Assignment One
Members: Jordan, Nicole, Ruben

File: menu.h
Purpose: Declare functions and variables for the keypad C file.
Hardware: ATMega328
*/


#ifndef MENU_H
#define MENU_H

#include <stdint.h>

void welcome_screen_method(void);
int display_menu_selection_method(void);
uint8_t function4_menu_method(void);

#endif
