/*	Author: Alex Kuang
 *  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Lab #  Exercise #
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *
 *  Demo Link: https://www.youtube.com/watch?v=o4kCu2VG4FU&ab_channel=alexkuang
 *
 */
#include <avr/io.h>
#include "io.h"
#include <avr/interrupt.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

//Timer 
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

enum ThreeLEDstates {ThreeLEDsStart, First, Second, Third} ThreeLEDstate;
enum BlinkingLEDstates {BlinkLEDStart, On, Off} BlinkingLEDstate;
enum CombinedLEDstates {CombinedLEDsStart} CombinedLEDstate;

unsigned char tempA = 0x00;
unsigned char tempB = 0x00;

void ThreeLEDsSM()
{
    switch (ThreeLEDstate)
    {
    case ThreeLEDsStart:
        ThreeLEDstate = First;
        break;

    case First:
        ThreeLEDstate = Second;
        break;

    case Second:
        ThreeLEDstate = Third;
        break;

    case Third:
        ThreeLEDstate = First;
        break;

    default:
        ThreeLEDstate = ThreeLEDsStart;
        break;
    }

    switch (ThreeLEDstate)
    {
    case ThreeLEDsStart:
        break;
    
    case First:
        tempA = 0x01;
        break;

    case Second:
        tempA = 0x02;
        break;
    
    case Third:
        tempA = 0x04;
        break;
    
    default:
        break;
    }
}

void BlinkingLEDSM (){
    switch (BlinkingLEDstate)
    {
    case BlinkLEDStart:
        BlinkingLEDstate = On;
        break;
    
    case On:
        BlinkingLEDstate = Off;
        break;

    case Off:
        BlinkingLEDstate = On;
        break;

    default:
        BlinkingLEDstate = BlinkLEDStart;
        break;
    }

    switch (BlinkingLEDstate)
    {
    case BlinkLEDStart:
        break;
    
    case On:
        tempB = 0x08;    
        break;

    case Off:
        tempB = 0x00;
        break;

    default:
        break;
    }
}

void CombineLEDsSM(){
    switch (CombinedLEDstate)
    {
    case CombinedLEDsStart:
        CombinedLEDstate = CombinedLEDsStart;
        break;
    
    default:
        CombinedLEDstate = CombinedLEDsStart;
        break;
    }
    
    switch (CombinedLEDstate)
    {
    case CombinedLEDsStart:
        PORTB = tempA | tempB;
        break;
    }
}

unsigned char TimeElasped_ThreeLED = 0x00;
unsigned char TimeElasped_BlinkingLED = 0x00;
int main(void) {
    DDRA = 0x00; PORTA = 0xFF;
    DDRB = 0xFF; PORTB = 0x00;
    TimerSet(100);
    TimerOn();
    while (1) {
        if(TimeElasped_ThreeLED > 3){
            ThreeLEDsSM();
            TimeElasped_ThreeLED = 0;
        }
        if(TimeElasped_BlinkingLED > 10){
            BlinkingLEDSM();
            TimeElasped_BlinkingLED = 0;
        }
        CombineLEDsSM();
        while (!TimerFlag) {}
        TimerFlag = 0;
        TimeElasped_ThreeLED++;
        TimeElasped_BlinkingLED++;
    }
    return 1;
}
