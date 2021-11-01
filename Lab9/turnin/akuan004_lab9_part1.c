/*	Author: lab
 *  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Lab #  Exercise #
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *
 *  Demo Link: https://www.youtube.com/watch?v=fE_E22ds7yo&ab_channel=alexkuang
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

enum States {Start, Idle, C_Note, D_Note, E_Note} state;
unsigned char button1 = 0x00; 
unsigned char button2 = 0x00;
unsigned char button3 = 0x00;
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
        if (button1 && !button2 && !button3)
            state = C_Note;
        else if (!button1 && button2 && !button3)
            state = D_Note;
        else if (!button1 && !button2 && button3)
            state = E_Note;
        else 
            state = Idle;
        break;
    case C_Note:
        if (button1 && !button2 && !button3)
            state = C_Note;
        else
            state = Idle;
    break;
    case D_Note:
        if (!button1 && button2 && !button3)
            state = D_Note;
        else
            state = Idle;
    break;
    case E_Note:
        if (!button1 && !button2 && button3)
            state = E_Note;
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
		case C_Note:	
            set_PWM(261.63); 
            break;
		case D_Note:	
            set_PWM(293.66); 
            break;
		case E_Note:	
            set_PWM(329.63); 
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
