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
* File Name     : main_wdt.c
* Version       : 1.1
* Device(s)     : R5F562N8
* Tool-Chain    : Renesas RX Standard Toolchain 1.0.1
* OS            : None
* H/W Platform  : YRDKRX62N
* Description   : This sample demonstrates usage of the watchdog timer, by 
*                 configuring the timer to require periodic resets to prevent
*                 a timer overflow. The sample configures a periodic timer reset
*                 which has a period that varies with the position of the
*                 potentiometer, VR1. The LEDs flash to indicate each period.
*
* Operation     : 1. Build and download the sample code to the YRDK.
*
*                 2. Ensure the potentiometer, VR1, is turned to the fully counter-
*                    clockwise position before starting the sample. To start the
*                    sample, click 'Reset Go'.
*                    
*                 3. The debug LCD will display the sample name, and the user
*                    LEDs will begin to flash.
*                    
*                 4. Slowly turn the potentiometer VR1 clockwise, and observe the
*                    gradual decrease the LED flash rate. Each time the LEDs 
*                    toggle output state, the watchdog timer is reset, preventing
*                    it from triggering an overflow interrupt.
*                    
*                 5. Continue to turn the potentiometer clockwise - the LED
*                    flashing will slow down to a point where the watchdog timer
*                    is allowed to overflow.
*                    
*                 6. When the timer has overflowed, the LEDs will stay constantly
*                    lit, and the program will wait in an infinite while loop. 
*                    The debug LCD will display "Watchdog Overflow".
*                    
*                 7. To repeat the sample, ensure RV1 is fully counter-clockwise
*                    and click 'Reset Go'.
*******************************************************************************
* History : DD.MMM.YYYY     Version     Description
*         : 08.Oct.2010     1.00        First release
*         : 02.Dec.2010     1.10        Second YRDK release
*******************************************************************************/

/*******************************************************************************
* Project Includes
*******************************************************************************/
#include <stdbool.h>
/* Defines debug LCD functions used in this file */
#include "lcd.h"
/* defines watchdog timer functions used in this file */
#include "wdt.h"
/* RSPI API library support */
#include "YRDKRX62N_RSPI_API.h"
/* Board specific definitions */
#include "YRDKRX62N.h"

/*******************************************************************************
* Outline      : main
* Description  : Main program function. This function first initialises the 
*                debug LCD, and displays the name of the sample. The function
*                then calls the Init_WDT function, which configures the watchdog
*                timer, and the LED flash reset interrupts. After the
*                initialisation function has been called, main waits in an
*                infinite while loop.
* Argument     : none
* Return value : none
*******************************************************************************/
void main(void)
{
    /* Initialise the LCD display on the RSPI bus */
    YRDKRX62N_RSPI_Init(RSPI_CHANNEL_0);
    InitialiseLCD();
    
    /* Display the sample name on the debug LCD */
    DisplayLCD(LCD_LINE1, "  WDT DEMO ");
    DisplayLCD(LCD_LINE3, "Turn pot CW") ;
    DisplayLCD(LCD_LINE4, "to decrease") ;
    DisplayLCD(LCD_LINE5, "refresh rate") ;

    /* Configure the watchdog timer and timer reset interrupts */
    Init_WDT();

    /* Infinite while loop */
    while(1);
}
/*******************************************************************************
* End of function main
*******************************************************************************/
