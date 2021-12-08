#ifndef __NOKIA_5110_H__
#define __NOKIA_5110_H__

#include <avr/pgmspace.h>
#include <stdint.h>

#define PORT_LCD PORTB
#define DDR_LCD DDRB

#define LCD_SCE PB1
#define LCD_RST PB2
#define LCD_DC PB3
#define LCD_DIN PB4
#define LCD_CLK PB5

#define LCD_CONTRAST 0x40

void nokia_lcd_init(void);

void nokia_lcd_clear(void);

void nokia_lcd_set_pixel(uint8_t x, uint8_t y, uint8_t value);

void nokia_lcd_write_char(char code, uint8_t scale);

void nokia_lcd_write_string(const char *str, uint8_t scale);

void nokia_lcd_set_cursor(uint8_t x, uint8_t y);

void nokia_lcd_render(void);

int divideRoundUp(int num, int divisor);

void nokia_lcd_draw( uint8_t* image, uint8_t x, uint8_t y);

void drawDino(int x);

void drawDinoLeft();

void drawDinoRight();

void drawDinoDownLeft();

void drawDinoDownRight();

void drawCactus(int x);

void drawBird(int x);

void drawGround();
#endif
