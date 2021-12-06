
#include "nokia5110.h"

#include <avr/pgmspace.h>
#include <avr/io.h>
#include <util/delay.h>
#include "assets.h"


static struct {
    /* screen byte massive */
    uint8_t screen[504];

    /* cursor position */
    uint8_t cursor_x;
    uint8_t cursor_y;

} nokia_lcd = {
    .cursor_x = 0,
    .cursor_y = 0
};

static void write(uint8_t bytes, uint8_t is_data)
{
	register uint8_t i;
	PORT_LCD &= ~(1 << LCD_SCE);
	if (is_data)
		PORT_LCD |= (1 << LCD_DC);
	else
		PORT_LCD &= ~(1 << LCD_DC);
	for (i = 0; i < 8; i++) {
		if ((bytes >> (7-i)) & 0x01)
			PORT_LCD |= (1 << LCD_DIN);
		else
			PORT_LCD &= ~(1 << LCD_DIN);
		PORT_LCD |= (1 << LCD_CLK);
		PORT_LCD &= ~(1 << LCD_CLK);
	}

	PORT_LCD |= (1 << LCD_SCE);
}

static void write_cmd(uint8_t cmd)
{
	write(cmd, 0);
}

static void write_data(uint8_t data)
{
	write(data, 1);
}

void nokia_lcd_init(void)
{
	register unsigned i;
	DDR_LCD |= (1 << LCD_SCE);
	DDR_LCD |= (1 << LCD_RST);
	DDR_LCD |= (1 << LCD_DC);
	DDR_LCD |= (1 << LCD_DIN);
	DDR_LCD |= (1 << LCD_CLK);

	PORT_LCD |= (1 << LCD_RST);
	PORT_LCD |= (1 << LCD_SCE);
	_delay_ms(10);
	PORT_LCD &= ~(1 << LCD_RST);
	_delay_ms(70);
	PORT_LCD |= (1 << LCD_RST);

	PORT_LCD &= ~(1 << LCD_SCE);
	write_cmd(0x21);
	write_cmd(0x13);
	write_cmd(0x06);
	write_cmd(0xC2);
	write_cmd(0x20);
	write_cmd(0x09);

	write_cmd(0x80);
	write_cmd(LCD_CONTRAST);
	for (i = 0; i < 504; i++)
		write_data(0x00);

	write_cmd(0x08);
	write_cmd(0x0C);
}

void nokia_lcd_clear(void)
{
	register unsigned i;
	write_cmd(0x80);
	write_cmd(0x40);
	nokia_lcd.cursor_x = 0;
	nokia_lcd.cursor_y = 0;
	for(i = 0;i < 504; i++)
		nokia_lcd.screen[i] = 0x00;
}

void nokia_lcd_power(uint8_t on)
{
	write_cmd(on ? 0x20 : 0x24);
}

void nokia_lcd_set_pixel(uint8_t x, uint8_t y, uint8_t value)
{
	uint8_t *byte = &nokia_lcd.screen[y/8*84+x];
	if (value)
		*byte |= (1 << (y % 8));
	else
		*byte &= ~(1 << (y %8 ));
}

void nokia_lcd_write_char(char code, uint8_t scale)
{
	register uint8_t x, y;

	for (x = 0; x < 5*scale; x++)
		for (y = 0; y < 7*scale; y++)
			if (pgm_read_byte(&CHARSET[code-32][x/scale]) & (1 << y/scale))
				nokia_lcd_set_pixel(nokia_lcd.cursor_x + x, nokia_lcd.cursor_y + y, 1);
			else
				nokia_lcd_set_pixel(nokia_lcd.cursor_x + x, nokia_lcd.cursor_y + y, 0);

	nokia_lcd.cursor_x += 5*scale + 1;
	if (nokia_lcd.cursor_x >= 84) {
		nokia_lcd.cursor_x = 0;
		nokia_lcd.cursor_y += 7*scale + 1;
	}
	if (nokia_lcd.cursor_y >= 48) {
		nokia_lcd.cursor_x = 0;
		nokia_lcd.cursor_y = 0;
	}
}

void nokia_lcd_write_string(const char *str, uint8_t scale)
{
	while(*str)
		nokia_lcd_write_char(*str++, scale);
}

void nokia_lcd_set_cursor(uint8_t x, uint8_t y)
{
	nokia_lcd.cursor_x = x;
	nokia_lcd.cursor_y = y;
}

void nokia_lcd_render(void)
{
	register unsigned i;
	write_cmd(0x80);
	write_cmd(0x40);
	for (i = 0; i < 504; i++)
		write_data(nokia_lcd.screen[i]);
}

void nokia_lcd_draw( uint8_t* image, uint8_t x, uint8_t y)
{
	int row, bit, byteColumn, lineSize, height, width;
	
	height = image[0];
	width = image[1];
	lineSize = divideRoundUp( width, 8 );
	
	for ( row = height - 1; row >= 0; row-- )
	{	
		for ( byteColumn = 0; byteColumn < lineSize; byteColumn++ )
		{
			for ( bit = 7; bit >= 0; bit--)
			{
				if ( (image[row*lineSize + byteColumn + 2] & (1 << bit) ) )
					nokia_lcd_set_pixel( x + (byteColumn * 8) + ( 7 - bit ), y + height - row, 1 );
			}
		} 
	}
}

int divideRoundUp(int num, int divisor)
{
	int i, quotient;
	for(i = num, quotient = 0; i > 0; i -= divisor, quotient++);
	return quotient;
}

void drawDino(int x)
{
	nokia_lcd_draw(dino, 0, x);
}

void drawDinoLeft()
{
	nokia_lcd_draw(dino_left, 0, 22);	
}

void drawDinoRight()
{
	nokia_lcd_draw(dino_right, 0, 22);
}

void drawDinoDownLeft()
{
	nokia_lcd_draw(dino_down_left, 0, 32);
}

void drawDinoDownRight()
{
	nokia_lcd_draw(dino_down_right, 0, 32);
}

void drawCactus(int x)
{
	nokia_lcd_draw(cactus, x, 32);
}

void drawBird(int x){
	nokia_lcd_draw(bird, x, 18);
}

void drawGround(){
	for (int i = 0; i < 84; i++){
		nokia_lcd_set_pixel(i, 48, 1);
	}
}