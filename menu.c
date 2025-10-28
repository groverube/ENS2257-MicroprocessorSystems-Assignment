/* 
ENS2257 - Assignment One
Members: Jordan, Nicole, Ruben

File: menu.c
Purpose: Provide a user interface on the LCD screen.
Hardware: ATMega328
*/

// Import required dependency libraries.
#include <avr/pgmspace.h>
#include <util/delay.h>
#include "lcd.h"

// Method to display the welcome screen on the LCD.
void welcome_screen_method(void){
    lcd_line_one();
    lcd_write_string(PSTR("********************"));
    lcd_line_two();
    lcd_write_string(PSTR("*  Welcome to the  *"));
    lcd_line_three();
    lcd_write_string(PSTR("*   Servo Control  *"));
    lcd_line_four();
    lcd_write_string(PSTR("********************"));
    _delay_ms(3000);
}


// Method to display the LCD menu for user prompting.
// Returns the users menu selection.
int display_menu_selection_method() {
    char lastKeyPressed;
    uint8_t userSelection = 0;

    while (userSelection == 0) {
        // Displays the selection menu on the LCD screen.
        lcd_line_one();
        lcd_write_string(PSTR(" Please select from "));
        lcd_line_two();
        lcd_write_string(PSTR("   options 1 to 4.  "));
        lcd_line_three();
        lcd_write_string(PSTR("                    "));
        lcd_line_four();
        lcd_write_string(PSTR("  Press # for info. "));
        _delay_ms(3000);

        // While loop to pause until a key is pressed. Once false read the key and save it to lastKeyPressed.
        do {
            lastKeyPressed = keypad_read_method();
        } while (lastKeyPressed == '$');

        switch (lastKeyPressed){
            // Display information about the functions when # is pressed.
            case '#':
                lcd_line_one();
                lcd_write_string(PSTR(" 1: Adjust, 2: Read "));
                lcd_line_two();
                lcd_write_string(PSTR("3: Select, 4: Record"));
                lcd_line_three();
                lcd_write_string(PSTR("                    "));
                lcd_line_four();
                lcd_write_string(PSTR(" Press # to return. "));
                _delay_ms(3000);

                // Wait for # to pressed before returning to the menu.
                do {
                    lastKeyPressed = keypad_read_method();
                } while (lastKeyPressed != '#');
            break;
            // Display confirmation of function 1 selection and set the selection value to 1.
            case '1':
                lcd_line_one();
                lcd_write_string(PSTR("*********************"));
                lcd_line_two();
                lcd_write_string(PSTR("  Function 1: Adjust "));
                lcd_line_three();
                lcd_write_string(PSTR("  has been selected! "));
                lcd_line_four();
                lcd_write_string(PSTR("*********************"));
                _delay_ms(3000);
                userSelection = 1;
            break;
            // Display confirmation of function 2 selection and set the selection value to 2.
            case '2':
                lcd_line_one();
                lcd_write_string(PSTR("*********************"));
                lcd_line_two();
                lcd_write_string(PSTR("  Function 2: Read   "));
                lcd_line_three();
                lcd_write_string(PSTR("  has been selected! "));
                lcd_line_four();
                lcd_write_string(PSTR("*********************"));
                _delay_ms(3000);
                userSelection = 2;
            break;
            // Display confirmation of function 3 selection and set the selection value to 3.
            case '3':
                lcd_line_one();
                lcd_write_string(PSTR("*********************"));
                lcd_line_two();
                lcd_write_string(PSTR(" Function 3: Select "));
                lcd_line_three();
                lcd_write_string(PSTR("  has been selected! "));
                lcd_line_four();
                lcd_write_string(PSTR("*********************"));
                _delay_ms(3000);
                userSelection = 3;
            break;
            // Display confirmation of function 4 selection and set the selection value to 4.
            case '4':
                lcd_line_one();
                lcd_write_string(PSTR("*********************"));
                lcd_line_two();
                lcd_write_string(PSTR(" Function 4: Record  "));
                lcd_line_three();
                lcd_write_string(PSTR("  has been selected! "));
                lcd_line_four();
                lcd_write_string(PSTR("*********************"));
                _delay_ms(3000);
                userSelection = 4;
            break;
            // Handling for invalid selections.
            default:
                lcd_line_one();
                lcd_write_string(PSTR("*********************"));
                lcd_line_two();
                lcd_write_string(PSTR("  Invalid Selection  "));
                lcd_line_three();
                lcd_write_string(PSTR("  Please try again   "));
                lcd_line_four();
                lcd_write_string(PSTR("*********************"));
                _delay_ms(3000);
            break;
        }
    }
    return userSelection;
}

uint8_t function4_menu_method(void) {
    lcd_line_one();
    lcd_write_string(PSTR("*********************"));
    lcd_line_two();
    lcd_write_string(PSTR(" 1=Record 2=PlayBack "));
    lcd_line_three();
    lcd_write_string(PSTR("       #=Exit         "));
    lcd_line_four();
    lcd_write_string(PSTR("*********************"));
    return keypad_read_method();
}
