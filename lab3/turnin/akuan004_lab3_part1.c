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
    DDRB = 0x00;
    PORTB = 0xFF; // Configure port B's 8 pins as inputs
    DDRC = 0xFF;
    PORTC = 0x00; // Configure port C's 8 pins as outputs

    unsigned char tempA = 0x00;
    unsigned char tempB = 0x00;
    unsigned char sum = 0x00;
    /* Insert your solution below */
    while (1)
    {
        tempA = PINA;
        tempB = PINB;
        sum = 0x00;
        //Check A port starting for right most bit
        if ((tempA & 0x01) == 0x01)
            sum += 1;
        if ((tempA & 0x02) == 0x02)
            sum += 1;
        if ((tempA & 0x04) == 0x04)
            sum += 1;
        if ((tempA & 0x08) == 0x08)
            sum += 1;
        if ((tempA & 0x10) == 0x10)
            sum += 1;
        if ((tempA & 0x20) == 0x20)
            sum += 1;
        if ((tempA & 0x40) == 0x40)
            sum += 1;
        if ((tempA & 0x80) == 0x80)
            sum += 1;
        //Check B port starting from right most bit
        if ((tempB & 0x01) == 0x01)
            sum += 1;
        if ((tempB & 0x02) == 0x02)
            sum += 1;
        if ((tempB & 0x04) == 0x04)
            sum += 1;
        if ((tempB & 0x08) == 0x08)
            sum += 1;
        if ((tempB & 0x10) == 0x10)
            sum += 1;
        if ((tempB & 0x20) == 0x20)
            sum += 1;
        if ((tempB & 0x40) == 0x40)
            sum += 1;
        if ((tempB & 0x80) == 0x80)
            sum += 1;
        //Output to total
        PORTC = sum;
    }

    return 1;
}
