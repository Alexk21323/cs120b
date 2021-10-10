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

enum States {Start, Init, Idle, OnPressAdd, HoldAdd, OnPressMinus, HoldMinus, OnBothPressed} state;
void Tick(){
    switch(state){
        case Start:
            state = Init;
            break;
        case Init:
            state = Idle;
            break;
        case Idle:
            if ((PINA & 0x01) == 1)
                state = OnPressAdd;
            else if ((PINA & 0x02) == 0x02)
                state = OnPressMinus;
            else if ((PINA & 0x03) == 0x03)
                state = OnBothPressed;
            break;
        case OnPressAdd:
            if ((PINA & 0x03) == 0x03)
                state = OnBothPressed;
            else 
                state = HoldAdd;
            break;
        case HoldAdd:
            if ((PINA & 0x01) == 1)
                state = HoldAdd;
            else if ((PINA & 0x03) == 0x03)
                state = OnBothPressed;
            else
                state = Idle;
            break;
        case OnPressMinus:
            if ((PINA & 0x03) == 0x03)
                state = OnBothPressed;
            else 
                state = HoldMinus;
            break;
        case HoldMinus:
            if ((PINA & 0x02) == 2)
                state = HoldMinus;
            else if ((PINA & 0x03) == 0x03)
                state = OnBothPressed;
            else
                state = Idle;
            break;
        case OnBothPressed:
            if ((PINA & 0x03) == 0x03)
                state = OnBothPressed;
            else 
                state = Idle;
            break;

        default:
            state = Start;
            break; 
        }
    switch(state){
        case Start:
            break;
        case Init:
            PORTC = 0x07;
            break;
        case Idle:
            break;
        case OnPressAdd:
            if(PORTC < 0x09)
                PORTC += 1;
            break;   
        case HoldAdd:
            break;     
        case OnPressMinus:
            if( PORTC > 0x00)
                PORTC -= 1;
            break;        
        case HoldMinus:
            break;
        case OnBothPressed:
            PORTC = 0x00;
            break;
        default:
            PORTC = 0x07;
            break;
        }
    }

int main(void) {
    /* Insert DDR and PORT initializations */
    DDRA = 0x00; PORTA = 0xFF;          //Initialize Port A to be inputs
    DDRC = 0xFF; PORTC = 0x00;          //initualize Port B to be outputs 
    /* Insert your solution below */
    while (1) {
        Tick();
    }
    return 1;
}
