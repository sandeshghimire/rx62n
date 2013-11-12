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
* File Name     : sprite_images.c
* Version       : 1.1
* Device(s)     : R5F562N8
* Tool-Chain    : Renesas RX Standard Toolchain 1.0.1
* OS            : None
* H/W Platform  : YRDKRX62N
* Description   : Board specific definitions
* Limitations   : None
*******************************************************************************
* History : DD.MMM.YYYY   Version    Description
*         : 08.Oct.2010   1.00       First release
*         : 02.Dec.2010   1.10       Second YRDK release
*******************************************************************************/

/*******************************************************************************
* Project Includes
*******************************************************************************/    

/* LCD Graphics definitions */
#include "lcd_graphics.h"


// Sample sprite -- rectangle
uint8_t     Rectangle_image[] = { 0xFF, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0xFF};
SPRITE      Rectangle = { 8, 8, Rectangle_image};

// Sample sprite -- triangle
uint8_t     Triangle_image[] = 
  { 0x00, // pixel row 0
    0x18, // pixel row 1
    0x24, 
    0x42, 
    0x81, 
    0xFF };
SPRITE      Triangle = { 8, 6, Triangle_image};

// Sample sprite -- a 15x10 bit large square, requires 24 bytes of data
uint8_t     Square_LG_image[] = 
  { 0xFF, 0xFE, 
    0x80, 0x02,
    0x80, 0x02,
    0x80, 0x02,
    0x80, 0x02,
    0x80, 0x02,
    0x80, 0x02,
    0x80, 0x02, 
    0x80, 0x02,
    0xFF, 0xFE };
SPRITE      Square_LG = { 15, 10, Square_LG_image};

// Sample sprite -- a 15x10 bit circle, requires 24 bytes of data
uint8_t     Circle_image[] =    
  { 0x0F, 0xC0, 
    0x30, 0x30,
    0x60, 0x18,
    0xC0, 0x0C,
    0xC0, 0x0C,
    0xC0, 0x0C,                                 
    0xC0, 0x0C,
    0x60, 0x18,
    0x30, 0x30, 
    0x0F, 0xC0 };

SPRITE      Circle = { 15, 10, Circle_image};


// Sample sprite -- a 14x10 bit filled circle, requires 24 bytes of data
uint8_t     FilledCircle_image[] =  
{   0x0F, 0xC0,     //0
    0x3D, 0xF0,     //1
    0x7E, 0xF8,     //2
    0xFF, 0x7C,     //3
    0xDF, 0x7C,     //4
    0xE7, 0x74,     //5			  
    0xF8, 0x0C,     //6
    0x7E, 0xF8,     //7
    0x3D, 0xF0,     //8
    0x0F, 0xC0 };   //9

SPRITE      FilledCircle = { 14, 10, FilledCircle_image};


// Sample sprite -- a 8x6 bit filled circle, requires 24 bytes of data
uint8_t     SmallCircle_image[] =   
{   0x3C,       //0
    0x7E,       //1
    0xFF,       //2
    0xFF,       //3 
    0x7E,       //4
    0x3C };     //5


SPRITE      SmallCircle = { 8, 6, SmallCircle_image};


// configure what sprite to display on the LCD
SPRITE*     sprite = &SmallCircle;   

