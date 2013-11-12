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
* File Name     : Ball.c
* Version       : 1.1
* Device(s)     : R5F562N8
* Tool-Chain    : Renesas RX Standard Toolchain 1.0.1
* OS            : None
* H/W Platform  : YRDKRX62N
* Description   : Compile this file with the "-nofpu" switch to disable FPU
* Limitations   : None
*******************************************************************************
* History : DD.MMM.YYYY   Version    Description
*         : 08.Oct.2010   1.00       First release
*         : 02.Dec.2010   1.10       Second YRDK release
*******************************************************************************/

/******************************************************************************
 Include files
******************************************************************************/
#include "Ball_FPU.h"

/******************************************************************************
 Macro definitions
******************************************************************************/

/******************************************************************************
 Type defintions
******************************************************************************/

/******************************************************************************
 External (imported) data and functions (from other files) [list vars then fns]
******************************************************************************/

/******************************************************************************
 Exported global variables & functions (to be used by other modules)
******************************************************************************/

/******************************************************************************
 Private (static) variables and functions
******************************************************************************/

/******************************************************************************
* Function name : ballUpdatePosition
* Description   : Updates the balls position
* Arguments     : BALL *ball - pointer to ball to move
* Return Value  : none - the ball's position is updated
******************************************************************************/
void ballUpdatePosition (BALL *ball)
{
    ball->y += ball->yv ; 
    ball->x += ball->xv ;

    /* Check for ceiling & floor bounces */
    if (ball->y  < SW_TOP_LIMIT)
    {
        ball->yv = -ball->yv ;
        ball->y  = SW_TOP_LIMIT ;
    } else if (ball->y > SW_BOTTOM_LIMIT)
    {
        ball->yv = -ball->yv ;
        ball->y  = SW_BOTTOM_LIMIT ;
    }

    if (   (ball->x > SW_RIGHT_LIMIT) 
	    || (ball->x < SW_LEFT_LIMIT))
    {
        ball->xv = -ball->xv ; 
        ball->x +=  ball->xv ;
    }
}

/******************************************************************************
* Function name : ballInit
* Description   : Set the initial X, Y, X velocity and Y velocity of a ball
* Arguments     : BALL *ball - pointer to ball to be initialized
*                 float x - initial X position
*                 float y - initial Y position
*                 float xv - initial X velocity
*                 float yv - initial Y velocity
* Return Value  : none - the ball is initialized
******************************************************************************/
void ballInit (BALL *ball, float x, float y, float xv, float yv)
{
    ball->x  = x;
    ball->y  = y;
    ball->xv = xv;
    ball->yv = yv;
}
