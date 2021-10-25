/*	Author: Alex Kuang
 *  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Lab #  Exercise #
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *
 *  code, is my own original work.
 *
 *  Demo Link: https://www.youtube.com/watch?v=oJmy_67W4BI&ab_channel=alexkuang 
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

volatile unsigned char TimerFlag = 0;
unsigned long _avr_timer_M = 1;
unsigned long _avr_timer_cntcurr = 0;

void TimerOn(){
    TCCR1B = 0x0B;
    OCR1A = 125; 
    TIMSK1 = 0x02;
    TCNT1 = 0;
    
    _avr_timer_cntcurr = _avr_timer_M;
    SREG |= 0x80; 
}
void TimerOff(){
    TCCR1B = 0x00;
}
void TimerISR(){
    TimerFlag = 1;
}
ISR(TIMER1_COMPA_vect){
    _avr_timer_cntcurr--;
    if (_avr_timer_cntcurr == 0){
        TimerISR();
        _avr_timer_cntcurr = _avr_timer_M;
    }
}
void TimerSet(unsigned long M) {
    _avr_timer_M = M;
    _avr_timer_cntcurr= _avr_timer_M;
}

unsigned char b1 = 0x00;
unsigned char b2 = 0x00; 
unsigned char timer  = 0x00;

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
            if(PORTB < 0x09)
                PORTB =  PORTB + 1;
            break;   
        case HoldAdd:
            if(timer < 10)
                timer++;
            else{
                if(PORTB < 0x09)
                    PORTB =  PORTB + 1;
                timer = 0;
            }
            break;     
        case OnPressMinus:
            if(PORTB > 0x00)
                PORTB = PORTB - 1;
            break;        
        case HoldMinus:
            if(timer < 10)
                timer++;
            else{
                if(PORTB > 0x00)
                    PORTB =  PORTB - 1;
                timer = 0;
            }
            break;
        case OnBothPressed:
            PORTB = 0x00;
            break;
        default:
            break;
        }
    }
int main(void) {
    DDRA = 0x00; PORTA = 0xFF;          //Initialize Port A to be inputs
    DDRB = 0xFF; PORTB = 0x00;          //initualize Port B to be outputs 
    state = Start;
    PORTB = 0x07;
    TimerSet(100);
    TimerOn();
    while (1) {
        Tick();
        while(!TimerFlag);
	    TimerFlag = 0;
    }
    return 1;
}
