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
* File Name     : main_iic_master.c
* Version       : 1.0
* Device(s)     : R5F562N8
* Tool-Chain    : Renesas RX Standard Toolchain 1.0.1
* OS            : None
* H/W Platform  : YRDKRX62N
* Description   : Demonstrates usage of IIC unit in master mode, by reading 
*                  actively reading from 2 separate IIC devices:
*                   -- ADXL345 digital accelerometer
*                   -- ADT7420 thermal sensor
*
* Operation     : 1. Build and download the sample code to the YRDK.
* 
*                 2. Click 'Reset Go' to start the software. The name of the
*                    sample will by displayed on the debug LCD, along with the
*                    current board temperature in Celcius.
*
*                 3. Place a finger over the ADT720 device at U17.  It is located
*                    below the APPLICATION HEADER J8 located along the top edge of
*                    the board.  Observe that the temperature is updated four 
*                    times per second.  When you remove your finger, the    
*                    temperature returns to normal.
*                    
*                 4. Pick up the board and tilt the board forward/backward and
*                    left/right.  
*
*                 5. As you slowly roll the board in a circular motion, observe 
*                    the circle LEDs light to indicate the direction of the tilt.
*******************************************************************************
* History : DD.MMM.YYYY     Version     Description
*         : 02.Dec.2010     1.00        First release
*******************************************************************************/

/*******************************************************************************
* Project Includes
*******************************************************************************/    
#include <stdbool.h>

/* Defines LCD functions used in this file */
#include "lcd.h"

/* Defines IIC parameters */
#include"iic_defines.h"

/* IIC RPDL function definitions */
#include "r_pdl_iic.h"

/* General RPDL function definitions */
#include "r_pdl_definitions.h"

/* RSPI API library support */
#include "YRDKRX62N_RSPI_API.h"

/* Board specific definitions */
#include "YRDKRX62N.h"

/* External functions */
void Init_Thermal_Sensor( void );
void Init_Accelerometer ( void );

/* Local functions */
void Init_IIC_Master(void);


/*******************************************************************************
* Outline      : main
* Description  : The main program function. This function first intializes the
*                debug LCD, and displays the sample name. It then calls the 
*                Init_IIC_Master to initialize the I2C bus and next calls
*                Init_Accelerometer, from which is does not exit from.
* Argument     : none
* Return value : none
*******************************************************************************/
void main(void)
{
    /* Initialise the debug LCD on the RSPI bus */
    YRDKRX62N_RSPI_Init(RSPI_CHANNEL_0);
    InitialiseLCD();

    /* Display IIC mode on the debug LCD */
    DisplayLCD(LCD_LINE1, " IIC Master");
    DisplayLCD(LCD_LINE2, "    Demo");

    DisplayLCD(LCD_LINE8, "Tilt board");
    
    /* Initialize IIC master */
    Init_IIC_Master();

    /* Initialize IIC thermal sensor */
    Init_Thermal_Sensor();

    /* Initialize IIC accelerometer */
    Init_Accelerometer();

    /* Infinite while loop */    
    while (1);
}
/*******************************************************************************
* End of function main
*******************************************************************************/

/*******************************************************************************
* Outline      : Init_IIC_Master
* Description  : This function configures the IIC unit to operate in bus master
*                 mode. 
* Argument     : none
* Return value : none
*******************************************************************************/
void Init_IIC_Master(void)
{
    /* Declare error flag */
    bool err = true;

    /* Configure the IIC unit to operate as bus master */
    err &=    R_IIC_Create
              (
              IIC_CHANNEL,                               // channel selection
              PDL_IIC_MODE_IIC | PDL_IIC_INT_PCLK_DIV_8, // channel configuration
              PDL_NO_DATA,                               // detection configuration
              PDL_NO_DATA,                               // slave address
              PDL_NO_DATA,                               // slave address
              PDL_NO_DATA,                               // slave address
              IIC_TRANSFER_DATA_RATE,                    // transfer rate control
              (SCL_RISE_TIME << 16) | SCL_FALL_TIME      // rise and fall time correction
              );            

    /* Halt in while loop when RPDL errors detected */    
    while (!err);
}
/*******************************************************************************
* End of function Init_IIC_Master
*******************************************************************************/