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
* File Name     : accelerometer.c
* Version       : 1.0
* Device(s)     : R5F562N8
* Tool-Chain    : Renesas RX Standard Toolchain 1.0.1
* OS            : None
* H/W Platform  : YRDKRX62N
* Description   : Driver for the Analog Devices ADXL345 accelerometer
*******************************************************************************
* History : DD.MMM.YYYY     Version     Description
*         : 02.Dec.2010     1.00        First release
*******************************************************************************/

/*******************************************************************************
* Project Includes
*******************************************************************************/
/* IIC RPDL function definitions */
#include "r_pdl_iic.h"
/* I/O port RPDL definitions */
#include "r_pdl_io_port.h"
/* CMT RPDL function definitions */
#include "r_pdl_cmt.h"
/* General RPDL function definitions */
#include "r_pdl_definitions.h"
/* Defines ADXL345 parameters */
#include "ADXL345.h"
/* Defines IIC parameters */
#include"iic_defines.h"

/* Defines LCD functions used in this file */
#include "lcd.h"
/* Defines string functions used in this file */
#include <string.h>
#include <stdio.h>

/* Provides declarations of functions defined in this file */


/*******************************************************************************
* Global Variables
*******************************************************************************/
volatile short Accel_X_Zero, Accel_X;
volatile short Accel_Y_Zero, Accel_Y;
volatile short Accel_Z_Zero, Accel_Z;


/*******************************************************************************
* Local Function Prototypes
*******************************************************************************/
/* CMT interrupt callback function prototype declaration */
void   CB_CMT_Accelerometer(void);
short  Accel_AxisRd(uint8_t);
bool   Accel_SelfTest( void );

/*******************************************************************************
* Outline      : Init_Accelerometer
* Description  : This function configures the ADXL345 accelerometer and starts
*                a timer to periodically read it.
* Argument     : none
* Return value : none
*******************************************************************************/
void Init_Accelerometer(void)
{
    /* Declare error flag */
    bool     err = true;
    uint8_t  target_reg, target_data;
    uint16_t i;    
    uint8_t     target_reg_data[2];

    // Read the DEVID register and verify the device
    target_reg = 0x00;
    err &= R_IIC_MasterSend( IIC_CHANNEL, PDL_NO_DATA, ADXL345_ADDR, &target_reg,
                             1, PDL_NO_FUNC, 0 );
    err &= R_IIC_MasterReceive( IIC_CHANNEL, PDL_NO_DATA, ADXL345_ADDR, &target_data, 
                                1,     PDL_NO_FUNC, 0 );                                 
    if ( target_data != 0xE5 )
        return;

    // set the data format
#if 0         
    // example of writing slave address and data as separate function calls
    target_reg = ADXL345_DATA_FORMAT_REG;
    err &= R_IIC_MasterSend( IIC_CHANNEL, PDL_IIC_STOP_DISABLE, ADXL345_ADDR, &target_reg,
                             1, PDL_NO_FUNC, 0 );
    target_data = 3;                                 
    err &= R_IIC_MasterSend( IIC_CHANNEL, PDL_IIC_START_DISABLE, ADXL345_ADDR, &target_data,
                             1, PDL_NO_FUNC, 0 );         
#else        
    // example of writing slave address and data in same function call
    target_reg_data[0] = ADXL345_DATA_FORMAT_REG;
    target_reg_data[1] = 3;    
    err &= R_IIC_MasterSend( IIC_CHANNEL, PDL_NO_DATA, ADXL345_ADDR, &target_reg_data[0],
                             2, PDL_NO_FUNC, 0 );
#endif

    // put FIFO into bypass mode
    target_reg_data[0] = ADXL345_FIFO_CTL_REG;
    target_reg_data[1] = 0;    
    err &= R_IIC_MasterSend( IIC_CHANNEL, PDL_NO_DATA, ADXL345_ADDR, &target_reg_data[0],
                             2, PDL_NO_FUNC, 0 );

    // set the measure bit in the power control register                                     
    target_reg_data[0] = ADXL345_POWER_CTL_REG;
    target_reg_data[1] = 8;    
    err &= R_IIC_MasterSend( IIC_CHANNEL, PDL_NO_DATA, ADXL345_ADDR, &target_reg_data[0],
                             2, PDL_NO_FUNC, 0 );                                 

    // get baseline readings to calibrate accelerometer
    Accel_X_Zero = 0;
    Accel_Y_Zero = 0;
    Accel_Z_Zero = 0;

    for (i = 0; i < 8; i++)
    {
        Accel_X_Zero += Accel_AxisRd(0x32);
        Accel_Y_Zero += Accel_AxisRd(0x34);
        Accel_Z_Zero += Accel_AxisRd(0x36);            
    }

    // determine the average reading
    Accel_X_Zero = Accel_X_Zero / 8;
    Accel_Y_Zero = Accel_Y_Zero / 8;
    Accel_Z_Zero = Accel_Z_Zero / 8;


    // run self test
    err &= Accel_SelfTest();


    // Activate X, Y Z to detect activity
    target_reg_data[0] = ADXL345_ACT_INACT_CTL_REG;
    target_reg_data[1] = 0x70;    
    err &= R_IIC_MasterSend( IIC_CHANNEL, PDL_NO_DATA, ADXL345_ADDR, &target_reg_data[0],
                             2, PDL_NO_FUNC, 0 );


#ifdef ACCELEROMETER_DEBUG        
    // read all the registers for debug
    target_reg = ADXL345_THRESH_TAP_REG;
    err &= R_IIC_MasterSend( IIC_CHANNEL, PDL_NO_DATA, ADXL345_ADDR, &target_reg,
                             1, PDL_NO_FUNC, 0 );
    err &= R_IIC_MasterReceive( IIC_CHANNEL, PDL_NO_DATA, ADXL345_ADDR, &target_string[0], 
                                29, PDL_NO_FUNC, 0 );        
#endif    


    /* Configure CMT2 to execute callback function every 100ms */
    err &=    R_CMT_Create( 2, PDL_CMT_PERIOD, 100E-3, CB_CMT_Accelerometer, 3 );                                                     

    /* Halt in while loop when RPDL errors detected */    
    while (!err);
}
/*******************************************************************************
* End of function Init_Accelerometer
*******************************************************************************/



/*******************************************************************************
* Outline      : CB_CMT_Accelerometer
* Description  : CMT interrupt callback function. This callback function is
*                executed after every period of the CMT timer. The function 
*                fetches the ADXL345 accelerometer readings & updates the LED's.
* Argument     : none
* Return value : none
*******************************************************************************/
void CB_CMT_Accelerometer(void)
{

#ifdef ACCELEROMETER_DEBUG    
    /* Declare display buffer */
    uint8_t  lcd_buffer[13];
#endif    
    uint16_t led;

    short adjusted_X, adjusted_Y, adjusted_Z;
    short slope = 0;

    /* Display the contents of lcd_buffer onto the debug LCD */

    Accel_X = Accel_AxisRd(ADXL345_DATAX0_REG);
    Accel_Y = Accel_AxisRd(ADXL345_DATAY0_REG);
    Accel_Z = Accel_AxisRd(ADXL345_DATAZ0_REG);        

    adjusted_X = Accel_X - Accel_X_Zero;
    adjusted_Y = Accel_Y - Accel_Y_Zero;
    adjusted_Z = Accel_Z - Accel_Z_Zero;

    /* calculate the slope, make sure not dividing by zero */
    if ( adjusted_X == 0 )
         adjusted_X =  1;
    slope = (100 * adjusted_Y) / adjusted_X;

#ifdef ACCELEROMETER_DEBUG        
    sprintf((char *)lcd_buffer, " x = %d" , adjusted_X);
    DisplayLCD(LCD_LINE4, lcd_buffer);

    sprintf((char *)lcd_buffer, " y = %d" , adjusted_Y);    
    DisplayLCD(LCD_LINE5, lcd_buffer);

    sprintf((char *)lcd_buffer, " z = %d" , adjusted_Z);    
    DisplayLCD(LCD_LINE6, lcd_buffer);
#endif

    // turn all LEDs off

    /* Port D: All LED's off */
    R_IO_PORT_Write( PDL_IO_PORT_D, 0xFF );    

    /* Port E: All LED's off */
    R_IO_PORT_Write( PDL_IO_PORT_E, 0x0F );    

    /* ignore baseline noise */    
    if ((adjusted_X < 2) && (adjusted_X > -2 ) &&        
        (adjusted_Y < 2) && (adjusted_Y > -2 ))
        return;

    if ((slope > 0) && (slope < 55))
    {
        if ( adjusted_X < 0 )
            led = PDL_IO_PORT_D_2;    // LED 6
        else
            led = PDL_IO_PORT_D_3;    // LED 12
    }
    else if ((slope >= 55) && (slope < 170))
    {
        if ( adjusted_X < 0 )
            led = PDL_IO_PORT_E_3;    // LED 5
        else
            led = PDL_IO_PORT_D_7;    // LED 11
    }
    else if (slope >= 170)
    {
        if ( adjusted_X < 0 )
            led = PDL_IO_PORT_D_5;    // LED 4
        else
            led = PDL_IO_PORT_D_1;    // LED 10
    }
    else if ((slope <= 0) && (slope > -55))
    {
        if ( adjusted_X > 0 )
            led = PDL_IO_PORT_E_1;    // LED 13
        else
            led = PDL_IO_PORT_E_0;    // LED 7        
    }
    else if ((slope <= -55) && (slope > -170))
    {
        if ( adjusted_X > 0 )
            led = PDL_IO_PORT_D_0;    // LED 14
        else
            led = PDL_IO_PORT_D_4;    // LED 8
    }
    else if (slope <= -170)
    {
        if ( adjusted_X > 0 )
            led = PDL_IO_PORT_D_6;    // LED 15
        else
            led = PDL_IO_PORT_E_2;    // LED 9
    }

    adjusted_Z = adjusted_Z ; /* Suppress compiler warning about not using this var */
    
    // turn on the appropriate LED
    R_IO_PORT_Modify( led, PDL_IO_PORT_XOR, 1 );

}
/*******************************************************************************
* End of function CB_CMT_Accelerometer
*******************************************************************************/



/*******************************************************************************
* Outline      : Accel_SelfTest
* Description  : This function reads the acceleromter's X axis
* Argument     : none
* Return value : bool     true - pass, false - fail
*******************************************************************************/
bool  Accel_SelfTest( void )
{

    bool     err =true;
    uint8_t     target_reg_data[2];    
    uint16_t i;

    // set the data format + self test mode
    target_reg_data[0] = ADXL345_DATA_FORMAT_REG;
    target_reg_data[1] = 0x83;    
    err &= R_IIC_MasterSend( IIC_CHANNEL, PDL_NO_DATA, ADXL345_ADDR, &target_reg_data[0],
                             2, PDL_NO_FUNC, 0 );


    // Wait for self-test forces to act
    for (i = 0; i < 1000; i++);

    Accel_X = 0;
    Accel_Y = 0;
    Accel_Z = 0;

    for (i = 0; i < 8; i++)
    {
        Accel_X += Accel_AxisRd(ADXL345_DATAX0_REG);
        Accel_Y += Accel_AxisRd(ADXL345_DATAY0_REG);
        Accel_Z += Accel_AxisRd(ADXL345_DATAZ0_REG);        
    }

    Accel_X = Accel_X / 8;
    Accel_Y = Accel_Y / 8;
    Accel_Z = Accel_Z / 8;

    // normalize the self test values
    Accel_X = SCALE_X(Accel_X) - Accel_X_Zero;
    Accel_Y = SCALE_Y(Accel_Y) - Accel_Y_Zero;
    Accel_Z = SCALE_Z(Accel_Z) - Accel_Z_Zero;

    // Range check self-test values
    err &= ((Accel_X >   6) && (Accel_X <  67)) ? true : false;
    err &= ((Accel_Y > -67) && (Accel_Y <  -6)) ? true : false;
    err &= ((Accel_Z >  10) && (Accel_Z < 110)) ? true : false;



    // turn off self test mode
    target_reg_data[0] = ADXL345_DATA_FORMAT_REG;
    target_reg_data[1] = 0x03;    
    err &= R_IIC_MasterSend( IIC_CHANNEL, PDL_NO_DATA, ADXL345_ADDR, &target_reg_data[0],
                             2, PDL_NO_FUNC, 0 );

    return err;                                 
}

/*******************************************************************************
* Outline      : Accel_AxisRd
* Description  : This function reads the acceleromter's X, Y or Z axis
* Argument     : uint8_t    axis -- axis registers to read
* Return value : short value of axis
*******************************************************************************/
short  Accel_AxisRd (uint8_t axis)
{
    uint16_t    axis_val;
    uint8_t      accel_reg, accel_data[2];


    accel_reg = axis;
    R_IIC_MasterSend( IIC_CHANNEL, PDL_NO_DATA, ADXL345_ADDR, &accel_reg,
                      1, PDL_NO_FUNC, 0 );
    R_IIC_MasterReceive( IIC_CHANNEL, PDL_NO_DATA, ADXL345_ADDR, &accel_data[0], 
                         2, PDL_NO_FUNC, 0 );

    axis_val  = accel_data[1] << 8;
    axis_val += accel_data[0];


    return((short)axis_val);
}
