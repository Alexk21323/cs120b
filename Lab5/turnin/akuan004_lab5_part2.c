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

unsigned char b1 = 0x00;
unsigned char b2 = 0x00; 

enum States {Start, Idle, OnPressAdd, HoldAdd, OnPressMinus, HoldMinus, OnBothPressed} state;
void Tick(){
    b1 = ~PINA & 0x01;
    b2 = ~PINA & 0x02;
    switch(state){
        case Start:
            state = Idle;
            break;
        case Idle:
            if (b1 && !b2)
                state = OnPressAdd;
            else if (!b1 && b2)
                state = OnPressMinus;
            else if (b1 && b2)
                state = OnBothPressed;
            else 
                state = Idle;
            break;
        case OnPressAdd:
            if (b1 & !b2)
                state = HoldAdd;
            else    
                state = Idle;
            break;
        case HoldAdd:
            if (b1 && !b2)
                state = HoldAdd;
            else if (b1 & b2)
                state = OnBothPressed;
            else
                state = Idle;
            break;
        case OnPressMinus:
            if (!b1 && b2)
                state = HoldMinus;
            else    
                state = Idle;
            break;
        case HoldMinus:
            if (!b1 && b2)
                state = HoldMinus;
            else if (b1 && b2)
                state = OnBothPressed;
            else
                state = Idle;
            break;
        case OnBothPressed:
            state = Idle;
            break;
        default:
            state = Start;
            break; 
        }

        switch(state){
    
        case Start:
            break;
        case Idle:
            break;
        case OnPressAdd:
            if(PORTC < 0x09)
                PORTC =  PORTC + 1;
            break;   
        case HoldAdd:
            break;     
        case OnPressMinus:
            if(PORTC > 0x00)
                PORTC = PORTC - 1;
            break;        
        case HoldMinus:
            break;
        case OnBothPressed:
            PORTC = 0x00;
            break;
        default:
            break;
        }
    }

int main(void) {
    /* Insert DDR and PORT initializations */
    DDRA = 0x00; PORTA = 0xFF;          //Initialize Port A to be inputs
    DDRC = 0xFF; PORTC = 0x00;          //initualize Port B to be outputs 
    /* Insert your solution below */
    state = Start;
    PORTC = 0x07;
    while (1) {
        Tick();
    }
    return 1;
}
