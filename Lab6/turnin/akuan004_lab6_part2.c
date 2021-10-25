/*	Author: Alex Kuang
 *  Partner(s) Name: 
 *	Lab Section: 21 
 *	Assignment: Lab #  Exercise #
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *
 *  Demo Link: https://www.youtube.com/watch?v=EOnTtpDn0JA&ab_channel=alexkuang
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

volatile unsigned char TimerFlag = 0;
unsigned long _avr_timer_M = 1;
unsigned long _avr_timer_cntcurr = 0;
unsigned char button = 0x00;

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
enum States {Start, Init, Light2, Light2Left, Light3, WinHold, WinRelease} state;
void Tick() {
    button = (~PINA & 0x01);
    switch (state)
    {
    case Start:
        state = Init;
        break;
    case Init:
        state = Light2;
        break;
    case Light2:
        if(button)
            state = WinHold;
        else
            state = Light3;
        break;
    case WinHold:
        if(button == 1)
            state = WinHold;
        else 
            state = WinRelease;
        break;
    case WinRelease:
        if (button)
            state = Start;
        else 
            state = WinRelease;
        break;
    case Light3:
        state = Light2Left;
        break;
    case Light2Left:
        if(button)
            state = WinHold;
        else
            state = Init;
        break;
    default:
        state = Start;
        break;
    }
    switch (state)
    {
    case Start:
        break;
    case Init:
        PORTB = 0x01;
        break;
    case Light2:
        PORTB = 0x02;
        break;
    case WinHold:
        PORTB = 0x02;
        break;
    case WinRelease:
        PORTB = 0x02;
        break;
    case Light3:
        PORTB = 0x04;
        break;
    case Light2Left: 
        PORTB = 0x02;
        break;
    default:
        break;
    }
}
int main(void) {
    DDRA = 0x00; PORTA = 0xFF;          //Initialize Port A to be inputs
    DDRB = 0xFF; PORTB = 0x00;          
    TimerSet(300);
    TimerOn();
    while (1) {
        Tick();
        while(!TimerFlag);
        TimerFlag = 0;
    }
    return 1;
}
