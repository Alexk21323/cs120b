/*	Author: Alex Kuang
 *  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Lab #  Exercise #
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *
 *  Demo Link:  https://www.youtube.com/watch?v=KhOd93f6S44&ab_channel=alexkuang
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
    break;
    }
    return state;
}

enum LCDWriteStates {LCDWriteStart, LCDWriteInit, LCDWrite};
unsigned char lcd;
unsigned char index;
int LCDWriteSMTick(int state) {
    switch (state)
    {
    case LCDWriteStart:
        state = LCDWriteInit;
        break;
    
    case LCDWriteInit: 
        if (keypad_input == 0x1F)
            state = LCDWriteInit;
        else 
            state = LCDWrite;
        break;

    case LCDWrite:
        state = LCDWriteInit;
        break;

    default:
        state = LCDWriteStart;
        break;
    }

    switch (state)
    {
    case LCDWriteStart:
        break;

    case LCDWriteInit:
        LCD_Cursor(index);
        break;
    
    case LCDWrite:
        LCD_WriteData(x);
        if(index < 16)
            index++;
        else 
            index = 0;
        break;
    
    default:
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
    task1.period = 10;
    task1.elapsedTime = task1.period;
    task1.TickFct = &KeypadSMTick;

    task2.state = start;
    task2.period = 20;
    task2.elapsedTime = task2.period;
    task2.TickFct = &LCDWriteSMTick;

    TimerSet(10);
    TimerOn();
    unsigned short i;
    LCD_DisplayString(1, "Congratulations!");

    while (1) {
        for (i = 0; i < numTasks; i++){
            if (tasks[i]->elapsedTime  == tasks[i]->period){
                tasks[i]->state = tasks[i] ->TickFct(tasks[i]->state);
                tasks[i]->elapsedTime  = 0;
            }
            tasks[i]->elapsedTime += 1;
        }
        while(!TimerFlag){};
        TimerFlag =0;
    }
    return 0;
}
