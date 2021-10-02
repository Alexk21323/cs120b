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
	DDRB = 0x00; PORTB = 0xFF; // Configure port B's 8 pins as inputs
	DDRC = 0x00; PORTC = 0xFF; // Configure port C's 8 pins as inputs
	DDRD = 0xFF; PORTD = 0x00; // Configure port D's 8 pins as outputs
	
	unsigned char tempA;
	unsigned char tempB;
	unsigned char tempC;


	while(1) {
		tempA = PINA;
		tempB = PINB;
		tempC = PINC;
	        unsigned char tempD = 0x00; 
		//in the case of overweight, set output first bit to 1
		unsigned char totalWeight = tempA + tempB + tempC;
		if (totalWeight > 0x8C)
			tempD = 0x01;
		//in the case of overweight in difference, set output set second bit to 1
		if (tempA > tempC)
			if (tempA - tempC > 0x50)		
				tempD = tempD | 0x02;
		else if (tempC > tempA)
			if (tempC - tempA > 0x50)
				tempD = tempD | 0x02;
		//combine the two output
		totalWeight = totalWeight & 0xFC;
		tempD = tempD | totalWeight;		
		PORTD = tempD;
	}
	return 0;
}


