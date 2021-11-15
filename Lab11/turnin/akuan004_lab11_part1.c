/*	Author: Alex Kuang
 *  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Lab #  Exercise #
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *
 *  Demo Link : https://www.youtube.com/watch?v=1qN3TWizzx4&ab_channel=alexkuang 
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

enum KeypadSstates {Start, Init};
unsigned char x;
unsigned char keypad_input = 0x00;
int KeypadSMTick(int state){
    x = GetKeypadKey();
    switch (state)
    {
    case Start:
        state = Init;
        break;
    
    case Init:
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
        switch (x)
        {
        case '\0':
            keypad_input = 0x1F;
            break;
        case '1':
            keypad_input = 0x01;
            break;
        case '2':
            keypad_input = 0x02;
            break;
        case '3':
            keypad_input = 0x03;
            break;
        case '4':
            keypad_input = 0x04;
            break;
        case '5':
            keypad_input = 0x05;
            break;
        case '6':
            keypad_input = 0x06;
            break;
        case '7':
            keypad_input = 0x07;
            break;
        case '8':
            keypad_input = 0x08;
            break;
        case '9':
            keypad_input = 0x09;
            break;
        case '0':
            keypad_input = 0x00;
            break;

        case '#':
            keypad_input = 0x0F;
            break;

        default:
            break;
        }
    PORTB  = keypad_input;
    break;
    }
    return state;
}
int main(void) {
    DDRB = 0xFF; PORTB = 0x00;
    DDRC = 0xF0; PORTC = 0x0F;

    static task task1;
    task *tasks[] = {&task1};
    const unsigned short numTasks = sizeof(tasks)/sizeof(task*);

    const char start = -1;
    task1.state = start;
    task1.period = 100;
    task1.elapsedTime = task1.period;
    task1.TickFct = &KeypadSMTick;
    TimerSet(100);
    TimerOn();
    unsigned short i;
    while (1) {
        for (i = 0; i < numTasks; i++){
            if (tasks[i]->elapsedTime  == tasks[i]->period){
                tasks[i]->state = tasks[i] ->TickFct(tasks[i]->state);
                tasks[i]->elapsedTime  = 0;
            }
            tasks[i]->elapsedTime += 100;
        }
        while(!TimerFlag){};
        TimerFlag =0;
    }
    return 0;
}
