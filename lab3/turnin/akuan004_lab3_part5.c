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
    DDRD = 0x00;
    PORTD = 0xFF; // Configure port D's 8 pins as inputs
    DDRB = 0x01;
    PORTB = 0x00; // Configure port C's 8 pins as outputs

    unsigned char tempD = 0x00;
    unsigned char tempB = 0x00;
    unsigned short weight = 0x0000;
    /* Insert your solution below */
    while (1)
    {
        tempD = PIND;
        tempB = PINB;
        weight = (tempD <<1) | tempB;
        if (weight >= 0x46)
            tempB = 0x02;
        else if (weight > 0x05 && weight <0x46)
            tempB = 0x04;
        else if (weight <= 0x05 )
            tempB = 0x00;
        
        PORTB = tempB;
    }
    return 1;
}
