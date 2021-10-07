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

int main(void)
{
    /* Insert DDR and PORT initializations */
    DDRA = 0x00;
    PORTA = 0xFF; // Configure port A's 8 pins as inputs
    DDRC = 0xFF;
    PORTC = 0x00; // Configure port C's 8 pins as outputs

    unsigned char tempA = 0x00;
    unsigned char tempC = 0x00;
    /* Insert your solution below */
    while (1)
    {
        tempA = PINA;
        tempA = tempA & 0x0F;
        if (tempA == 0x00)
            tempC = 0x40;
        else if (tempA == 0x02 || tempA == 0x01)
            tempC = 0x60;
        else if (tempA == 0x04 || tempA == 0x03)
            tempC = 0x70;
        else if (tempA == 0x05 || tempA == 0x06) 
            tempC = 0x38;
        else if (tempA == 0x07 || tempA == 0x08 || tempA == 0x09)
            tempC = 0x3C;
        else if (tempA == 0x0A || tempA == 0x0B || tempA == 0x0C)
            tempC = 0x3E;
        else if (tempA == 0x0D || tempA == 0x0E || tempA == 0x0F)
            tempC = 0x3F;
        
        tempA = PINA;
        tempA = tempA & 0x70;
        if (tempA == 0x30)
            tempC = tempC | 0x80;
        PORTC = tempC;
    }
    return 1;
}
