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

enum States {Start, OnPressB1, OnReleaseB1, OnPressB2, OnReleaseB2} state;
void Tick(){
    switch(state){
        case Start:
            state = OnReleaseB2;
            break;
        case OnPressB1:
            if ((PINA & 0x01) == 1)
                state = OnPressB1;
            else   
                state = OnReleaseB1;
            break;
        case OnReleaseB1:
            if ((PINA & 0x01) == 0x01)
                state = OnPressB2;
            else
                state = OnReleaseB1;
            break;
        case OnPressB2:
            if ((PINA & 0x01) == 0x01)
                state = OnPressB2;
            else   
                state = OnReleaseB2;
            break;
        case OnReleaseB2:
            if ((PINA & 0x01) ==1)
                state = OnPressB1;
            else
                state = OnReleaseB2;
            break;
        default:
            state = Start;
            break; 
        }
    switch(state){
        case Start:
            PORTB = 0x01;
            break;
        case OnPressB1:
            PORTB = 0x02;
            break;
        case OnReleaseB1:
            PORTB = 0x02;
            break;
        case OnPressB2:
            PORTB = 0x01;
            break;
        case OnReleaseB2:
            PORTB = 0x01;
            break;
        default:
            PORTB = 0x01;
            break;
        }
    }

int main(void) {
    /* Insert DDR and PORT initializations */
    DDRA = 0x00; PORTA = 0xFF;          //Initialize Port A to be inputs
    DDRB = 0xFF; PORTB = 0x00;          //initualize Port B to be outputs 
    /* Insert your solution below */
    while (1) {
        Tick();
    }
    return 1;
}
