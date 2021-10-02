/*	Author: Alex Kuang
 *  Partner(s) Name: 
 *	Lab Section:21
 *	Assignment: Lab # 2 Exercise # 3
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

int main(void) {
	DDRA = 0x00; PORTA = 0xFF; // Configure port A's 8 pins as inputs
	DDRB = 0xFF; PORTC = 0x00; // Configure port C's 8 pins as outputs, initialize to 0s
	
	unsigned char temp_input = 0x00;
	while(1) {
		unsigned char cntavail = 0x04;
		temp_input = PINA;
		
		if ((temp_input & 0x01) == 0x01)
			cntavail = cntavail - 1;
		if ((temp_input & 0x02) == 0x02)
			cntavail = cntavail - 1;
		if ((temp_input & 0x04) == 0x04)
			cntavail = cntavail - 1;
		if ((temp_input & 0x08) == 0x08)
			cntavail = cntavail - 1;
		if ( cntavail == 0)
			cntavail = 0x80;
		PORTC = cntavail;
	}
	return 0;
}


