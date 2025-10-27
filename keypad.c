/* 
ENS2257 - Assignment One
Members: Jordan, Nicole, Ruben

File: keypad.c
Purpose: Read the inputs of a keypad.
Hardware: ATMega328
*/

// Import required dependency libraries.
#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>

// Define global variables.
uint8_t rowval;
char keych;


// Method to initialise the keypad input and output pins then enable internal pull up resistors.
void keypad_init_method() {
    DDRD |= (1<<PD0) | (1<<PD2) | (1<<PD4) ; // COLUMNS - Set Port D pins 0, 2 and 4 as outputs.
    DDRD &= ~(1<<PD1) & ~(1<<PD3) & ~(1<<PD5) & ~(1<<PD6); // ROWS - Set PD1, PD3, PD5 and PD6 to input mode.
    PORTD |= (1<<PD1) | (1<<PD3) | (1<<PD5) | (1<<PD6); // Enable internal pull up resistors for PD1, PD3, PD5 and PD6.
    PORTD &= ~(1<<PD0) & ~(1<<PD2) & ~(1<<PD4); // Set PD0, PD2 and PD4 as low.
}



// Method to detect which key has been pressed on the keypad. 
// Returns char lastchar as the last valid key pressed.
char keypad_read_method() {	
	keych = '$'; // Set default no key pressed value.
	
	PORTD &= ~(1<<PD2 ); // Check column 1 by setting PD2 low.
	PORTD |= (1<<PD0) | (1<<PD4); // Set remaining column pins as high.
	_delay_us(10); // Prevent debounce issues by allowing the signal to settle.
	
	rowval = PIND & 0x6A;
	
    // Check column 1. 
	switch(rowval) {
		case 0x68: keych = '1';
		break;
		case 0x2A: keych = '4';
		break;
		case 0x4A: keych = '7';
		break;
		case 0x62: keych = '*';
		break;
		default: keych = '$';
		break;
	}
	
    // If no valid keypress detected, check column 2.
	if (keych=='$') {
		PORTD &= ~(1<<PD0 ); // Check column 2 by setting PD0 low.
		PORTD |= (1<<PD2) | (1<<PD4); // Set remaining column pins as high.
		_delay_us(10); // Prevent debounce issues by allowing the signal to settle.
		
		rowval = PIND & 0x6A;
		switch(rowval) {
			case 0x68: keych = '2';
			break;
			case 0x2A: keych = '5';
			break;
			case 0x4A: keych = '8';
			break;
			case 0x62: keych = '0';
			break;
			default: keych = '$';
			break;
		}
	}
	
    // If no valid keypress detected, check column 3.
	if (keych=='$') { 
		PORTD &= ~(1<<PD4 ); // Check column 3 by setting PD4 low.
		PORTD |= (1<<PD2) | (1<<PD0); // Set remaining column pins as high
		_delay_us(10); // Prevent debounce issues by allowing the signal to settle.
		
		rowval = PIND & 0x6A;
		switch(rowval) {
			case 0x68: keych = '3';
			break;
			case 0x2A: keych = '6';
			break;
			case 0x4A: keych = '9';
			break;
			case 0x62: keych = '#';
			break;
			default: keych = '$';
			break;
		}

	}

	// Set the lastchar value for return if a valid keypress was detected.
	if (keych != '$')  {
		lastchar = keych - '0';
	}
	
	return lastchar;
}