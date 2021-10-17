/*	Author: Alex Kuang
 *  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Lab #  Exercise #
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *  
 *  Video Link: https://www.youtube.com/watch?v=EIPjolnkGiQ&ab_channel=alexkuang
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

unsigned char button = 0x00;
unsigned char tempB = 0x00;
enum States {Start, Idle, Press, Hold, Release} state;
void Tick(){
    button = ~PINA & 0x01;
    switch (state)
    {
    case Start:
        state = Idle;
        break;
    case Idle:
        if (button)
            state = Press;
        else 
            state = Idle;
        break;
    case Press:
        state = Hold;
        break;
    case Hold:
        if (button)
            state = Hold;
        else 
            state = Release;        
        break;
    case Release:
        state = Idle;
        break;
    default:
        state = Start;
        break;
    }
    switch (state)
    {
    case Start:
        tempB = 0x2A;
        break;
    case Idle:
        break;
    case Press:
        if (tempB == 0x2A)
            tempB = 0x15;
        else 
            tempB = 0x2A;
        break;
    case Hold:
        break;
    case Release:
        break;
    default:
        state = Start;
        break;
    }
    PORTB = tempB;
}

int main(void) {
    DDRA = 0x00; PORTA = 0xFF;          //Initialize Port A to be inputs
    DDRB = 0xFF; PORTB = 0x00;          //initualize Port B to be outputs 
    state = Start;
    while (1) {
        Tick();
    }
    return 1;
}
