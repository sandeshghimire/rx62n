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
* File Name     : DCrepeat.c
* Version       : 1.1
* Device(s)     : R5F562N8
* Tool-Chain    : Renesas RX Standard Toolchain 1.0.1
* OS            : None
* H/W Platform  : YRDKRX62N
* Description   : Defines ADC repeat functions used in this sample.
* Limitations   : None
*******************************************************************************
* History : DD.MMM.YYYY     Description
*         : 08.Oct.2010     First release
*         : 02.Dec.2010     Second YRDK release
*******************************************************************************/

/*******************************************************************************
* Project Includes
*******************************************************************************/
/* ADC RPDL function definitions */
#include "r_pdl_adc_12.h"
/* CMT RPDL function definitions */
#include "r_pdl_cmt.h"
/* General RPDL function definitions */
#include "r_pdl_definitions.h"
/* Defines switch functions and variables used in this file */
#include "switch.h"
/* Defines LCD functions used in this file */
#include "lcd.h"
/* Provides declarations of functions defined in this file */
#include "ADCrepeat.h"

/*******************************************************************************
* Global Variables
*******************************************************************************/
/* Global ADC result container variable */
uint16_t usADC_Result[8] ;

/* Global switch flag variable, indicates which switches have been pressed */
extern volatile uint8_t gSwitchFlag;

/*******************************************************************************
* Local Function Prototypes
*******************************************************************************/
/* CMT interrupt callback function prototype declaration */
void CB_CMTADC(void);
/* 16 bit integer to character string function prototype declaration */
void uint16_ToString(uint8_t *, uint8_t, uint16_t);

/*******************************************************************************
* Outline       : Init_ADCRepeat
* Description   : ADC initialisation function. This function configures the ADC
*                 unit for repeat operation, then configures a CMT timer for a
*                 periodic interrupt every 100ms.
* Argument      : none
* Return value  : none
*******************************************************************************/
void Init_ADCRepeat(void)
{
    /* Declare error flag */
    bool err = true;

    /* Configure the ADC unit */
    err &=  R_ADC_12_Create
            (
            0,
            PDL_ADC_12_SCAN_CONTINUOUS | PDL_ADC_12_CHANNEL_4 | PDL_ADC_12_DATA_ALIGNMENT_RIGHT | PDL_ADC_12_DIV_8,
            PDL_ADC_12_TRIGGER_SOFTWARE,
            PDL_NO_DATA,
            PDL_NO_FUNC,
            0
            );

    /* Configure CMT timer to execute callback function every 100ms */
    err &=  R_CMT_Create
            (
            2,
            PDL_CMT_PERIOD,
            100E-3,
            CB_CMTADC,
            3
            );

    /* Start ADC repeat conversions */
    err &=  R_ADC_12_Control
            (
            PDL_ADC_12_0_ON
            );

    /* Halt in while loop when RPDL errors detected */  
    while (!err);
}
/*******************************************************************************
* End of function Init_ADCRepeat
*******************************************************************************/

/*******************************************************************************
* Outline       : CB_CMTADC
* Description   : CMT interrupt callback function. This callback function is
*                 executed after every period of the CMT timer. The function 
*                 fetches the ADC result, and stores it in the global variable
*                 usADC_Result. The function then converts the ADC result into
*                 a character string, and displays it on the debug LCD.
* Argument      : none
* Return value  : none
*******************************************************************************/
void CB_CMTADC(void)
{
    /* Declare error flag */
    bool err = true;

    /* Declare temporary character string */
    uint8_t lcd_buffer[] = "=H\'WXYZ\0";

    /* Poll ADC unit until conversion is complete and store result in the
       global variable usADC_Result */  
    err &=  R_ADC_12_Read
            (
            0,
            usADC_Result
            );

    /* Convert ADC result into a character string, and store in the local
       string lcd_buffer */    
    uint16_ToString(lcd_buffer, 3, usADC_Result[4]);

    /* Display the contents of the local string lcd_buffer */
    DisplayLCD(LCD_LINE5, lcd_buffer);  

    /* Halt in while loop when RPDL errors detected */  
    while (!err);
}
/*******************************************************************************
* End of function CB_CMTADC
*******************************************************************************/

/*******************************************************************************
* Outline       : uint16_ToString
* Description   : Function converts a 16 bit integer into a character string, 
*                 inserts it into the array via the pointer passed at execution.
* Argument      : * output_string : Pointer to uint8_t array that will hold 
*                                   character string.
*                             pos : uint8_t number, element number to begin 
*                                   inserting the character string from (offset).
*                    input_number : 16 bit integer to convert into a string.
* Return value  : none
* Note          : No input validation is used, so output data can overflow the
*                 array passed.
*******************************************************************************/
void uint16_ToString(uint8_t * output_string, uint8_t pos, 
                     uint16_t input_number)
{
    /* Declare temporary charcter storage variable, and bit_shift variable */  
    uint8_t a = 0x00, bit_shift = 12u;

    /* Declare 16bit mask variable */
    uint16_t mask = 0xF000;

    /* Loop through until each hex digit is converted to an ASCII character */
    while (bit_shift < 30u)
    {
        /* Mask and shift the hex digit, and store in temporary variable, a */ 
        a = (input_number & mask) >> bit_shift;

        /* Convert the hex digit into an ASCII character, and store in output string */
        output_string[pos] = (a < 10) ? (a + '0') : (a - 10 + 'A');

        /* Shift the bit mask 4 bits to the right, to convert the next digit */
        mask = mask >> 4u;

        /* Decrement the bit_shift counter by 4 (bits in a each digit) */
        bit_shift -= 4u;

        /* Increment the output string location */
        pos++;
    }
}
/*******************************************************************************
* End of function uint16_t_ToString
*******************************************************************************/

