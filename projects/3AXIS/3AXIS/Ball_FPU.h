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
* File Name     : Ball_FPU.h
* Version       : 1.1
* Device(s)     : R5F562N8
* Tool-Chain    : Renesas RX Standard Toolchain 1.0.1
* OS            : None
* H/W Platform  : YRDKRX62N
* Description   : Header file for ball.c and FPU_ball.c
* Limitations   : None
*******************************************************************************
* History : DD.MMM.YYYY   Version    Description
*         : 08.Oct.2010   1.00       First release
*         : 02.Dec.2010   1.10       Second YRDK release
*******************************************************************************/
#ifndef _BALL_FPU_H_
#define _BALL_FPU_H_

#include "lcd_graphics.h"

/* Screen boundaries for SW (bottom) ball */
#define SW_LEFT_LIMIT 		0
#define SW_RIGHT_LIMIT 		(X_PIXELS_LCD-8)
#define SW_TOP_LIMIT 	   ((Y_PIXELS_LCD)/2)
#define SW_BOTTOM_LIMIT 	(Y_PIXELS_LCD-6)

/* Screen boundaries for FPU (top) ball */
#define LEFT_LIMIT 			0
#define RIGHT_LIMIT 		(X_PIXELS_LCD-8)
#define TOP_LIMIT 			0
#define BOTTOM_LIMIT 	   ((Y_PIXELS_LCD-6)/2)

/* Type definitions */
typedef struct
{
    float x;      /* Ball X location */
    float y;      /* Ball Y location */
    float xv;     /* Ball X velocity */
    float yv;     /* Ball Y velocity */
} BALL ;

/* Public functions */
void ballUpdatePosition    (BALL *ball) ;
void ballInit              (BALL *ball, float x, float y, float xv, float yv) ;
void fpu_ballUpdatePosition(BALL *ball) ;
void fpu_ballInit          (BALL *ball, float x, float y, float xv, float yv) ;

#endif
