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
* File Name     : main_adc_oneshot.c
* Version       : 1.1
* Device(s)     : R5F562N8
* Tool-Chain    : Renesas RX Standard Toolchain 1.0.1
* OS            : None
* H/W Platform  : YRDKRX62N
* Description   : This sample demonstrates usage of the ADC unit, in oneshot
*                 mode. The program takes a sample of the voltage created by the
*                 potentiometer VR1.
*
* Operation     : 1. Build and download the sample code to the YRDKRX62N
*
*                 2. Click 'Reset Go' to start the software.
*                 
*                 3. Instructions will be displayed on the debug LCD. Adjust the
*                    potentiometer VR1, and press switch SW3 to trigger an AD
*                    conversion.
*                    
*                 4. The ADC result will be displayed on the second line of the
*                    debug LCD in the format =H'0000.
*                
*                 5. Re-adjust VR1, and press SW3 again to perform another 
*                    conversion.
*                    
*                 6. The value of the AD conversion can be watched in the
*                    variable usADC_Result. Right click the variable, and select
*                    'Instant Watch' and click yes to the following dialogs.
*******************************************************************************
* History : DD.MMM.YYYY     Version     Description
*         : 08.Oct.2010     1.00        First release
*         : 29.Nov.2010     1.10        Updated to use new GlyphLib_v2
*******************************************************************************/

/*******************************************************************************
* Project Includes
*******************************************************************************/
#include <stdbool.h>

/* Defines debug LCD functions used in this file */
#include "lcd.h"

/* Defines oneshot ADC functions used in this file */
#include "ADCone.h"

/* RSPI API library support */
#include "YRDKRX62N_RSPI_API.h"

/* Board specific definitions */
#include "YRDKRX62N.h"

/*******************************************************************************
* Global Variables
*******************************************************************************/
/* ADC result global storage variable */
extern uint16_t usADC_Result;

/*******************************************************************************
* Outline       : main
* Description   : The main program function. The function first initialises the
*                 debug LCD, then displays name of the sample and operation 
*                 instructions. Main then calls the ADC initialisation function,
*                 Init_ADCOneshot, which configures the ADC. After this, the
*                 MCU returns to main, where it enters an infinite while loop.
* Argument      : none
* Return value  : none
*******************************************************************************/
void main(void)
{
    /* Initialise the LCD on the RSPI bus */
    YRDKRX62N_RSPI_Init(RSPI_CHANNEL_0);
    InitialiseLCD();

    /* Display instructions onto the LCD */
    DisplayLCD(LCD_LINE1, "ADC One Shot ");
    DisplayLCD(LCD_LINE3, "Adjust pot");
    DisplayLCD(LCD_LINE4, "Push SW3");

    /* Initialise the ADC unit */
    Init_ADCOneshot();

    /* Infinite while loop */
    while (1);

}
/*******************************************************************************
* End of function main
*******************************************************************************/
