/*	Author: Alex Kuang
 *  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Lab #  Exercise #
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *  
 *  Demo Link: https://www.youtube.com/watch?v=cGHTJ14KRos&ab_channel=alexkuang
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#include "io.h"
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
unsigned char score = 0x05;
unsigned char reset = 0x01;
enum States {Start, Init, Light2, Light2Left, Light3, WinHold, WinRelease, LoseHold1, LoseRelease1, LoseHold2, LoseRelease2} state;
void Tick() {
    if (score == 0x00)
        LCD_DisplayString(1, "0");
    else if (score == 0x01)
        LCD_DisplayString(1, "1");
    else if (score == 0x02)
        LCD_DisplayString(1, "2");
    else if (score == 0x03)
        LCD_DisplayString(1, "3");
    else if (score == 0x04)
        LCD_DisplayString(1, "4");
    else if (score == 0x05)
        LCD_DisplayString(1, "5");
    else if (score == 0x06)
        LCD_DisplayString(1, "6");
    else if (score == 0x07)
        LCD_DisplayString(1, "7");
    else if (score == 0x08)
        LCD_DisplayString(1, "8");
    else if (score == 0x09)
        LCD_DisplayString(1, "Vicotry");
    button = (~PINA & 0x01);
    switch (state)
    {
    case Start:
        state = Init;
        break;
    case Init:
        if (button)
            state = LoseHold1;
        else 
            state = Light2;
        break;
    case LoseHold1: 
        if (button)
            state = LoseHold1;
        else 
            state = LoseRelease1;
        break;
    case LoseRelease1:
        if(button)
            state = Start;
        else
            state = LoseRelease1;
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
        if(button)
            state = Start;
        else 
            state = WinRelease;
        break;
    case Light3:
        if(button)
            state = LoseHold2;
        else 
            state = Light2Left;
        break;
    case LoseHold2:
        if(button)
            state = LoseHold2;
        else 
            state = LoseRelease2;
        break;
    case LoseRelease2:
        if(button)
            state = Start;
        else 
            state = LoseRelease2;
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
    case LoseHold1:
        if(score >0 && reset){
            score -=1;
            reset = 0x00;
        }
        PORTB = 0x01;
        break;
    case LoseRelease1:
        reset = 0x01;
        PORTB = 0x01;
        break;
    case Light2:
        PORTB = 0x02;
        break;
    case WinHold:
        if(score <9 && reset){
            score +=1;
            reset = 0x00;
        }
        PORTB = 0x02;
        break;
    case WinRelease:
        reset = 0x01;
        PORTB = 0x02;
        break;
    case Light3:
        PORTB = 0x04;
        break;
    case LoseHold2:
        if(score >0 && reset){
            score -=1;
            reset = 0x00;
        }
        PORTB = 0x04;
        break;
    case LoseRelease2:
        reset = 0x01;
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
    /* Insert DDR and PORT initializations */
    DDRA = 0x00; PORTA = 0xFF;          //Initialize Port A to be inputs
    DDRB = 0xFF; PORTB = 0x00;          //initualize Port B to be outputs 
    DDRC = 0xFF; PORTC = 0xFF;
    DDRD = 0xFF; PORTD = 0xFF;
    
    state = Start;
    TimerSet(300);
    TimerOn();
    LCD_init();
    /* Insert your solution below */
    while (1) {
        Tick();
        while(!TimerFlag);
	    TimerFlag = 0;
    }
    return 1;
}
