/***********************************************************************************
FILE NAME 		lcd.c
DESCRIPTION 	LCD Module utility functions.
		Written for KS0066u compatible LCD Module.
		(8 characters by 2 lines)

Copyright : 2008 Renesas Technology Europe Ltd.
Copyright : 2008 Renesas Technology Corporation.
All Rights Reserved
***********************************************************************************/

/***********************************************************************************
Revision History
DD.MM.YYYY OSO-UID Description
05.03.2008 RTE-MBA First Release
***********************************************************************************/

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include <machine.h>
/* Defines for I/O registers */
#include <stdio.h>
#include <stdint.h>
#include <string.h>  
/* rdkrx62n.h provides common defines for widely used items. */
#include "rdkrx62n.h"
/* Header files for lcd.c */
#include "lcd.h"
#include "Glyph_API.h"
#include "iodefine.h"

/******************************************************************************
Typedef definitions
******************************************************************************/

/******************************************************************************
Macro definitions
******************************************************************************/
#define LCDOKAYA	1
//#define LCD2BY8	1

/******************************************************************************
Imported global variables and functions (from other files)
******************************************************************************/
/******************************************************************************
Exported global variables and functions (to be accessed by other files)
******************************************************************************/
#if LCD2BY8
static unsigned char next_pos;
#endif

/******************************************************************************
Private global variables and functions
******************************************************************************/
T_glyphHandle G_lcd ;

/**********************************************************************************
User Program Code
***********************************************************************************/
#if LCD2BY8
/*****************************************************************************
Name:			InitialiseDisplay
Description:	Intializes the LCD display. 
Parameters:		none				
Returns:		none
*****************************************************************************/
void InitialiseDisplay(void)
{
	/* Variable initialization */
	next_pos = 0xFF;
	
	/* Power Up Delay for LCD Module */
	EN_PIN = SET_BIT_HIGH;
	DisplayDelay(7000);
	EN_PIN = SET_BIT_LOW;
	
	/* Display initialises in 8 bit mode - so send one write (seen as 8 bit)
		to set to 4 bit mode. */

	/* Function Set */
	LCD_nibble_write(CTRL_WR, 0x03);
	LCD_nibble_write(CTRL_WR, 0x03);
	DisplayDelay(39);
 
	/* Configure display */
	LCD_nibble_write(CTRL_WR, 0x03);
	LCD_nibble_write(CTRL_WR, 0x02);
	LCD_nibble_write(CTRL_WR, (LCD_DISPLAY_ON | LCD_TWO_LINE));
	LCD_nibble_write(CTRL_WR, (LCD_DISPLAY_ON | LCD_TWO_LINE));
	DisplayDelay(39);

	/* Display ON/OFF control */
	LCD_write(CTRL_WR, LCD_CURSOR_OFF);
	DisplayDelay(39);

	/* Display Clear */
	LCD_write(CTRL_WR, LCD_CLEAR);
	DisplayDelay(1530);

	/* Entry Mode Set */
	LCD_write(CTRL_WR, 0x06);
	LCD_write(CTRL_WR, LCD_HOME_L1);
}
/**********************************************************************************
End of function InitialiseDisplay
***********************************************************************************/   

/*****************************************************************************
Name:		DisplayString
Description:	This function controls LCD writes to line 1 or 2 of the LCD.
				You need to use the defines LCD_LINE1 and LCD_LINE2 in order
				to specify the starting position.
				For example, to start at the 2nd position on line 1...
				DisplayString(LCD_LINE1 + 1, "Hello")
Parameters:	position  Line number of display
			string	Pointer to data to be written to display.
					Last character should be null.
Returns:	none
*****************************************************************************/
void DisplayString(uint8_t position, uint8_t* string)
{
	/* Set a line position if needed. If current cursor position is outside of the 
	NUMB_CHARS_PER_LINE value, the current data writes are skipped.
	If data are written byte to byte, no CTRL writes are written to LCD module
	(CTRL writes takes more time) */
	if (next_pos != position)
	{
		if ((position >= NUMB_CHARS_PER_LINE && position <= (LCD_LINE2 - 1)) 
			|| position >= (NUMB_CHARS_PER_LINE + LCD_LINE2))
		{
			return;
		}
		
		if (position < LCD_LINE2)
		{
			/* Configure LCD to write on Line 1 */
			LCD_write(CTRL_WR, (unsigned char)(LCD_HOME_L1 + position));
		}
		else
		{
			/* Configure LCD to write on Line 2 */
			LCD_write(CTRL_WR, (unsigned char)(LCD_HOME_L2 + position - LCD_LINE2));
		}
		/* Set position index to known value */
		next_pos = position;	
	}

	do
	{
		/* Check for current position, if outside of NUMB_CHARS_PER_LINE, break */
		if (next_pos == NUMB_CHARS_PER_LINE || next_pos == (NUMB_CHARS_PER_LINE + LCD_LINE2))
		{
			break;
		}
		
		/* Write symbol to current LCD position */
		LCD_write(DATA_WR,*string++);
		
		/* Increment position index */
		next_pos++;
	}
	while (*string);
}
/**********************************************************************************
End of function DisplayString
***********************************************************************************/   

#else	//LCDOKAYA///////////////////////////////////////////////////////////

/*****************************************************************************
Name:          	InitialiseDisplay 
Parameters:     -
Returns:        -
Description:    Intializes the LCD. 
*****************************************************************************/
void InitialiseDisplay( void )
{
  if (GlyphOpen(&G_lcd, 0) == GLYPH_ERROR_NONE) 
  {
      /* use Glyph full access direct functions */
      GlyphNormalScreen(G_lcd) ;
      GlyphSetFont(G_lcd, GLYPH_FONT_8_BY_16) ;
      GlyphClearScreen(G_lcd) ;
  }
}

/*****************************************************************************
Name:        	DisplayString   
Parameters:  	position	Line number of display
			    string		Pointer to data to be written to display.
			    			Last character should be null.
Returns:        none
Description:    This function controls LCD writes to line 1 or 2 of the LCD.
                You need to use the defines LCD_LINE1 and LCD_LINE2 in order
				to specify the starting position.
				For example, to start at the 2nd position on line 1...
				   DisplayString(LCD_LINE1 + 1, "Hello")
*****************************************************************************/
void DisplayString(uint8_t lineNum, uint8_t* string)
{
  // Figure out the number of characters
  // Clear the screen for n characters
  // Write the new characters

  int8_t y = lineNum - (lineNum % 8);
  int8_t xOffset = (lineNum % 8)<<3 ;

  /* Draw text lines, 16 pixels high, 96 pixels wide */

  /* Clear the rectangle of this line */
  GlyphEraseBlock(G_lcd, xOffset, y, (95 - xOffset), y+15);
  GlyphSetXY (G_lcd, xOffset, y);
  GlyphString(G_lcd, (uint8_t *)string, strlen((const char*)string));
}
#endif	//LCD type

#if LCD2BY8
/*****************************************************************************
Name:		LCD_write
Description:	Writes data to display. Sends command to display.  
Parameters:	value - the value to write
			data_or_ctrl - To write value as DATA or CONTROL
							1 = DATA
							0 = CONTROL
Returns:	none
*****************************************************************************/
void LCD_write(unsigned char data_or_ctrl, unsigned char value)
{
	/* Write upper nibble first */
	LCD_nibble_write(data_or_ctrl, (value & 0xF0) >> 4);

	/* Write lower nibble second */
	LCD_nibble_write(data_or_ctrl, (value & 0x0F));
}
/**********************************************************************************
End of function LCD_write
***********************************************************************************/   

/*****************************************************************************
Name:		LCD_nibble_write
Description:	Writes data to display. Sends command to display.  
Parameters:	value - the value to write
			data_or_ctrl - To write value as DATA or CONTROL
							1 = DATA
							0 = CONTROL
Returns:	none
*****************************************************************************/
void LCD_nibble_write(unsigned char data_or_ctrl, unsigned char value)
{
	unsigned char ucStore;
	/* Check for the type of data and set RS_PIN */
	if (data_or_ctrl == DATA_WR)
	{
		RS_PIN = SET_BIT_HIGH;
	}
	else
	{
		RS_PIN = SET_BIT_LOW;
	}
	/* There must be 40ns between RS write and EN write */
	DisplayDelay(2);
	/* EN enable chip (HIGH) */
	EN_PIN = SET_BIT_HIGH;
	/* Tiny delay */		
	DisplayDelay(2);
	/* Clear port bits used */  
	/* Set upper 4 bits of nibble on port pins. */
	ucStore = DATA_PORT;
	ucStore &= ~DATA_PORT_MASK;
	/* OR in data */  
	ucStore |= ((value << 4) & DATA_PORT_MASK );
	/* Write lower 4 bits of nibble */
	DATA_PORT = ucStore;

	/* Write delay while En High */
	DisplayDelay(20);
	/* Latch data by dropping EN */	 
	EN_PIN = SET_BIT_LOW;
	/* Data hold delay */	   
	DisplayDelay(40);		 

	if (data_or_ctrl == CTRL_WR)
	{
		/* Extra delay needed for control writes */
		DisplayDelay(80);	   
	}	   
}
/**********************************************************************************
End of function LCD_nibble_write
***********************************************************************************/   

/*****************************************************************************
Name:		DisplayDelay
Description:   Delay routine for LCD display.
Parameters:	units - Approximately in microseconds				   
Returns:	none 
*****************************************************************************/
void DisplayDelay(unsigned long int units)
{
	unsigned long counter = units * DELAY_TIMING;
	while (counter--)
	{
		nop();
	}
}
/**********************************************************************************
End of function DisplayDelay
***********************************************************************************/   
#endif	//LCD2BY8

/*****************************************************************************
Name:          LcdShow2DigHex
Parameters:    data, LCD-display line 1 or 2
Returns:       -
Description:   convert one byte to 2 ascii chars and show them.
*****************************************************************************/
void LcdShow2DigHex( uint8_t  data, uint8_t  position)
{
	uint8_t stringdata[3]; /* One char extra for trailing NULL. */

	/* Convert data to string for LCD. */
	IntToAsciiHex( stringdata, /* Pointer to where to put string */
				   (uint8_t) 2, /* Nr characters output string will have */
				   data ); /* uint32_t value */
	
	DisplayString(position, (uint8_t*)stringdata);
}/* end LcdShow2DigHex() */

/*****************************************************************************
Name:          LcdShow4DigHex
Parameters:    msbyte, lsbyte, LCD-display line 1 or 2
Returns:       -
Description:   Convert two bytes to 4 ascii characters and show them to LCD 
               at 5th position line 1 or 2.
*****************************************************************************/
void LcdShow4DigHex(	uint8_t  	msbyte,
						uint8_t  	lsbyte,
						uint8_t	    position)
{
	uint32_t intdata;
	uint8_t stringdata[FOUR_DIGITS_PLUS_NULL];

	intdata = ((uint32_t) msbyte)<<8;
	intdata = intdata + (uint32_t)lsbyte;

	/* Convert data to string for LCD. */
	IntToAsciiHex( 	stringdata, /* Pointer to where to put string */
				   	FOUR_DIGITS_PLUS_NULL-1, /* Nr characters output string will have */
					intdata  ); /* Value */
	
	DisplayString(position, (uint8_t*)stringdata);
}/* end LcdShow4DigHex() */

/*****************************************************************************
Name:		IntToAsciiHex   
Parameters:	dest_string
				Pointer to a buffer will the string will reside
			min_digits
				Specifies the number of characters the output string will
				have. Leading zeros will be written as '0' characters.
Returns:	A pointer to the string's NULL character in the string that was just
				created.
Does:		Converts a passed uint32_t into a ASCII	string represented in 
				Hexidecimal format.
*****************************************************************************/
void IntToAsciiHex( uint8_t*    dest_string,
					uint8_t		min_digits, 
					uint32_t 	value )
{
	uint32_t i, total_digits = 0;
	uint8_t  buff[4];
	
	for(i=0; i<4; i++)
	{
		buff[i] = (uint8_t)(value & 0x0F);
		value = value >> 4;
		if( buff[i] <= 9)
			buff[i] += '0';
		else
			buff[i] = (uint8_t)(buff[i] - 0xA + 'A');

		if(buff[i] != '0')
			total_digits = i+1;
	}

	if( total_digits < 	min_digits)
		total_digits = min_digits;

	i = total_digits;
	while(i)
	{
		*dest_string++ = buff[i-1];
		i--;
	}

	*dest_string = 0;
}/* end IntToAsciiHex() */
/* eof */