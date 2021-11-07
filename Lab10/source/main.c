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
enum Speakerstates {SpeakerStart, SpeakerOff, SpeakerOn} SpeakerState;


unsigned char tempA = 0x00;
unsigned char tempB = 0x00;
unsigned char tempC = 0x00;

unsigned char button = 0x00;

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

unsigned char tick = 0x00; 
void SpeakerSM() {
    button = ~PINA & 0x04;
    switch (SpeakerState)
    {
    case SpeakerStart:
        SpeakerState = SpeakerOff;
        break;
    
    case SpeakerOff:
        if (button)
            SpeakerState = SpeakerOn;
        else 
            SpeakerState = SpeakerOff;
        break;
    
    case SpeakerOn:
        if (button)
            SpeakerState = SpeakerOn;
        else 
            SpeakerState = SpeakerOff;
        break;

    default:
        SpeakerState = SpeakerStart;
        break;
    }

    switch (SpeakerState)
    {
    case SpeakerStart:
        break;
    
    case SpeakerOff:
        tempC = 0x00;
        break;

    case SpeakerOn:
        if (tick <= 2) 
            tempC = 0x10;
		else if (tick > 2){
            tempC = 0x00;			
            tick = 0;
        }
        tick++;
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
        PORTB = tempA | tempB | tempC;
        break;
   }
}


unsigned long TimeElasped_ThreeLED = 0x00;
unsigned long TimeElasped_BlinkingLED = 0x00;
int main(void) {
    DDRA = 0x00; PORTA = 0xFF;
    DDRB = 0xFF; PORTB = 0x00;
    TimerSet(1);
    TimerOn();
    while (1) {
        if(TimeElasped_ThreeLED >= 300){
            ThreeLEDsSM();
            TimeElasped_ThreeLED = 0;
        }
        if(TimeElasped_BlinkingLED >= 1000){
            BlinkingLEDSM();
            TimeElasped_BlinkingLED = 0;
        }
        SpeakerSM();
        CombineLEDsSM();
        while (!TimerFlag) {}
        TimerFlag = 0;
        TimeElasped_ThreeLED++;
        TimeElasped_BlinkingLED++;
    }
    return 1;
}
