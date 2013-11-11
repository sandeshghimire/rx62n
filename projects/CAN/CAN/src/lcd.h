/***********************************************************************************
FILE NAME		lcd.h

DESCRIPTION		Driver for KS0066u LCD Module Controller (8 characters by 2 lines )
				on the Renesas RDK boards - header file

Copyright   : 2008 Renesas Technology Europe Ltd.
Copyright   : 2008 Renesas Technology Corporation.
All Rights Reserved
***********************************************************************************/

/***********************************************************************************
Revision History
DD.MM.YYYY OSO-UID Description
05.03.2008 RTE-MBA First Release
***********************************************************************************/
#include <stdint.h>

#ifndef LCD_H
#define LCD_H

/**********************************************************************************
User Includes
***********************************************************************************/
/* rsk1668def.h provides common defines for widely used items. */
#include	"rdkrx62n.h"

#define LCD_FLASH	{Delay(0x00400000); DisplayString(LCD_LINE1, "        "); \
					 DisplayString(LCD_LINE2, "        "); Delay(0x00002000);}


/* RS Register Select pin */
#define RS_PIN			PORT5.DR.BIT.B1

/* Display Enable pin */  
#define EN_PIN			PORTC.DR.BIT.B2
	
/* Data bus port */
#define DATA_PORT		PORTE.DR.BYTE

/* Bit mask from entire port */   
#define DATA_PORT_MASK	0xF0

#define DATA_WR 1
#define CTRL_WR 0

/* Set to ensure base delay of 1uS minimum */
#define DELAY_TIMING	0x80

/* Maximum characters per line of LCD display */ 
#define NUMB_CHARS_PER_LINE 8

/* Number of lines on the LCD display */
//#define MAXIMUM_LINES	2   

#define LCD_LINE1       0
#define LCD_LINE2       16
#define LCD_LINE3       24
#define LCD_LINE4       32

/**********************************************************************************/
/* LCD commands - use LCD_write function to write these commands to the LCD.	  */
/**********************************************************************************/
/* Clear LCD display and home cursor */
#define LCD_CLEAR		0x01
/* move cursor to line 1 */
#define LCD_HOME_L1		0x80
/* move cursor to line 2 */	  
#define LCD_HOME_L2		0xC0
/* Cursor auto decrement after R/W */  
#define CURSOR_MODE_DEC	0x04
/* Cursor auto increment after R/W */
#define CURSOR_MODE_INC	0x06
/* Setup, 4 bits,2 lines, 5X7 */
#define FUNCTION_SET	0x28
/* Display ON with Cursor */
#define LCD_CURSOR_ON	0x0E
/* Display ON with Cursor off */
#define LCD_CURSOR_OFF	0x0C
/* Display on with blinking cursor */
#define LCD_CURSOR_BLINK 0x0D
/* Move Cursor Left One Position */
#define LCD_CURSOR_LEFT	0x10
/* Move Cursor Right One Position */
#define LCD_CURSOR_RIGHT 0x14

#define LCD_DISPLAY_ON	0x04
#define LCD_TWO_LINE	0x08

#define FOUR_DIGITS_PLUS_NULL   5

/**********************************************************************************/
/* LCD functions - declaration of main LCD functions.	  */
/**********************************************************************************/
void InitialiseDisplay(void);
void DisplayString(uint8_t position, uint8_t* string);
void LCD_write(unsigned char data_or_ctrl, unsigned char value);
void LCD_nibble_write(unsigned char data_or_ctrl, unsigned char value);
void DisplayDelay(unsigned long int units);
//void Convert_16BitNumber_ToString(unsigned int c, char* buffer);
void LcdShow2DigHex( uint8_t  data, uint8_t  position);
void LcdShow4DigHex(	uint8_t  msbyte, 
						uint8_t  lsbyte,
						uint8_t	 position);
void IntToAsciiHex( uint8_t*    dest_string,
					uint8_t		min_digits, 
					uint32_t 	value );
						
/* LCD_H */
#endif 
