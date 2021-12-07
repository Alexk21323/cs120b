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
#include <time.h>
#include <stdio.h>  // to use rand 
#include <stdlib.h> // to use rand 
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

//direction
unsigned char up = 0;
unsigned char down = 0;
unsigned char left = 1;
unsigned char crouch = 0;

//jump and hover
unsigned char height = 22;
unsigned char count = 5;

//enemy distance and type
unsigned char type = 0;
signed char obstacle_distance = 80;

//joy stick input
unsigned short input;
enum gameState{ gameStart, gameInit, gameEnd};
int gameSMTick(int state)
{
  switch (state)
  {
  case gameStart:
    type = 0;
    state = gameInit;
    break;

  case gameInit:
    input = ADC;

    if (input > 0 && input < 200 &&  height > 0)
      crouch = 1;
    else 
      crouch  = 0; 
    
    if (input > 800 &&  height > 0 && down != 1)
      up = 1;
    break;

  case gameEnd:
    input = ADC;
    if (input > 800)
      state = gameInit;
    else 
      state = gameEnd;
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
    // jump movement 
    if(up && height > 0)
      height -= 5;
    
    if (up && height == 2){
      height -= 2;
    }

    // hover for 7 frame 
    if (height == 0)
    {
      count -= 1; 
      if (count == 0)
      {
        count = 5;
        up = 0;
        down = 1;
      }
    }
    // fall movement
    if (down && height < 20)
      height += 5;

    if (down && height == 20 ){
      height += 2;
      down = 0;
    }
    //cactus movement
    if (obstacle_distance > 0)
      obstacle_distance -= 5;
    else if (obstacle_distance == 0)
    {
      type = rand()%2;
      if (type == 1)
        obstacle_distance = 65;
      else 
        obstacle_distance = 80;     
    }
    //render obstacle 
     
    nokia_lcd_clear();
    if(type == 1)
      drawBird(obstacle_distance);
    else
      drawCactus(obstacle_distance);
    
    //render dino 
    if (left && !up && !down && !crouch ){
      drawDinoLeft();
      left = 0;
    }

    else if (!left && !up && !down && !crouch) {
      drawDinoRight();
      left = 1;
    }

    if (left && !up && !down && crouch){
      drawDinoDownLeft();
      left = 0;
    }
    
    else if  (!left && !up && !down && crouch){
      drawDinoDownRight();
      left = 1;
    }

    if (up || down )
      drawDino(height);
    
    //check for hitbox 
    if (type == 1 && obstacle_distance < 22 && !crouch){
      nokia_lcd_clear();
      state = gameEnd;
    }
    if (type != 1 && obstacle_distance < 22 && height > 7){
      nokia_lcd_clear();
      state = gameEnd;
    }
    nokia_lcd_render();
    break;

  case gameEnd:
    //reset game state 
    height = 22; 
    crouch = 0;
    up = 0;
    left  = 1;
    obstacle_distance = 80;
    type = 0;
    nokia_lcd_set_cursor(0, 0);
    nokia_lcd_write_string("GameOver", 2);
    nokia_lcd_set_cursor(0, 30);
    nokia_lcd_write_string("move to start", 1);
    nokia_lcd_render() ;
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
  srand(time(0));
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