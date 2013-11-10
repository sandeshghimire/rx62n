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
* File Name     : wdt.c 
* Version       : 1.1
* Device(s)     : R5F562N8
* Tool-Chain    : Renesas RX Standard Toolchain 1.0.1
* OS            : None
* H/W Platform  : YRDKRX62N
* Description   : Defines watchdog timer functions used in this sample.
*******************************************************************************
* History : DD.MMM.YYYY     Version     Description
*         : 08.Oct.2010     1.00        First release
*         : 02.Dec.2010     1.10        Second YRDK release
*******************************************************************************/

/*******************************************************************************
* Project Includes
*******************************************************************************/
/* WDT RPDL function definitions */
#include "r_pdl_wdt.h"
/* TMR RPDL function definitions */
#include "r_pdl_tmr.h"
/* ADC RPDL function definitions */
#include "r_pdl_adc_12.h"
/* IO port RPDL function definitions */
#include "r_pdl_io_port.h"
/* General RPDL function definitions */
#include "r_pdl_definitions.h"
/* Defines LCD functions used in this file */
#include "lcd.h"
/* Defines switch functions and variables used in this file */
#include "switch.h"
/* Provides declarations of functions defined in this file */
#include "wdt.h"

/*******************************************************************************
* Local Function Prototypes
*******************************************************************************/
/* AD conversion complete callback function prototype declaration */
void CB_ADConversion(void);
/* Watchdog timer overflow interrupt callback function prototype declaration */
void CB_WDTOverflow(void);
/* Timer period interrupt callback function prototype delcaration */
void CB_TMRTimer(void);

/*******************************************************************************
* Local Function Prototypes
*******************************************************************************/
/* Flash/WDT reset interrupt period global variable */
float gTMR_Period = 5E-3;

/*******************************************************************************
* Outline      : Init_WDT
* Description  : This function configures the ADC unit for singleshot operation,
*                the watchdog for a 699ms timeout period, and the TMR unit to
*                create a periodic interrupt every 100ms.
* Argument     : none
* Return value : none
*******************************************************************************/
void Init_WDT(void)
{
    /* Declare error flag */
    bool err = true;

    /* Configure the ADC unit */
    err &=    R_ADC_12_Create
              (
              0,
              PDL_ADC_12_SCAN_SINGLE | PDL_ADC_12_CHANNEL_4 | PDL_ADC_12_DATA_ALIGNMENT_RIGHT | PDL_ADC_12_DIV_8,
              PDL_ADC_12_TRIGGER_SOFTWARE,
              PDL_NO_DATA,
              CB_ADConversion,
              5
              );

    /* Configure the WDT unit to timeout after 699ms, and call the function
       the callback function CB_WDTOverflow    when the timer overflows */
    err &=    R_WDT_Create
              (
              PDL_WDT_PCLK_DIV_131072,
              CB_WDTOverflow,
              7
              );    

    /* Configure TMR unit to create a periodic interupt every 100ms,
       and execute the callback function CB_TMRTimer every period */                
    err &=    R_TMR_CreatePeriodic
              (
              PDL_TMR_UNIT0,
              PDL_TMR_PERIOD,
              100E-3,
              10E-3,
              PDL_NO_FUNC,
              CB_TMRTimer,
              3
              );

    /* Halt in while loop when RPDL errors detected */    
    while (!err);
}
/*******************************************************************************
* End of function Init_WDT
*******************************************************************************/

/*******************************************************************************
* Outline      : CB_ADConversion
* Description  : This callback function is executed after every succesful AD
*                conversion. The function fetches the ADC result, and uses it to
*                calculate a new timer period for the LED flash/WDT reset ISR.
* Argument     : none
* Return value : none
*******************************************************************************/
void CB_ADConversion(void)
{
    /* Declare error flag */
    bool err = true;

    /* Declare temporary ADC result container variable */
    uint16_t adc_result[8] ;

    /* Fetch the result from the ADC conversion */
    err &= R_ADC_12_Read
           (
           0,
           adc_result
           );

    /* Set the timer period global variable value relative to the ADC result.
       Timer period is one thousand of the ADC result will a 20ms offset */
    gTMR_Period = (float) (((adc_result[4] >> 2) * 1E-3) + 20E-3);

    /* Halt in while loop when RPDL errors detected */
    while (!err);

}
/*******************************************************************************
* End of function CB_ADConversion
*******************************************************************************/

/*******************************************************************************
* Outline      : CB_TMRTimer
* Description  : Timer period interrupt callback function. This function resets
*                the watchdog timer to prevent it overflowing, and toggles the 
*                user LEDs. The function also changes the time period between
*                timer interrupts, by changing the timer period to the value 
*                stored in the variable, gTMR_Period.
* Argument     : none
* Return value : none
*******************************************************************************/
void CB_TMRTimer(void)
{
    /* Declare error flag */
    bool err = true;

    /* Reset the watchdog timer */
    err &=    R_WDT_Control
              (
              PDL_WDT_RESET_COUNTER
              );

    /* Toggle user LEDs */
    err &=    R_IO_PORT_Modify
              (
              PDL_IO_PORT_D,
              PDL_IO_PORT_XOR,
              0xFF
              );

    /* Toggle user LEDs */        
    err &=    R_IO_PORT_Modify
              (
              PDL_IO_PORT_E,
              PDL_IO_PORT_XOR,
              0x0F
              );

    /* Trigger AD conversion to start */
    err &=    R_ADC_12_Control
              (
              PDL_ADC_12_0_ON
              );

    /* Reconfigure the timer interrupt period with the value of the
       globabl variable gTMR_Period */
    err &=    R_TMR_ControlPeriodic
              (
              PDL_TMR_UNIT0,
              PDL_TMR_PERIOD | PDL_TMR_START,
              gTMR_Period,
              1E-3
              );    

    /* Halt in while loop when RPDL errors detected */    
    while (!err);        
}
/*******************************************************************************
* End of function CB_TMRTimer
*******************************************************************************/

/*******************************************************************************
* Outline      : CB_WDTOverflow
* Description  : This callback function is executed when the watchdog timer
*                overflows. The function sets the user LEDs ON, displays
*                "Watchdog Overflow" on the debug LCD, and waits in an infinite
*                while loop.
* Argument     : none
* Return value : none
*******************************************************************************/
void CB_WDTOverflow(void)
{
    /* Declare error flag */
    bool err = true;

    /* Turn user LEDs on */
    err &=    R_IO_PORT_Write
              (
              PDL_IO_PORT_D,
              0
              );

    /* Turn user LEDs on */
    err &=    R_IO_PORT_Write
              (
              PDL_IO_PORT_E,
              0
              );

    /* Display 'Watchdog Overflow' message on the debug LCD */
    ClearLCD() ;
    DisplayLCD(LCD_LINE4, "  Watchdog");
    DisplayLCD(LCD_LINE5, "  Overflow");

    /* Halt in while loop when RPDL errors detected */    
    while (!err);        

    /* Infinite while loop */    
    while (1);
}
/*******************************************************************************
* End of function CB_WDTOverflow
*******************************************************************************/