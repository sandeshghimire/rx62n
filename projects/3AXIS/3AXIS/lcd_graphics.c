/******************************************************************************
* DISCLAIMER

* This software is supplied by Renesas Electronics Corp. and is
* only intended for use with Renesas products.  

* No other uses are authorized.

* This software is owned by Renesas Electronics Corp. and is 
* protected under the applicable laws, including copyright laws.

* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES
* REGARDING THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY,
* INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, 
* FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT.  ALL SUCH
* WARRANTIES ARE EXPRESSLY DISCLAIMED.

* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER 
* RENESAS ELECTRONICS CORP. NOR ANY OF ITS AFFILIATED COMPANIES
* SHALL BE LIABLE FOR AND DIRECT, INDIRECT, SPECIAL, INCIDENTAL
* OR COSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS SOFTWARE,
* EVEN IF RENESAS OR ITS AFFILIATES HAVE BEEN ADVISED OF THE
* POSSIBILITIES OF SUCH DAMAGES.

* Renesas reserves the right, without notice, to make changes to this
* software and to discontinue availability of this software.
* By using this software, you agree to the additional terms and 
* conditions found by accessing the following link:
* http://www.renesas.com/disclaimer
*******************************************************************************/
/* Copyright (C) 2010. Renesas Electronics Corp., All Rights Reserved         */
/******************************************************************************
* File Name     : lcd_graphics.c
* Version       : 1.1
* Device(s)     : R5F562N8
* Tool-Chain    : Renesas RX Standard Toolchain 1.0.1
* OS            : None
* H/W Platform  : YRDKRX62N
* Description   : Sample code to interface with the YRDKRX62N debug LCD at the
*                 pixel level. It utilizes the Renesas Peripheral Driver Library
*                 (RPDL) to drive data via the DTC and SPI pipeline to the the 
*                 LCD conroller.
* Limitations   : None
*******************************************************************************
* History : DD.MMM.YYYY   Version    Description
*         : 08.Oct.2010   1.00       First release
*         : 02.Dec.2010   1.10       Second YRDK release
*******************************************************************************/

/*******************************************************************************
* Project Includes
*******************************************************************************/    
/* Defines standard variable types used in this function */
#include <stdint.h>
#include <string.h>

/* RPDL definitions */
#include "r_pdl_spi.h"
#include "r_pdl_dtc.h"
#include "r_pdl_intc.h"
#include "r_pdl_cmt.h"
#include "r_pdl_definitions.h"

/* LCD Graphics definitions */
#include "lcd_graphics.h"

/* Local functions */
static void SendToLCD(const void * buf, uint16_t n) ;
static void init_LCD_Hardware( void );
static void set_LCD_XY_Coordinates( uint8_t, uint8_t );
static void clear_LCD_Block    ( IMAGE_BUF *, uint8_t, uint8_t, uint8_t, uint8_t );
static void clear_raster_buffer( RASTER_BUF * );
static void fill_raster_buffer ( IMAGE_BUF *, RASTER_BUF * );

#if 0
static void clear_LCD_Block    ( IMAGE_BUF *, uint8_t, uint8_t, uint8_t, uint8_t );
static void fill_raster_buffer ( IMAGE_BUF *, RASTER_BUF * );
static void clear_raster_buffer( RASTER_BUF * );
static void set_LCD_XY_Coordinates( uint8_t, uint8_t );
//static void LCD_SPI_Callback ( void );
static void init_LCD_Hardware( void );
#endif

IMAGE_BUF   buffer;  /* working buffer to build LCD image in */
RASTER_BUF  raster1; /* raster buffer for ST7579 formatted image */


/* series of commands to be issued to ST7579 LCD controller to initialize the LCD */
/* NOTE:  must write 2 bytes at a time to SPI data register so double up command/data bytes. */
static const uint16_t ST7579_InitCmds[] =                                    
{
    0x2916, /* set H[1:0] = (0,1) / Set BIAS = 5 */
    0x2B9B, /* set H[1:0] = (1,1) / Booster Efficiency Set */
    0x2804, /* set H[1:0] = (0,0) / Set V0 range */
#if 0    
	0x29DF,	/* set H[1:0] = (0,1) / Set V0 (normal setting) */
#endif
    0x29E8, /* set H[1:0] = (0,1) / Set V0 to make screen a little darker (easier to see bouncing ball) */
    0x2B0F, /* set H[1:0] = (1,1) / Frame Rate Control -- 137 Hz (max setting) */
    0x280C, /* set H[1:0] = (0,0) / Set Display Control -- normal mode */					
    0x2940, /* set H[1:0] = (0,1) / Set start line (low) -- line address 0 */
    0x0428, /* set start line (high) -- line address 0 / independent instruction -- set H[1:0] = (0,0) */
    0x4080, /* Page = 0 (y-axis) / segment = 0 (x-axis) */
};


/* pointer to sprite image to display */
extern SPRITE   *sprite;


/*******************************************************************************
* Global Variables
*******************************************************************************/
/* Reserve an area for the DTC vector table, MUST be on a 4K boundary */
#pragma address dtc_vector_table = 0x00015000
/* Declaration of the DTC vector table */
uint32_t dtc_vector_table[256];

/* Reserve 16 bytes for the ADC trigger transfer data area */
uint32_t dtc_spi0_transfer_data[4];


/*******************************************************************************
* Outline      : init_LCD_Graphics
* Description  : Routine to set up the LCD hardware and clear out the 
*                working buffer.
* Argument     : none
* Return value : none
*******************************************************************************/
void init_LCD_Graphics(void)
{
    /* send commands to LCD controller to initialize display */
    init_LCD_Hardware();

    /* clear the image buffer */
    clear_LCD_Block( &buffer, 0, 0, X_PIXELS, Y_PIXELS );
}
/*******************************************************************************
* End of function init_LCD_Graphics
*******************************************************************************/

/*******************************************************************************
* Outline      : CallbackLCD
* Description  : Routine that runs after SPI transfer completes.
*                We want this routine, even though it is empty, so that the SPI
*                transfer is interrupt-driven and not polled.
*                This lets us start calculating the next ball positions while
*                the previous ball positions are being output to the LCD.
*                This routine is called when the SPI is ready for more data,
*                which is typically BEFORE the actual transfer is complete.
* Argument     : none
* Return value : none
*******************************************************************************/
static void CallbackLCD(void)
{
}
/*******************************************************************************
* End of function LCD_SPI_Callback
*******************************************************************************/

/*******************************************************************************
* Outline      : SendToLCD
* Description  : 
*
* Argument     : 
* Return value : none
*******************************************************************************/
void SendToLCD(const void * buf, uint16_t n)
{
    bool err;
	
	n /= 2; /* convert length from bytes to 16-bit words */
	
    err = R_DTC_Control(
	                     PDL_DTC_START           /* Enable / re-enable or suspend DTC transfers */
                       | PDL_DTC_TRIGGER_SPI0_TX /* trigger selection: SPI channel 0 TX buffer empty (vector 46) */
					   | PDL_DTC_UPDATE_SOURCE   /* update the source register  */
					   | PDL_DTC_UPDATE_COUNT,   /* update the count  register  */
                       dtc_spi0_transfer_data,   /* transfer data start address */
                       buf,                      /* source   data start address */
                       PDL_NO_PTR,               /* destination   start address */
                       n,                        /* transfer count in frames    */
                       PDL_NO_DATA               /* block size */
                       );

    /* Reset SPI to trigger the DTC	*/		
    err &= R_SPI_Transfer(                           /* Transfer data over an SPI channel. */
                         0,                          /* channel selection */
                         PDL_SPI_DTC_TRIGGER_ENABLE, /* enable activation of the DTC for data transmission and reception */
                         PDL_NO_PTR,                 /* Start address of data to transmit. PDL_NO_PTR  if DTC will handle the data transfer. */
		  			     PDL_NO_PTR,                 /* Stop  address of data to receive.  PDL_NO_PTR  if DTC will handle the data transfer. */
                         PDL_NO_DATA,                /* Times to execute command sequence. PDL_NO_DATA if DTC will handle the data transfer. */
                         CallbackLCD,                /* callback function */
                         5                           /* interrupt priority level. Parameter ignored if PDL_NO_FUNC is specified for callback func. */
                         );
						 
	while ( !err );  
}

/*******************************************************************************
* Outline      : init_LCD_Hardware
* Description  : Initialization routine that uses DTC to feed LCD display
*                configuration data to the ST7579 controller via the SPI.
*                SPI SSL3 is used as the chip select to the LCD controller.
* Argument     : none
* Return value : none
*******************************************************************************/
void init_LCD_Hardware( void )
{
    bool err; 

    /* Zero out the dtc_vector_table */
	memset(dtc_vector_table, 0, sizeof(dtc_vector_table));

    /* set up the DTC */
    err = R_DTC_Set(                          /* Set the global options for the Data Transfer Controller. */
                     PDL_DTC_READ_SKIP_ENABLE /* Enable or disable skipping of transfer data read when the vector numbers match. */
				   | PDL_DTC_ADDRESS_FULL,    /* Select 32-bit (full) or 24-bit (short) address mode. */
                   dtc_vector_table           /* vector table base address, must be on a 4K boundary */
                   );

    /* Initialize the DTC to send data or commands commands to the ST7579 LCD controller */
    err &= R_DTC_Create( /* Configure the Data Transfer Controller for a transfer. */
                         PDL_DTC_NORMAL                    /* Transfer mode: normal */
					   | PDL_DTC_SOURCE_ADDRESS_PLUS       /* after a data transfer, increment     source      address */
					   | PDL_DTC_DESTINATION_ADDRESS_FIXED /* after a data transfer, do not change destination address */
					   | PDL_DTC_SIZE_16                   /* transfer data size per operation, can be 8, 16, or 32 bits */ 
					   | PDL_DTC_CHAIN_DISABLE             /* Disable chain transfer operation */
					   | PDL_DTC_IRQ_COMPLETE              /* Select interrupt request generation when the transfer sequence completes */
					   | PDL_DTC_TRIGGER_SPI0_TX,          /* trigger selection: SPI channel 0 TX buffer empty (vector 46) */
                       dtc_spi0_transfer_data,             /* transfer data start address. 16 bytes required for full address mode. */
                       PDL_NO_PTR,                         /* source        start address */
                       (uint16_t *)&RSPI0.SPDR,            /* destination   start address (SPI channel 0 data register at 0x88384) */
                       PDL_NO_DATA,                        /* transfer count in frames */
                       1                                   /* block size */
                       );  

    err &= R_SPI_Create(                             /* Configure an SPI command. */
                       0,                            /* channel selection */
                         PDL_SPI_MODE_SPI_MASTER     /* Connection type: SPI-master, SPI-multiMaster, SPI-slave, sync-master, or sync-slave */
					   | PDL_SPI_TRANSMIT_ONLY       /* enable either full-duplex or transmit-only */
					   | PDL_SPI_PIN_A               /* select A or B pins for MISO, MOSI, RSPCK, SSL0, SSL1, SSL2, & SSL3. */
					   | PDL_SPI_PIN_RSPCK_ENABLE    /* enable         signal RSPCK */
					   | PDL_SPI_PIN_MOSI_ENABLE     /* enable  output signal MOSI  */
					   | PDL_SPI_PIN_MISO_DISABLE    /* disable input  signal MISO  */
					   | PDL_SPI_PIN_SSL0_DISABLE    /* output signal SSL0: active-low, active-high, or disabled */
					   | PDL_SPI_PIN_SSL1_DISABLE    /* output signal SSL1: active-low, active-high, or disabled */
					   | PDL_SPI_PIN_SSL2_DISABLE    /* output signal SSL2: active-low, active-high, or disabled */
					   | PDL_SPI_PIN_SSL3_LOW        /* output signal SSL3: active-low, active-high, or disabled */
					   | PDL_SPI_PIN_MOSI_IDLE_LAST, /* MOSI output state when no SSLn pin is active: last, low, or high */
                         PDL_SPI_BUFFER_64           /* Select a buffer size of 64 bits (up to four 16-bit frames) or 128 bits (up to four 32-bit frames) */
					   | PDL_SPI_FRAME_1_1,          /* 1 command transfer, 1 frame per command transfer, 1 transfer frame total */
                         PDL_SPI_CLOCK_DELAY_1       /* Num of bit clock periods between assertion of SSL pin     and start of RSPCK oscillation. Ignored in Slave mode. */
					   | PDL_SPI_SSL_DELAY_1         /* Num of bit clock periods between end of RSPCK oscillation and negation of active SSL pin. Ignored in Slave mode. */
					   | PDL_SPI_NEXT_DELAY_1,       /* Num of bit clock periods (plus two cycles of peripheral clock) between end of one frame and start of next frame. Ignored in Slave mode. */
                       8E6                           /* The maximum required SPI bit rate, can also be the SPBR register value */
                       );

    err &= R_SPI_Command(                             /* Configure an SPI command. */
                        0,                            /* channel selection */
                        0,                            /* command selection */
                          PDL_SPI_CLOCK_MODE_0        /* Clock is low when idle; data is sampled on the rising edge. */
					    | PDL_SPI_DIV_1               /* Use the bit rate (specified for R_SPI_Create) ÷ 1, 2, 4 or 8. */
					    | PDL_SPI_ASSERT_SSL3         /* The SSL pin to be asserted during the frame transfer, 0, 1, 2, or 3. */
					    | PDL_SPI_SSL_KEEP            /* Negate or keep the SSL signal after the frame transfer. Ignored in Slave mode. */
					    | PDL_SPI_LENGTH_16           /* Number of bits in the frame transfer. */
					    | PDL_SPI_MSB_FIRST,          /* Select least- or most-significant bit first. */
                          PDL_SPI_CLOCK_DELAY_MINIMUM /* Select minimum or extended delay between assertion of SSL pin     and start of RSPCK oscillation. */
					    | PDL_SPI_SSL_DELAY_MINIMUM   /* Select minimum or extended delay between end of RSPCK oscillation and negation of active SSL pin. */
					    | PDL_SPI_NEXT_DELAY_MINIMUM  /* Select minimum or extended delay between end of one frame         and start of next frame. */
                        );

    /* Halt in while loop when RPDL errors detected */  
    while (!err)
    {
    }

    SET_LCD_SEND_COMMAND;       /* set A0 line on ST7579 controller to send commands and not data */
	
    SendToLCD(&ST7579_InitCmds[0], sizeof(ST7579_InitCmds)); /* Send the initialization commands to the LCD */						
}                                                            /* This transfer takes 18 bytes * 8 bits/byte / 8 MHz = 18 uS */
/*******************************************************************************
* End of function init_LCD_Hardware
*******************************************************************************/

/*******************************************************************************
* Outline      : refresh_LCD
* Description  : 
*
* Argument     : 
* Return value : none
*******************************************************************************/
void refresh_LCD( IMAGE_BUF *image, RASTER_BUF *raster )
{
    /* clear out the target raster buffer, so we only need to write '1' bits */
    clear_raster_buffer( raster );

    /* convert the image data for ST7579 display */
    fill_raster_buffer( image, raster );    
	
    /* send commands to start at x=0, y=0 */
    set_LCD_XY_Coordinates( 0, 0 );

	/* Send the data to the LCD display */
    SET_LCD_SEND_DATA; /* set A0 line on ST7579 controller to send data and not commands */
	
	SendToLCD(raster1.page, sizeof(raster1.page)); /* This transfer takes 816 bytes * 8 bits/byte / 8 MHz = 816 uS */
}

/*******************************************************************************
* End of function refresh_LCD
*******************************************************************************/


/*******************************************************************************
* Outline      : set_LCD_XY_Coordinates
* Description  : Sends commands to the ST7579 LCD controller to set the X:Y
*                coordinates. Range checks by limiting arguments to max values.
* Argument     : uint8_t	x_pixel -- location on the x-axis
*                uint8_t	y_page  -- page on the y_axis
* Return value : none
*******************************************************************************/
void set_LCD_XY_Coordinates( uint8_t x_pixel, uint8_t y_page )
{
    static uint16_t coord[] = /* Needs to be static because the DTC may still be sending the data after the function returns. */
	{ 
		0, /* set x address of RAM */
	    0, /* set y address of RAM */
	};
	
    /* Range check the input variables */
	if (x_pixel > X_PIXELS_LCD - 1)
	    x_pixel = X_PIXELS_LCD - 1;
		
	if (y_page  > Y_PAGES      - 1)
	    y_page  = Y_PAGES      - 1;
		
	coord[0]  = 0x2880;  /* Set X address of RAM */
	coord[0] |= x_pixel; 
	
	coord[1]  = 0x2840;  /* Set Y address of RAM */
	coord[1] |= y_page;

    SET_LCD_SEND_COMMAND;        /* set A0 line on ST7579 controller to send commands and not data to the LCD */
	
    SendToLCD(coord, sizeof(coord)); /* This transfer takes 4 bytes * 8 bits/byte / 8 MHz = 4 uS */
}
/*******************************************************************************
* End of function set_LCD_XY_Coordinates
*******************************************************************************/



/*******************************************************************************
* Outline      : clear_LCD_Block
* Description  : Clears an area of an image buffer.
*              	
* Argument     : IMAGE_BUF *buffer -- pointer to an image buffer
*                uint8_t x0   -- x location of top left corner of region to clear
*                uint8_t y0   -- y location of top left corner of region to clear
*                uint8_t xlen -- x number of pixels width to clear
*                uint8_t ylen -- y number of pixels height to clear
* Return value : none
*******************************************************************************/
void clear_LCD_Block( IMAGE_BUF *buffer, uint8_t x0, uint8_t y0, uint8_t xlen, uint8_t ylen )
{
    unsigned int x;
    unsigned int y;
    unsigned int xbyte;
    unsigned int bitmask_to_clear;

    /* Range checking to make sure stay inside dimension of LCD */
    if (   (x0 >= X_PIXELS_LCD) 
        || (y0 >= Y_PIXELS    ) )
        return;

    if ((x0 + xlen) >= X_PIXELS_LCD)
	{
	    xlen  = X_PIXELS_LCD - 1;
	    xlen -= x0;
	}
    if ((y0 + ylen) >= Y_PIXELS    ) 
	{
	    ylen  = Y_PIXELS - 1;
	    ylen -= y0;
	}

    for     ( y = y0; y <= (y0 + ylen); y++ )
    {
        for ( x = x0; x <= (x0 + xlen); x++ )
        {
            /* determine bit location and set to zero */
            xbyte                =            x >> 3;
            bitmask_to_clear     = 1 << (7 - (x &  7));
            buffer->b[y][xbyte] &= ~bitmask_to_clear;
        }
    }
}
/*******************************************************************************
* End of function clear_LCD_Block
*******************************************************************************/


/*******************************************************************************
* Outline      : insert_Sprite
* Description  : This function copies a sprite image into the working buffer
*                at the specified x-y coordinates
* Argument     : buffer -- pointer to the working buffer
*                x0 -- x-axis upper left corner of sprite location
*                y0 -- y-axis upper left corner of sprite location
*                sprite -- pointer to sprite image to be inserted
* Return value : none
*******************************************************************************/
void insert_Sprite( IMAGE_BUF *buffer, uint8_t x0, uint8_t y0, SPRITE* sprite )
{
    unsigned int x;
    unsigned int y;
    unsigned int xbyte;
    unsigned int bitmask_to_set;
    unsigned int x_end;
    unsigned int y_end;
    unsigned int bit_byte;
    unsigned int index;

    uint8_t x_len = sprite->x_dimension;
    uint8_t y_len = sprite->y_dimension;

    /* Range checking to make sure stay inside dimension of LCD */
    if (   (x0 >= X_PIXELS_LCD) 
	    || (y0 >= Y_PIXELS    ) )
	{
        return;
	}

    if ( (x0 + x_len) >= X_PIXELS_LCD )
    {
        x_end  = X_PIXELS_LCD - 1;     
        x0     = X_PIXELS_LCD - 1;
        x0    -= x_len;
    } 
	else
    {
        x_end = x0 + x_len;     
    }

    if ( (y0 + y_len) >= Y_PIXELS )
    {
        y_end  = Y_PIXELS - 1;
        y0     = Y_PIXELS - 1;
        y0    -= y_len;
    } 
	else
    {
        y_end = y0 + y_len; 
    }

    index = 0;

    for ( y = y0; y < y_end; y++ )
    {
        for ( x = x0; x < x_end; x++ )
        {
            /* get a bit from the image array */
            bit_byte = sprite->image[index >> 3];
            bit_byte = bit_byte & 1 << (7 - (index & 7));
            if ( bit_byte)
            {
                /* turn on bit in raster buffer	*/
                xbyte                = x >> 3;
                bitmask_to_set       = 1 << (7 - (x & 7));
                buffer->b[y][xbyte] |= bitmask_to_set;  
            }
            index++;
        }
        index = (((index - 1) >> 3) + 1) << 3; /* be sure to be on a byte boundary to start new row */
    }
}
/*******************************************************************************
* End of function insert_Sprite
*******************************************************************************/

/*******************************************************************************
* Outline      : remove_Sprite
* Description  : 
*
* Argument     : none
* Return value : none
*******************************************************************************/
void remove_Sprite( IMAGE_BUF *buffer, uint8_t x0, uint8_t y0, SPRITE* sprite )
{
    uint8_t xlen = sprite->x_dimension;
    uint8_t ylen = sprite->y_dimension;

    clear_LCD_Block( buffer, x0, y0, xlen, ylen );
}
/*******************************************************************************
* End of function remove_Sprite
*******************************************************************************/


/*******************************************************************************
* Outline      : clear_raster_buffer
* Description  : zeros out the raster buffer pointed to in 1st argument
* Argument     : raster -- pointer to a RASTER_BUF instance.
* Return value : none
*******************************************************************************/
void clear_raster_buffer( RASTER_BUF *raster )
{
    memset(raster, 0, sizeof(raster->page));
}
/*******************************************************************************
* End of function clear_raster_buffer
*******************************************************************************/



/*******************************************************************************
* Outline      : fill_raster_buffer
* Description  : This function takes an image buffer and converts into a format
* 				 that is displayable on the debug LCD.
* Argument     : buffer -- pointer to IMAGE_BUF instance containing data to be 
*						   displayed.
*				 raster -- pointer to RASTER_BUF instance to contain the 
*						   converted data.	
* Return value : none
*******************************************************************************/
void fill_raster_buffer( IMAGE_BUF *buffer, RASTER_BUF *raster )
{
    unsigned int x;
	unsigned int y;
	unsigned int i;
	unsigned int j;
	unsigned int bit_mask;
    unsigned int num_pixels;

    /* need to invert the raster buffer for writing to the h/w */
    /* very confusing but basically, h/w writes are on pages where byte0 is */
    /* bit 0 of first 8 rows of pixels.  byte1 is 2nd pixels of same rows. */
    /* byte8 is bit0 of 2nd 8 rows. */

    /* to make this work efficiently with DTC and SPI in 16-bit mode, need to put */
    /* data into each halfword of the raster buffer */

    uint8_t * raster_ptr = &raster->page[0][0];

    for ( y = 0; y < Y_PIXELS; y = y + 8 )
    {
        num_pixels = 0;     /* LCD is not multiple of 8 bits wide, */
                            /*  so we need to keep count and stop at end of page */
        for ( x = 0; x < (X_PIXELS + 7) / 8; x++ )
        {
            for ( i = 0; i < 8; i++ )   /* loop thru all 8 bits */
            {
                if ( num_pixels == X_PIXELS )
                    break;

                bit_mask = 0x80 >> i;
                for ( j = 0; j < 8; j++ )
                {
                    if ( buffer->b[y + j][x] & bit_mask)
                    {
                        /* swap the bytes as we go for streaming out SPI */
                        if ( (num_pixels & 0x01) == 0 )
                            *(raster_ptr + 1) |= 1 << j;
                        else
                            *(raster_ptr - 1) |= 1 << j;
					}
                }

                raster_ptr++;
                num_pixels++;   
            }
        }
    }
}
/*******************************************************************************
* End of function fill_raster_buffer
*******************************************************************************/


