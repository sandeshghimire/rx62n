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
* File Name     : thermal_sensor.c
* Version       : 1.0
* Device(s)     : R5F562N8
* Tool-Chain    : Renesas RX Standard Toolchain 1.0.1
* OS            : None
* H/W Platform  : YRDKRX62N
* Description   : Driver for the ADT7420 I2C thermal sensor.
*******************************************************************************
* History : DD.MMM.YYYY     Version     Description
*         : 02.Dec.2010     1.00        First release
*******************************************************************************/

/*******************************************************************************
* Project Includes
*******************************************************************************/
/* IIC RPDL function definitions */
#include "r_pdl_iic.h"
/* CMT RPDL function definitions */
#include "r_pdl_cmt.h"
/* General RPDL function definitions */
#include "r_pdl_definitions.h"
/* Defines ADT7420 parameters */
#include "ADT7420.h"
/* Defines IIC parameters */
#include"iic_defines.h"
/* IIC RPDL function definitions */
#include "r_pdl_iic.h"
/* Defines LCD functions used in this file */
#include "lcd.h"
/* Defines string functions used in this file */
#include <string.h>
#include <stdio.h>

/* Provides declarations of functions defined in this file */

/*******************************************************************************
* Global Variables
*******************************************************************************/

/*******************************************************************************
* Local Function Prototypes
*******************************************************************************/
/* CMT interrupt callback function prototype declaration */
void   CB_CMT_Thermal(void);

/*******************************************************************************
* Outline      : Init_Thermal_Sensor
* Description  : This function configures the ADT7420 thermal device and starts
*                a timer to periodically read the temperature.
* Argument     : none
* Return value : none
*******************************************************************************/
void Init_Thermal_Sensor(void)
{
    /* Declare error flag */
    bool     err = true;
    uint8_t  target_reg, target_data;

    DisplayLCD(LCD_LINE5, "Temp:");

    /* Configure the ADT7420 */
    target_reg = ADT7420_CONFIG_REG;
    err = R_IIC_MasterSend( IIC_CHANNEL, PDL_NO_DATA, ADT7420_ADDR, &target_reg,
                            1, PDL_NO_FUNC, 0 );
    target_data = 0x00;                                     
    err = R_IIC_MasterSend( IIC_CHANNEL, PDL_NO_DATA, ADT7420_ADDR, &target_data,
                            1, PDL_NO_FUNC, 0 );

    if (err!=0)
    {
        /* Configure CMT3 to execute callback function every 250ms */
        err &=    R_CMT_Create( 3, PDL_CMT_PERIOD, 250E-3, CB_CMT_Thermal, 3 );
    }
    else
    {
        /* Temperature sensor not installed on this board */
        DisplayLCD(LCD_LINE6, "No sensor");
    }

}
/*******************************************************************************
* End of function Init_Thermal_Sensor
*******************************************************************************/


/*******************************************************************************
* Outline      : CB_CMT_Thermal
* Description  : CMT interrupt callback function. This callback function is
*                executed after every period of the CMT timer. The function 
*                fetches the temperature and displays it on the LCD.
* Argument     : none
* Return value : none
*******************************************************************************/
void CB_CMT_Thermal(void)
{
    bool         err = true;
    uint8_t      target_reg, target_data[2];
    uint16_t     temp;
    uint8_t      temp_int[2];

    /* Declare display buffer */
    uint8_t lcd_buffer[13];

    /* Read the temperature */
    target_reg = ADT7420_TEMP_MSB_REG;
    err = R_IIC_MasterSend( IIC_CHANNEL, PDL_NO_DATA, ADT7420_ADDR, &target_reg,
                            1, PDL_NO_FUNC, 0 );

    if (err!=0)
    {

        err = R_IIC_MasterReceive( IIC_CHANNEL, PDL_NO_DATA, ADT7420_ADDR, &target_data[0], 
                                   2, PDL_NO_FUNC, 0 );

        /* Convert the device measurement into a decimal number and insert 
           into a temporary string to be displayed */
        temp = (target_data[0] << 8) + target_data[1];
        temp = temp >> 3;
        temp_int[0] = (int16_t)  temp / 16;
        temp_int[1] = (int16_t)((temp & 0x000F) * 10) / 16;

        sprintf((char *)lcd_buffer, "   %d.%d C" , temp_int[0], temp_int[1] );   
    }
    else
    {
        sprintf((char *)lcd_buffer, "No sensor");   
    }

    /* Display the contents of lcd_buffer onto the debug LCD */
    DisplayLCD(LCD_LINE6, lcd_buffer);

}
/*******************************************************************************
* End of function CB_CMT_Thermal
*******************************************************************************/

