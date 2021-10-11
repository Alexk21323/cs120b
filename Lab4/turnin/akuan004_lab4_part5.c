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

enum States {Start, Locked, NextInput, BadInput, Open} state;
unsigned char array[4] = {0x04, 0x01, 0x2, 0x01};
unsigned char i = 0x00;
void Tick(){
    switch (state)
    {
    case Start:
        state = Locked;
        break;
    case Locked:
        if((PINA & 0x07) == array[i])
            state = NextInput;
        else if ((PINA & 0x07) != array[i])
            state = BadInput;
        else if ((PINA & 0x80) == 0x80)
            state = Locked;
        else if (PINA == 0)
            state = Locked;
    case NextInput:
        if (PINA == 0){
            i ++;
            if (i == 3)
                state = Open;
            else 
                state = Locked;
        }
        else 
            state = NextInput;
    case BadInput:
        if ((PINA & 0x07) == 0x00)
            state = BadInput;
        else {
            i = 0x00;
            state = Locked;
        }
    case Open:
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
        break;
    case NextInput:
        break;
    case BadInput:
        break;
    case Open:
        PORTB = 0x01;
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
