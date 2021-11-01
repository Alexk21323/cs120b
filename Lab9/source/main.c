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

enum States {Start, Idle, On, OnRelease, Increase} state;
// const double freq[8] = {261.33, 293.66, 329.63, 349.23, 392.00, 440.00, 493.88, 523.25};
const double freq[9] = {500, 500, 500, 350, 150, 500, 350, 150, 650};


unsigned char button1 = 0x00; 
unsigned char button2 = 0x00;
unsigned char button3 = 0x00;
unsigned char i = 0x00;
void Tick(){
    button1 = (~PINA & 0x01);
    button2 = (~PINA & 0x02);
    button3 = (~PINA & 0x04);
    switch (state)
    {
    case Start:
        state = Idle;
        break;
    case Idle:
        if (button1)
            state = On;
        else
            state = Idle;
        break;
    case On:
        if (button1)
            state = On;
        else 
            state = OnRelease; 
        break;
    case OnRelease: 
        state = Increase;
        break;
    case Increase:
        if(i<8){
            i++;
            state = Increase;
        }
        else 
            state = Idle;
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
		case On:
            break;
        case Increase:
            set_PWM(freq[i]);
            break;
        default:        
            break;
	}
}
int main(void) {
    DDRA = 0x00; PORTA = 0xFF;
    DDRB = 0xFF; PORTB = 0x00;
    PWM_on();
    TimerSet(500);
    TimerOn();
    while (1) {
        Tick();
        while(!TimerFlag);
	    TimerFlag = 0;
    }
    return 1;
}
