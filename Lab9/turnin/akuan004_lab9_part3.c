/*	Author: lab
 *  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Lab #  Exercise #
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *
 *  Demo Link: https://www.youtube.com/watch?v=KXFueSle9AY&ab_channel=alexkuang
 * 
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#include "io.h"
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

//PWN
void set_PWM(double frequency) {
	static double current_frequency;
	if (frequency != current_frequency) {
		if (!frequency) { TCCR3B &= 0x08; }
		else { TCCR3B |= 0x03; }
		if (frequency < 0.954) { OCR3A = 0xFFFF; }
		else if (frequency > 31250) { OCR3A = 0x0000; }
		else { OCR3A = (short) (8000000 / (128 * frequency)) -1; }
		TCNT3 = 0;
		current_frequency = frequency;
	}
}

void PWM_on() {
	TCCR3A = (1 << COM3A0);
	TCCR3B = (1 << WGM32) | (1 << CS31) | (1 << CS30);
	set_PWM(0);
}

void PWM_off() {
	TCCR3A = 0x00;
	TCCR3B = 0x00;
}

enum States {Start, Idle, Play} state;
const double freq[60] = {440, 440, 440, 0, 
                        440,  440, 440, 0, 
                        440,  440, 440, 0,
                        349, 349, 0, 
                        523, 0, 
                        440, 440, 440,0, 
                        349, 349, 0 , 
                        523, 0, 
                        440, 440, 440, 0,
                        659, 659, 659, 0,
                        659, 659, 659, 0,
                        659, 659, 659, 0,
                        698, 698, 0,
                        523, 0,
                        415, 415, 415, 0,
                        349, 349, 0, 
                        523, 0,
                        440, 440, 440, 0
                        };
unsigned char button1 = 0x00; 
unsigned char i = 0x00;
void Tick(){
    button1 = (~PINA & 0x01);
    switch (state)
    {
    case Start:
        state = Idle;
        break;
    case Idle:
        if (button1)
            state = Play;
        else
            state = Idle;
        break;
    case Play:
        if(i < 59){
            i++;
            state = Play;
        }
        else {   
            i = 0x00;
            state = Idle;
        }
        break;
    default:
        state = Start;
        break;
    }
    switch (state) {
		case Start:     
            break;
		case Idle:	
            set_PWM(0); 
            break;
        case Play:
            set_PWM(freq[i]);
            break;
        default:        
            break;
	}
}
int main(void) {
    DDRA = 0x00; PORTA = 0xFF;
    DDRB = 0xFF; PORTB = 0x00;
    TimerSet(150);
    TimerOn();
    PWM_on();
    while (1) {
        Tick();
        while(!TimerFlag){};
	    TimerFlag = 0;
    }
    return 1;
}
