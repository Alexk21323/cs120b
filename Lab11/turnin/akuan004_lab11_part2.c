/*	Author: Alex Kuang
 *  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Lab #  Exercise #
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *
 *  Demo Link:  https://www.youtube.com/watch?v=gWAjq51G0R0&ab_channel=alexkuang
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

enum KeypadSstates {KeypadStart, KeyPadInit};
unsigned char x;
unsigned char keypad_input = 0x00;
int KeypadSMTick(int state){
    x = GetKeypadKey();
    switch (state)
    {
    case KeypadStart:
        state = KeyPadInit;
        break;
    
    case KeyPadInit:
        state = KeyPadInit; 
        break;

    default:
        state = KeypadStart;
        break;
    }

    switch (state)
    {
    case KeypadStart:
        break;
    
    case KeyPadInit:
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

enum LCDStates {LCDStart, LCDInit};
unsigned char str[] ="     CS120B is Legend... wait for it DARY!";
unsigned char i;
unsigned char lcd[16];
int LCDSMTick(int state) {
    switch (state)
    {
    case LCDStart:
        state = LCDInit;
        break;
    
    case LCDInit: 
        state = LCDInit;
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
        for (int j = 0; j<16; j++){
            lcd[j] = str[i+j];   
        }
        LCD_DisplayString(1,lcd);
        i = (i+1)%42;
        break;
    }
    return state;
}

int main(void) {
    DDRA = 0xFF; PORTA = 0x00;
    DDRB = 0xFF; PORTB = 0x00;
    DDRC = 0xF0; PORTC = 0x0F;
    DDRD = 0xFF; PORTD = 0x00;

    LCD_init();
    static task task1, task2;
    
    task *tasks[] = {&task1, &task2};
    const unsigned short numTasks = sizeof(tasks)/sizeof(task*);

    const char start = -1;
    task1.state = start;
    task1.period = 100;
    task1.elapsedTime = task1.period;
    task1.TickFct = &KeypadSMTick;

    task2.state = start;
    task2.period = 500;
    task2.elapsedTime = task2.period;
    task2.TickFct = &LCDSMTick;
    TimerSet(500);
    TimerOn();
    unsigned short i;
    while (1) {
        for (i = 0; i < numTasks; i++){
            if (tasks[i]->elapsedTime  == tasks[i]->period){
                tasks[i]->state = tasks[i] ->TickFct(tasks[i]->state);
                tasks[i]->elapsedTime  = 0;
            }
            tasks[i]->elapsedTime += 500;
        }
        while(!TimerFlag){};
        TimerFlag =0;
    }
    return 0;
}
