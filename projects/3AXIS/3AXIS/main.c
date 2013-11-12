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
* File Name     : main.c
* Version       : 1.1
* Device(s)     : R5F562N8
* Tool-Chain    : Renesas RX Standard Toolchain 1.0.1
* OS            : None
* H/W Platform  : YRDKRX62N
* Description   : Demonstrates hardware FPU versus software floating point performance.
*
* Operation	   : 1. Build and download the sample code to the RDK+.
* 
*				 2. Click 'Reset Go' to start the software. Two small bouncing 
*				    balls will appear on the LCD.  The top ball is being moved
*                   using the FPU. The lower ball position is calculated
*                   with software.
*
*                3. Experiment with the "fpuBall" and "theBall" x & y velocity 
*                   settings and see if you can get the balls to move in sync. 
*                   Slowing the fpuBall down to about quarter speed will get it
*                   into the general ballpark.  (Parameters 2 and 3 of ballInit
*                   and fpu_ballInit contain the x and y velocity variables.)
*
* Limitations   : None
*******************************************************************************
* History : DD.MMM.YYYY   Version    Description
*         : 08.Oct.2010   1.00       First release
*         : 02.Dec.2010   1.10       Second YRDK release
*******************************************************************************/

/*******************************************************************************
* Project Includes
*******************************************************************************/    
/* RPDL definitions */
#include "r_pdl_mtu2.h"
#include "r_pdl_spi.h"
#include "r_pdl_dtc.h"
#include "r_pdl_definitions.h"

#include "lcd_graphics.h"
#include "Ball_FPU.h"

/* External references */
extern SPRITE       SmallCircle;
extern RASTER_BUF   raster1;

/* Local functions */
static void Init_MTU_Timer (void);
static void Start_MTU_Timer(void);
static void Stop_MTU_Timer (void);
static void CB_MTU_Timer   (void);

/******************************************************************************
 Private (static) variables and constants
******************************************************************************/
BALL theBall ;
BALL fpuBall ;

unsigned long int fpuOps ;        /* Number of HW floating point ops/interval */
unsigned long int swOps ;         /* Number of SW floating point ops/interval */
unsigned long int totalFpuOps ;   /* Total number of HW floating point ops over many loops */
unsigned long int totalSwOps ;    /* Total number of SW floating point ops over many loops */

int   iterations = 0;
float speedup ;
float totalSpeedup ;

/* Allocate a copy of the structure for the selected channel */
R_MTU2_Create_structure          ch2_create_parms;
R_MTU2_ControlChannel_structure  ch2_control_parms;

IMAGE_BUF   image_buffer;


bool timerStopped = true;

/*******************************************************************************
* Outline      : main
* Description  : The main program function. This function first intializes the
*				 debug LCD, and displays the sample name. It then calls the 
*				 Init_IIC_Master to initialize the I2C bus and next calls
*                Init_Accelerometer, from which is does not exit from.
* Argument     : none
* Return value : none
*******************************************************************************/
void main(void)
{
    SPRITE * ball_sprite = &SmallCircle; 
		
    /* Set intial location & velocity of two balls for demo */
    fpu_ballInit (&fpuBall, 0.0, 0.0, 0.00030, 0.00030) ;
    ballInit     (&theBall, 0.0, 0.0, 0.00030, 0.00030) ;

    /* Initialize the timer */
    Init_MTU_Timer();

    /* initialize LCD display */
    init_LCD_Graphics();

    /*
     Main loop:
    
    In this loop, the locations of two balls are updated.  One ball is moved using	
    the RX hardware Floating Point Unit (FPU), while the other ball is moved using
    a software floating point emulation.  Each ball is given the same amount of time
    to complete as many calculations as possible; the number of calculations per time
    slice is recorded for each.  The time slice is 3 ms.
    
    To compare performance between using native hardware FPU and the software emulation,	
    observe the speed difference in the balls on the display - the faster ball is the 
    one moved using the hardware FPU.  Also, set a breakpoint on one of the two
    calls to LedBitOn() near the bottom of the loop, add "fpuOps" and "swOps" to the 
    watch window (set the radix to decimal) and run the program.  After the breakpoint,	
    compares the values in the watch window.  Keep hitting "Go" to see them update.
    */  
    while (1)
    {
        /* Erase the balls at their current locations */
        remove_Sprite( &image_buffer, (uint8_t)fpuBall.x, (uint8_t)fpuBall.y, ball_sprite );
        remove_Sprite( &image_buffer, (uint8_t)theBall.x, (uint8_t)theBall.y, ball_sprite );

        /* Move the ball using SW floating point emulation */
        swOps = 0 ;                                 /* Clear # of SW moves */
        Start_MTU_Timer() ;                         /* Start the 3 ms timer */
        while ( timerStopped == false )             /* Run for 3 ms */
        {
            ballUpdatePosition (&theBall) ;         /* Move the ball */
            swOps++ ;                               /* Count # of moves */
        }

        /* Move the ball using hardware FPU */ 
        fpuOps = 0 ;                                /* Clear # of HW FPU moves */
        Start_MTU_Timer() ;                         /* Start the 3 ms timer */
        while ( timerStopped == false )             /* Run for 3 ms */
        {
            fpu_ballUpdatePosition (&fpuBall) ;     /* Move the ball */
            fpuOps++ ;                              /* Count # of moves */
        }

        /* Update the balls in the screen buffer */
        insert_Sprite( &image_buffer, (uint8_t)fpuBall.x, (uint8_t)fpuBall.y, ball_sprite );
        insert_Sprite( &image_buffer, (uint8_t)theBall.x, (uint8_t)theBall.y, ball_sprite );

        /* update the LCD */
        refresh_LCD( &image_buffer, &raster1 ); 

        speedup = (float) (fpuOps) / (float) (swOps) ;

        /* Keep running totals for long-term averaging */
        totalSwOps  = totalSwOps  + swOps ;
        totalFpuOps = totalFpuOps + fpuOps ;

        /* Long term averaging to mitigate the effects of debugger */
        iterations++ ;
        if (iterations == 1000)
        {
            totalSpeedup = (float) (totalFpuOps) / (float) (totalSwOps) ;
            iterations   = 0 ;
            totalFpuOps  = 0 ;
			totalSwOps   = 0 ;
        }
    }
}
/*******************************************************************************
* End of function main
*******************************************************************************/

/*******************************************************************************
* Outline      : Init_MTU_Timer
* Description  : This function initializes an MTU to generate a 3 millisecond
*				 timer pop.
*
*	The math that determined the TGRB value:
*		PCLK = 48,000,000 ticks per second or 48,000 ticks per mSec or 144,000 for 3 mSecs
*			   144,000 / 1024 (PCLK divider) = 140.6 
*
* Argument     : none
* Return value : none
*******************************************************************************/
static void Init_MTU_Timer(void)
{
    /* Declare error flag */
    bool err;

    /* Configure the MTU pins */
    err = R_MTU2_Set
          (
          PDL_MTU2_PIN_CLKABCD_B /* configuration: Select the -B pins for MTCLKA, MTCLKB, MTCLKD and MTCLKD. */
          );  

    /* Load the defaults */
    R_MTU2_Create_load_defaults( &ch2_create_parms ); /* This library function loads the default values into the structure. */

    /* Set non-default options for channel 1 */
    ch2_create_parms.data2  =   PDL_MTU2_MODE_NORMAL       
                              | PDL_MTU2_SYNC_DISABLE
							  | PDL_MTU2_TGRB_DTC_TRIGGER_DISABLE;
								  
    ch2_create_parms.data3  =   PDL_MTU2_CLK_PCLK_DIV_1024 
	                          | PDL_MTU2_CLK_RISING
	                          | PDL_MTU2_CLEAR_TGRB;
								  
    ch2_create_parms.data11 = 140;          /* TGRB */
    ch2_create_parms.func2  = CB_MTU_Timer; /* TGRB event func */
    ch2_create_parms.data19 = 5;            /* interrupt priority */

    err &= R_MTU2_Create
           (
           2,                /* channel selection */
           &ch2_create_parms /* pointer to the structure */
           );  

    /* Start MTU channel 2 */
    ch2_control_parms.data2 = PDL_MTU2_START;

    /* Modify the operation of channel 2 */
    err &= R_MTU2_ControlChannel
           (
           2,                 /* channel selection */
           &ch2_control_parms /* pointer to the structure */
           );              

    /* Halt in while loop when RPDL errors detected */  
    while (!err)
    {
    }
}

/*******************************************************************************
* End of function Init_MTU_Timer
*******************************************************************************/


/*******************************************************************************
* Outline      : CB_MTU_Timer
* Description  : This callback functions is executed every "TGRB match"
*				 interrupt. The function toggles the output state of LED4 
*                every 500 calls (1/2 second interval).
* Argument     : none
* Return value : none
*******************************************************************************/
static void CB_MTU_Timer(void)
{
    Stop_MTU_Timer();
}

/*******************************************************************************
* End of function CB_MTU_Timer
*******************************************************************************/


/*******************************************************************************
* Outline      : Start_MTU_Timer
* Description  : This function initializes an MTU to generate a 3 millisecond
*				 timer pop.
* Argument     : none
* Return value : none
*******************************************************************************/
static void Start_MTU_Timer(void)
{
    /* Declare error flag */
    bool err;

    /* Start MTU channel 2 */
    ch2_control_parms.data2 = PDL_MTU2_START;

    /* Modify the operation of channel 2 */
    err = R_MTU2_ControlChannel
          (
          2,                 /* channel selection */
          &ch2_control_parms /* pointer to the structure */
          );              

    /* Halt in while loop when RPDL errors detected */  
    while (!err)
    {
    }

    timerStopped = false;
}

/*******************************************************************************
* End of function Stop_MTU_Timer
*******************************************************************************/

/*******************************************************************************
* Outline      : Stop_MTU_Timer
* Description  : This function initializes an MTU to generate a 3 millisecond
*				 timer pop.
* Argument     : none
* Return value : none
*******************************************************************************/
static void Stop_MTU_Timer(void)
{
    /* Declare error flag */
    bool err;

    /* Start MTU channel 2 */
    ch2_control_parms.data2 = PDL_MTU2_STOP; 
    ch2_create_parms.data11 = 140;           /* TGRB */

    /* Modify the operation of channel 2 */
    err = R_MTU2_ControlChannel
          (
          2,                 /* channel selection */
          &ch2_control_parms /* pointer to the structure */
          );              

    /* Halt in while loop when RPDL errors detected */  
    while (!err)
    {
    }

    timerStopped = true;
}

/*******************************************************************************
* End of function Stop_MTU_Timer
*******************************************************************************/


    
