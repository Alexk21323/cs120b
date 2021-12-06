/*	Author: Alex Kuang
 *  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Custom Project 
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *
 *  Demo Week 1 Display game assests:  https://www.youtube.com/watch?v=hYn4I0ElAN8&ab_channel=alexkuang
 * 
 */
#include <avr/io.h>
#include <util/delay.h>
#include "nokia5110.h"
#include "scheduler.h"
#include "timer.h"
#include <avr/interrupt.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

unsigned char up = 0;
unsigned char height = 20;
unsigned char obstacle_distance = 74;
unsigned short input;
enum gameState{ gameStart, gameInit };
int gameSMTick(int state)
{
  switch (state)
  {
  case gameStart:
    state = gameInit;
    break;

  case gameInit:
    break;

  default:
    state = gameStart;
    break;
  }
  switch (state)
  {
  case gameStart:
    break;

  case gameInit:
    input = ADC;
    if(input > 0 && input < 200 && height < 22 )
      height  += 2;
    else if (input > 800 &&  height > 0) //down 
      height  -= 2;
    // else if (input > 400 && input <= 600 && obstacle_distance < 74) //right 
    //   obstacle_distance += 1;
    // else if (input > 600 && input <= 800 && obstacle_distance > 0)// down 
    //   obstacle_distance -= 1;
    // if (up &&  height > 0)
    // {
    //   height -= 4; 
    //   if (height == 0)
    //     up = 0;
    // }
    // else if (!up && height < 20)
    // {
    //   height += 4;
    //   if (height == 20)
    //     up = 1;
    // }
    // if (obstacle_distance > 0)
    //   obstacle_distance -= 2;
    // else if (obstacle_distance == 0)
    //   obstacle_distance = 74;
    
    nokia_lcd_clear();
    drawDino(height);
    drawCactus(obstacle_distance);
    nokia_lcd_render();
    break;

  default:
    break;
  }
  return state;
}

//Render Using Queue
enum obstacleState{ obstacleStart, obstacleInit };
int obstacleSMTick(int state)
{
  switch (state)
  {
  case obstacleStart:
    state = obstacleInit;
    break;

  case obstacleInit:
    break;

  default:
    state = obstacleStart;
    break;
  }
  switch (state)
  {
  case obstacleStart:
    break;

  case obstacleInit:
    for (int i = 74; i > 0; i -=2)
    {
      nokia_lcd_clear();
      drawCactus(i);
    }
    default:
      break;
  }
}

void A2D_init() {
	ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADATE);
}

int main(void)
{
  nokia_lcd_init();
  nokia_lcd_clear();

  A2D_init();
  static task task1;
  task *tasks[] = {&task1};
  const unsigned short numTasks = sizeof(tasks) / sizeof(task *);
  const char start = -1;

  task1.state = start;
  task1.period = 10;
  task1.elapsedTime = task1.period;
  task1.TickFct = &gameSMTick;
  
  TimerSet(10);
  TimerOn();

  unsigned short i;
  while (1)
  {
    for (i = 0; i < numTasks; ++i)
    {
      if (tasks[i]->elapsedTime == tasks[i]->period)
      {
        tasks[i]->state = tasks[i]->TickFct(tasks[i]->state);
        tasks[i]->elapsedTime = 0;
      }
      tasks[i]->elapsedTime += 1;
    }
    while (!TimerFlag);
    TimerFlag = 0;
  }
}