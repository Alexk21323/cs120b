/*	Author: Alex Kuang
 *  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Lab #  Exercise #
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *
 *  Demo Link: https://www.youtube.com/watch?v=IDe5xGzuayc&ab_channel=alexkuang
 * 
 */
#include <avr/io.h>
#include "io.h"
#include "timer.h"
#include "keypad.h"
#include "scheduler.h"
#include <avr/interrupt.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

enum KeypadSstates {ButtonStart, ButtonPlay, ButtonPaused, ButtonUp, ButtonDown, ButtonHold, PlayHold, PausedHold};
unsigned char button1;
unsigned char button2;
unsigned char button3;
unsigned char play;
unsigned char cursor;

int ButtonSMTick (int state){
    button1 = ~PINB & 0x01;
    button2 = ~PINB & 0x02;
    button3 = ~PINB & 0x04; 
    switch (state)
    {
    case ButtonStart:
        state = ButtonPaused;
        break;

    case ButtonPlay:
        if (button1 && !button2 && !button3)
            state = PlayHold;
        else if (button2)
            state = ButtonUp;
        else if (button3)
            state = ButtonDown;
        else
            state = ButtonPlay;
        break;

    case PlayHold:
        if (button1)
            state = PlayHold;
        else 
            state = ButtonPaused;
        break;

    case ButtonPaused:
        if (button1)
            state = PausedHold;
        else 
            state = ButtonPaused;
        break;

    case PausedHold:
        if (button1)
            state = PausedHold;
        else 
            state = ButtonPlay;
        break;

    case ButtonUp:
        state = ButtonHold;
        break;
    
    case ButtonDown:
        state = ButtonHold;
        break;

    case ButtonHold:
        if (button2 || button3)
            state = ButtonHold;
        else 
            state = ButtonPlay;
        break;
     
    default: 
        state = ButtonStart;
        break;
    }
    
    switch (state)
    {
    case ButtonStart:
        play = 0;
        cursor = 2;
        break;
    
    case ButtonPlay:
        play = 0x01;
        break;
    
    case ButtonPaused:
        play = 0x00;
        break;
    
    case ButtonUp:
        cursor = 2;
        break;
    
    case ButtonDown:
        cursor = 18;
        break;

    case ButtonHold:
        break;

    default:
        break;
    }
    return state;
}

enum LCDStates {LCDStart, LCDInit, LCDGameOver};
unsigned char obsticles[] = "         #                   #  ";
unsigned char gameover[] = "    Game Over!                   "; 
unsigned char index; 
int LCDSMTick(int state) {
    button1 = ~PINB & 0x01;
    switch (state)
    {
    case LCDStart:
        state = LCDInit;
        break;
    
    case LCDInit: 
        if (obsticles[cursor + index] == '#')
            state = LCDGameOver;
        else 
            state = LCDInit;
        break;

    case LCDGameOver:
        if (button1)
            state = LCDStart;
        else 
            state = LCDGameOver;

        break;

    default:
        state = LCDStart;
        break;
    }

    switch (state)
    {
    case LCDStart:
        break;

    case LCDInit:
        for (int i = 0; i < 32; i++){
            LCD_Cursor(i);
            LCD_WriteData(obsticles[(i + index)%32]);
        }
        if(play){
            if (index <=32)
                index++;
            else 
                index = 0;
        }
        LCD_Cursor(cursor);
        break;
    
    case LCDGameOver:
        for (int i = 1; i < 33; i++){
            LCD_Cursor(i);
            LCD_WriteData(gameover[i]);
        }
        index = 0;

    default:
        break;
    }
    return state;
}

int main(void) {
    DDRA = 0xFF; PORTA = 0x00;
    DDRB = 0x00; PORTB = 0xFF;
    DDRC = 0xFF; PORTC = 0x00;//f00f
    DDRD = 0xFF; PORTD = 0x00;
    LCD_init();
    static task task1, task2;
    task *tasks[] = { &task1, &task2 };
    const unsigned short numTasks = sizeof(tasks) / sizeof(task*);
    const char start = -1;

    task1.state = start;
    task1.period = 10;
    task1.elapsedTime = task1.period;
    task1.TickFct = &ButtonSMTick;

    task2.state = start;
    task2.period = 25;
    task2.elapsedTime = task2.period;
    task2.TickFct = &LCDSMTick;

    TimerSet(10);
    TimerOn();

    unsigned short i;
    while (1) {
	for (i = 0; i < numTasks; ++i) {
		if (tasks[i]->elapsedTime == tasks[i]->period) {
			tasks[i]->state = tasks[i]->TickFct(tasks[i]->state);
			tasks[i]->elapsedTime = 0;
		}
		tasks[i]->elapsedTime += 1;
	}
	while(!TimerFlag);
	TimerFlag = 0;
    }
    return 1;
}
