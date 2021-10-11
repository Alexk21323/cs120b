/*	Author: Alex Kuang
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

enum States {Start, Locked, OnPressPound, OnReleasePound, OnPressY, OnReleaseY, Open} state;
void Tick(){
        switch (state)
        {
        case Start:
            state = Locked;
            break;
        case  Locked:
            if((PINA & 0x04) == 0x04)
                state = OnPressPound;
            else
                state = Locked;
            break;
        case OnPressPound:
            if((PINA & 0x04) == 0x04)
                state = OnPressPound;
            else if ((PINA & 0x02) == 0x02)
                state = Locked;
            else 
                state = OnReleasePound;
            break;
        case OnReleasePound:
            if ((PINA & 0x02) == 0x02)
                state = OnPressY;
            else if ((PINA & 0x04) == 0x04)
                state = OnPressPound;
            else if ((PINA == 0x00))   
                state = OnReleasePound;
            else
                state = Locked;
            break;
        case OnPressY:
            if ((PINA & 0x02) == 0x02)
                state = OnPressY;
            else 
                state  = OnReleaseY; 
            break;
        case OnReleaseY:
            state = Open;
            break;
        case Open:
            if ((PINA & 0x80) == 0x80)
                state = Locked;
            else
                state = Open;
            break;
        default:
            state = Start;
            break;
        }
        switch (state)
        {
        case Start:
            break;
        case Locked: 
            PORTB = 0x00;
            PORTC = 0x00;
            break;
        case OnPressPound:
            PORTC = 0x01;
            break; 
        case OnReleasePound:
            PORTC = 0x02;
            break;
        case OnPressY:
            PORTB = 0x01;
            PORTC = 0x03;
            break;
        case OnReleaseY:
            PORTB = 0x01;
            break;
        case Open:
            PORTB = 0x01;
            PORTC = 0x04;
            break;
        default:
            PORTB = 0x00;
            break;
        }
    }

int main(void) {
    /* Insert DDR and PORT initializations */
    DDRA = 0x00; PORTA = 0xFF;          //Initialize Port A to be inputs
    DDRB = 0x00; PORTB = 0xFF;          //initualize Port B to be outputs 
    DDRC = 0xFF; PORTC = 0x00;          //initualize Port B to be outputs 
    /* Insert your solution below */
    while (1) {
        Tick();
    }
    return 1;
}
