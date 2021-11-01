/*	Author: lab
 *  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Lab #  Exercise #
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *
 *  Demo Link: https://www.youtube.com/watch?v=lbetXK7RrWY&ab_channel=alexkuang
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

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

enum States {Start, Idle, IdleHold, On, OnRelease, Increase,IncreaseRelease, Decrease, DecreaseRelease} state;
// const double freq[8] = {261.33, 293.66, 329.63, 349.23, 392.00, 440.00, 493.88, 523.25};
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
    case IdleHold:
        if(button1)
            state = IdleHold;
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
        if(button2)
            state = Increase;
        else if (button3)
            state = Decrease;
        else if (button1)
            state = IdleHold;
        else 
            state = OnRelease;
        break;
    case Increase:
        if (button2)
            state = Increase;
        else{
            if(i<7)
                i++;
            state = OnRelease;
        }
        break;
    case Decrease:
        if (button3)
            state = Decrease;
        else{
            if(i>0)
                i--;
            state = OnRelease;
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
		case On:
            break;
        case OnRelease:
            set_PWM(freq[i]);
            break;
        case Increase:
            break;
        case Decrease:
            break;
        default:        
            break;
	}
}
int main(void) {
    DDRA = 0x00; PORTA = 0xFF;
    DDRB = 0xFF; PORTB = 0x00;
    PWM_on();
    while (1) {
        Tick();
    }
    return 1;
}
