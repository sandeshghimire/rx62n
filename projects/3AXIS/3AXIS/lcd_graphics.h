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
* File Name     : lcd_graphics.h
* Version       : 1.1
* Device(s)     : R5F562N8
* Tool-Chain    : Renesas RX Standard Toolchain 1.0.1
* OS            : None
* H/W Platform  : YRDKRX62N
* Description   : Provides delarations for the debug LCD
* Limitations   : None
*******************************************************************************
* History : DD.MMM.YYYY   Version    Description
*         : 08.Oct.2010   1.00       First release
*         : 02.Dec.2010   1.10       Second YRDK release
*******************************************************************************/

/*******************************************************************************
* Project Includes
*******************************************************************************/
#include <stdint.h>
#include <stdbool.h>

/*******************************************************************************
* Macro Definitions
*******************************************************************************/
/* Multiple inclusion prevention macro */
#ifndef LCD_GRAPHICS_H
#define LCD_GRAPHICS_H


#define SET_LCD_SEND_COMMAND	PORT5.DR.BIT.B1 = 0; // clear ST7579 A0 line to send a command 
#define SET_LCD_SEND_DATA		PORT5.DR.BIT.B1 = 1; // set ST7579 A0 line to send data

#define ASSERT_LCD_CS			PORTC.DR.BIT.B2 = 0;
#define DEASSERT_LCD_CS			PORTC.DR.BIT.B2 = 1;

//  ST7579 LCD Controller definitions 102x64 buffer size
#define Y_PAGES			 	 8
#define Y_PIXELS			64
#define X_PIXELS		   102

// LCD X pixels are less than what the controller supports
//    96x64 pixels
#define X_PIXELS_LCD		96
#define Y_PIXELS_LCD		64


// LCD refresh rate (per second)
#define LCD_REFRESH_RATE	80


#define TOTAL_SEGMENTS (Y_PAGES*X_PIXELS)

typedef struct
{
    uint8_t     b[Y_PIXELS][((X_PIXELS+7)/8)];
} IMAGE_BUF;    


// structure to contain converted data ready to send via DTC/SPI channel
typedef struct
{
    uint8_t page[Y_PAGES][X_PIXELS];    
} RASTER_BUF; 

typedef struct
{
    uint8_t     x_dimension;        // x-axis measurement in pixels
    uint8_t     y_dimension;        // y-axis measurement in pixels
    uint8_t    *image;              // array of image data
} SPRITE;   

/******************************************************************************
* Global Function Prototypes
******************************************************************************/
void init_LCD_Graphics(void) ;
void refresh_LCD( IMAGE_BUF *image, RASTER_BUF *raster ) ;
void remove_Sprite( IMAGE_BUF *buffer, uint8_t x0, uint8_t y0, SPRITE* sprite ) ;
void insert_Sprite( IMAGE_BUF *buffer, uint8_t x0, uint8_t y0, SPRITE* sprite ) ;

/* End of multiple inclusion prevention macro */
#endif
