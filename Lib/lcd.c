#include<lcd.h>
 
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
 
// This implementation uses Port B for the Data port, all 8 bits of it
#define DATA_PORT PORTB
 
 
// This implementation uses Port B for the Control port
#define LCD_RS PIN2_bm		/* RS on pin PB2 */
#define LCD_E  PIN1_bm		/* E on pin PB1 */
#define COMM_PORT PORTA

void polbajt(unsigned char data)
{
  //PORTB.OUT=0x00;
  if(data&0x01)
  {
	  DATA_PORT.OUTSET=PIN5_bm;//dla portu A ->PIN4_bm;//|=0x10;//LCD_D4;
	 
  }
  else
  {
	  DATA_PORT.OUTCLR=PIN5_bm;//PIN4_bm;//=~(0x10);//LCD_D4;
  }

  if(data&0x02)
  {
	  DATA_PORT.OUTSET=PIN4_bm;//PIN5_bm;//|=0x20;//LCD_D5;
  }
  else
  {
	  DATA_PORT.OUTCLR=PIN4_bm;//PIN5_bm;//&=~(0x20);//LCD_D5;
  }

  if(data&0x04)
  {
	  DATA_PORT.OUTSET=PIN3_bm;//PIN6_bm;//|=0x40;//LCD_D6;
  }
  else
  {
	  DATA_PORT.OUTCLR=PIN3_bm;//PIN6_bm;//&=~(0x40);//LCD_D6;
  }

  if(data&0x08)	
  {
	  PORTB.OUTSET=PIN2_bm;//PIN7_bm;//|=0x80;//LCD_D7;
  }
  else
  {
	 DATA_PORT.OUTCLR=PIN2_bm;//PIN7_bm;//&=~(0x80);//LCD_D7;
  }
} 
 
// This function clears the RS line to write a command
void lcd_set_write_instruction(void) {
	COMM_PORT.OUTCLR = LCD_RS;		// set RS line low 
	_delay_us(50);
}
 
 
// This function sets the RS line to write data
void lcd_set_write_data(void) {
	COMM_PORT.OUTSET = LCD_RS;		// set RS line high
	_delay_us(50);
}
 
 
 
// This function writes a byte to the LCD
void lcd_write_byte (char c) {
	//c=(c<<2);
	//DATA_PORT.OUT = c;			// Place data on Data Port
	polbajt(c>>4);
	COMM_PORT.OUTSET = LCD_E;		// set E line high
	_delay_us(1);
	COMM_PORT.OUTCLR = LCD_E;		// set E line low to latch data into LCD
	
	polbajt(c);
	//_delay_us(1);
	COMM_PORT.OUTSET = LCD_E;		// set E line high
	_delay_us(1);
	COMM_PORT.OUTCLR = LCD_E;		// set E line low to latch data into LCD
	_delay_ms(10);
}
 
 
// This function clears LCD and sets address to beginning of first line
void lcd_clear_and_home(void) {
	lcd_set_write_instruction();
	lcd_write_byte(0x01);
	_delay_ms(50);
	lcd_write_byte(0x02);
	_delay_ms(50);
}
 
 
// This function sets address to beginning of first line
	void lcd_home() {
	lcd_set_write_instruction();
	lcd_write_byte(0x02);
	_delay_ms(50);
}
 
 
// This function moves cursor to a given line and position
//  line is either 0 (first line) or 1 (second line)
//  pos is the character position from 0 to 15.
void lcd_goto(uint8_t line, uint8_t pos)
{
	uint8_t position = 0;
 
	lcd_set_write_instruction();
	switch(line)
	{
		case 0: position = 0;
		break;
		case 1: position = 0x40;
		break;
	}
		lcd_write_byte(0x80 | (position + pos));
	}
 
 
// This function moves the cursor to 1st character of 1st line
void lcd_line_one(void) { lcd_goto(0, 0); }
 
 
// This function moves the cursor to 1st character if 2nd line
void lcd_line_two(void) { lcd_goto(1, 0); }
 
 
// This function writes a character to the LCD
void lcd_write_data(char c) {
	lcd_set_write_data();
	lcd_write_byte(c);
}
 
 
// This function writes a string (in SRAM) of given length to the LCD
void lcd_write_string(char *x, uint8_t len ) {
	while (--len > 0)
		lcd_write_data(*x++);
}
 
 
// This function writes a null-terminated string (in SRAM) to the LCD
void lcd_write_string_0(char *x) {
	while (*x)
		lcd_write_data(*x++);
}
 
 
// Same as above, but the string is located in program memory,
//  so "lpm" instructions are needed to fetch it, and a \0
//  must be defined at the end of the string to terminate it.
void lcd_write_string_p(const char *s)
{
	char c;
 
	for (c = pgm_read_byte(s); c; ++s, c = pgm_read_byte(s))
		lcd_write_data(c);
}
 
 
// This function initializes the LCD plus any AVR ports etc
void lcd_init(void) {

  portENTER_CRITICAL();
  {
    xLCDrec = xQueueCreate(32, ( unsigned portBASE_TYPE ) sizeof( signed portCHAR ));
    }  
  portEXIT_CRITICAL();
	// Initialise the AVR ports and any other hardware bits
	DATA_PORT.OUT = 0x00;							// initial data lines all low
	DATA_PORT.DIRSET = 0xff;						// set the 8-bit data port to all outputs
	COMM_PORT.OUTCLR = LCD_E | LCD_RS;			// all LCD control lines low
	COMM_PORT.DIRSET = LCD_E | LCD_RS;  			// set the LCD control line pins to outputs
 
	// Now do the actual LCD initialisations
 
	// startup delay - make it long to allow time for power to settle
	// (you want wish to remove this line)
	_delay_ms(500);
	DATA_PORT.OUTSET=PIN4_bm|PIN5_bm;
	COMM_PORT.OUTSET = LCD_E;
	
	_delay_us(1);
	COMM_PORT.OUTCLR = LCD_E;
	_delay_ms(5); // czekaj 5ms
	
	// LCD display initialisation by instruction magic sequence
	lcd_set_write_instruction();
	lcd_write_byte(0x28);//(HD44780_FUNCTION_SET | HD44780_FONT5x7 | HD44780_TWO_LINE | HD44780_4_BIT);			// function set
	_delay_us(50);
	lcd_set_write_instruction();
	lcd_write_byte(0x28);//(HD44780_FUNCTION_SET | HD44780_FONT5x7 | HD44780_TWO_LINE | HD44780_4_BIT);			// function set
	_delay_us(50);
	lcd_set_write_instruction();
	lcd_write_byte(HD44780_DISPLAY_ONOFF | HD44780_DISPLAY_OFF);			// display on/off control
	_delay_us(50);
	lcd_set_write_instruction();
	lcd_write_byte(0x01);			// display clear
	_delay_ms(5);
	lcd_set_write_instruction();
	lcd_write_byte(0x06);			// entry mode set
	_delay_ms(5);
	//lcd_write_byte(0x14); 			// Cursor shift
	
 
	lcd_clear_and_home(); 			// LCD cleared and cursor is brought to 
         						// the beginning of 1st line
 
}