/*	Author: lab
 *  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Lab #  Exercise #
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

unsigned short voltage = 0x00;
unsigned short max = 200;

void ADC_init() {
	ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1<< ADATE);
}
int main(void) {
    DDRA = 0x00; PORTA = 0xFF;
    DDRB = 0xFF; PORTB = 0x00;
    DDRA = 0xFF; PORTA = 0x00;
    DDRD = 0xFF; PORTD = 0x00;
    ADC_init();
    while (1) {
        voltage = ADC;
        if (voltage <=  max/8)
            PORTB = 0x01;
        else if (voltage <= max/4)    
            PORTB = 0x03;
        else if (voltage <= max*3/8)
            PORTB = 0x07;
        else if (voltage <= max/2)
            PORTB = 0x0F;
        else if (voltage <= max*5/8)
            PORTB = 0x01F;
        else if (voltage <= max*2/3)
            PORTB = 0x03F;
        else if (voltage <= max*7/8)
            PORTB = 0x07F;
        else if (voltage <= max)
            PORTB = 0xFF;
    }
    return 1;
}

