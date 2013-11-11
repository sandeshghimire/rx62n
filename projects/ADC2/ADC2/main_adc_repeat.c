/******************************************************************************
* DISCLAIMER

* This software is supplied by Renesas Electronics Corp. and is
* only intended for use with Renesas products.  

* No other uses are authorized.

* This software is owned by Renesas Electronics Corp. and is 
* protected under the applicable laws, including copyright laws.

* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES
* REGARDING THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY,
* INCLUDING BUT NOT LIMITED TO WAWRRANTIES OF MERCHANTABILITY, 
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
* File Name     : main_adc_repeat.c
* Version       : 1.1
* Device(s)     : R5F562N8
* Tool-Chain    : Renesas RX Standard Toolchain 1.0.1
* OS            : None
* H/W Platform  : YRDKRX62N
* Description   : This sample demonstrates use of the ADC unit, by periodically
*                 sampling the voltage from the potentiometer VR1, and displaying
*                 the result on the debug LCD.
* Operation     : 1. Build and download the sample code to the YRDK.
*
*                 2. Click 'Reset Go' to start the software.
*         
*                 3. The debug LCD will show the name of the sample, and the
*                    current ADC value.
*          
*                 4. Adjust the potentiometer, VR1, and observe the value
*                    displayed on the LCD change.
*
*                 5. The user may examine the AD conversion result in the global
*                    array usADC_Result[ ].
* Limitations   : None
*******************************************************************************
* History : DD.MMM.YYYY     Description
*         : 08.Oct.2010     First release
*         : 02.Dec.2010     Second YRDK release
*******************************************************************************/

/*******************************************************************************
* Project Includes
*******************************************************************************/
#include <stdint.h>
#include <stdbool.h>
/* Defines debug LCD functions used in this file */
#include "lcd.h"
/* Defines ADC repeat functions used in this file */
#include "ADCrepeat.h"
/* RSPI API library support */
#include "YRDKRX62N_RSPI_API.h"
/* Board specific definitions*/
#include "YRDKRX62N.h"

/*******************************************************************************
* Outline       : main
* Description   : The main program function. The function first initialises the 
*                 debug LCD, then displays the name of the sample on the display.
*                 The function then executes the initialisation function, Init_
*                 ADCRepeat, which configures the ADC unit. The MCU then returns
*                 to the main function, where it enters an infinite while loop.
* Argument      : none
* Return value  : none
*******************************************************************************/
void main(void)
{
    /* Initialize RSPI peripheral */
    YRDKRX62N_RSPI_Init(RSPI_CHANNEL_0);

    /* Initialise the debug LCD */
    InitialiseLCD();

    /* Display instructions onto the LCD */
    DisplayLCD(LCD_LINE1, "ADC Repeat");
    DisplayLCD(LCD_LINE3, "Adjust pot");


    /* Initialise ADC unit for repeat operation */
    Init_ADCRepeat();

    /* Infinite while loop */
    while (1);

}
/*******************************************************************************
* End of function main
*******************************************************************************/

