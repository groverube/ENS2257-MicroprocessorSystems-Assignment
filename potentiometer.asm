;ENS2257 - Assignment One 
;Members: Jordan, Nicole, Ruben 

;File: Potentiometer.ASM
;Purpose: Assembly file for initialising and reading a potentiometers state.
;Hardware: ATMega328 

;PIN DEFINITIONS: 
;Potentiometer = PC0 
;   - ADC0 (0000) in the Multiplexer location for analog input pins.
;   - Range from 0 - 1023 therefore 10 bits+ are required for storage.

; Import required dependency libraries. 
#include "avr/io.h" 

; Global method to initialise the potentiometer.
.global pot_Init_ASM
pot_Init_ASM:
    ; Save the current registers.
    push r16
    push r17

    ; Select AVcc as reference voltage for r16 and write to the ADMUX register.
    ldi r16, (1<<REFS0)
    sts ADMUX, r16

    ; Enable the ADC and load the bitmask for the 128 ADC prescaler to r16 
    ldi r16, (1<<ADEN) | (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0)
    sts ADCSRA, r16

    ; Restore the registers.
    pop r17
    pop r16

    ret ; Return to main.c.

; Global method to read the potentiometer.
.global pot_read_ASM
pot_read_ASM:
    ; Start ADC conversion by setting ADSC in ADCSRA
    lds r16, ADCSRA
    ori r16, (1<<ADSC)
    sts ADCSRA, r16

wait_adc_calculations:
    ; Continuously loop until the conversion is complete by checking ADIF. Skip jump to complete_adc_calculations if ADIF is empty.
    lds r16, ADCSRA
    sbrc r16, ADIF
    rjmp complete_adc_calculations
    rjmp wait_adc_calculations

complete_adc_calculations:
    ; Clear ADIF for next conversion.
    lds r16, ADCSRA
    ori r16, (1<<ADIF)
    sts ADCSRA, r16

    ; Read the 10 bit ADC result that is stored in ADCL and ADCH.
    lds r24, ADCL          ; Load low byte
    lds r25, ADCH          ; Load high byte

    ret ; Return to main file.c.
.end
