#ifndef LCD_H
#define LCD_H
// PB5 D4, PB2 D7
#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"
#include "main.h"
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>


#define HD44780_CLEAR					0x01
#define HD44780_HOME					0x02
#define HD44780_ENTRY_MODE				0x04
	#define HD44780_EM_SHIFT_CURSOR		0
	#define HD44780_EM_SHIFT_DISPLAY	1
	#define HD44780_EM_DECREMENT		0
	#define HD44780_EM_INCREMENT		2
#define HD44780_DISPLAY_ONOFF			0x08
	#define HD44780_DISPLAY_OFF			0
	#define HD44780_DISPLAY_ON			4
	#define HD44780_CURSOR_OFF			0
	#define HD44780_CURSOR_ON			2
	#define HD44780_CURSOR_NOBLINK		0
	#define HD44780_CURSOR_BLINK		1

#define HD44780_DISPLAY_CURSOR_SHIFT	0x10
	#define HD44780_SHIFT_CURSOR		0
	#define HD44780_SHIFT_DISPLAY		8
	#define HD44780_SHIFT_LEFT			0
	#define HD44780_SHIFT_RIGHT			4
#define HD44780_FUNCTION_SET			0x20
	#define HD44780_FONT5x7				0
	#define HD44780_FONT5x10			4
	#define HD44780_ONE_LINE			0
	#define HD44780_TWO_LINE			8
	#define HD44780_4_BIT				0
	#define HD44780_8_BIT				16
#define HD44780_CGRAM_SET				0x40
#define HD44780_DDRAM_SET				0x80

//#define LCD_DATA PORTB.0UT;
#define LCD_D4 0x20
#define LCD_D5 0x10
#define LCD_D6 0x08
#define LCD_D7 0x04
extern xQueueHandle         xLCDrec;
unsigned char odwroc(unsigned char dana);
void polbajt(unsigned char data);
void lcd_set_write_instruction(void);
void lcd_set_write_data(void);
void lcd_write_byte (char c);
void lcd_clear_and_home(void);
void lcd_home(void);
void lcd_goto(uint8_t line, uint8_t pos);
void lcd_line_one(void);
void lcd_line_two(void);
void lcd_write_data(char c);
void lcd_write_string(char *x, uint8_t len );
void lcd_write_string_0(char *x);
void lcd_write_string_p(const char *s);
void lcd_init(void);
#endif
