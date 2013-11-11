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
* File Name     : hwsetup.c
* Version       : 1.1
* Device(s)     : R5F562N8
* Tool-Chain    : Renesas RX Standard Toolchain 1.0.1
* OS            : None
* H/W Platform  : YRDKRX62N
* Description   : Defines the initialisation routines used each time the MCU
*                 is restarted. 
*******************************************************************************
* History : DD.MMM.YYYY     Version     Description
*         : 08.Oct.2010     1.00        First release
*         : 02.Dec.2010     1.10        Second YRDK release
*******************************************************************************/

/******************************************************************************
* Project Includes
******************************************************************************/
/* IO Port RPDL function definitions */
#include "r_pdl_io_port.h"
/* CGC RPDL function definitions */
#include "r_pdl_cgc.h"
/* General RPDL function definitions */
#include "r_pdl_definitions.h"
/* Switch handler function definitions */
#include "switch.h"
/* Contains delcarations for the functions defined in this file */
#include "hwsetup.h"
/* Board specific definitions */
#include "YRDKRX62N.h"

/******************************************************************************
* Outline       : HardwareSetup
* Description   : Contains all the setup functions called at device restart
* Argument      : none
* Return value  : none
******************************************************************************/
void HardwareSetup(void)
{
    ConfigureOperatingFrequency();
    ConfigureOutputPorts();
    ConfigureInterrupts();
    EnablePeripheralModules();
}
/******************************************************************************
* End of function HardwareSetup
******************************************************************************/

/******************************************************************************
* Outline       : ConfigureOperatingFrequency
* Description   : Configures the clock settings for each of the device clocks
* Argument      : none
* Return value  : none
******************************************************************************/
void ConfigureOperatingFrequency(void)
{
    /* Declare error flag */
    bool err = true;

    /* Modify the MCU clocks */
    err &=     R_CGC_Set
               (
               12E6,
               96E6,
               48E6,
               24E6,
               PDL_NO_DATA
               );
    /* 
    Clock Description              Frequency
    ----------------------------------------
    Input Clock Frequency..............12MHz
    Internal Clock Frequency...........96MHz    
    Peripheral Clock Frequency.........48MHz
    External Bus Clock Frequency.......24MHz */

    /* Halt in while loop when RPDL errors detected */    
    while (!err);
}
/******************************************************************************
* End of function ConfigureOperatingFrequency
******************************************************************************/

/******************************************************************************
* Outline       : ConfigureOutputPorts
* Description   : Configures the port and pin direction settings, and sets the
*                 pin outputs to a safe level.
* Argument      : none
* Return value  : none
******************************************************************************/
void ConfigureOutputPorts(void)
{
    /* Declare error flag */
    bool err = true;

    /* Port 0 - all inputs (IRQ's from ethernet & WiFi) */
    /* Port 1 - all inputs (IIC and USB settings will override these later */

    /* Port 2 - USB signals */
    err &= R_IO_PORT_Set
           (
           USB0_EXICEN | USB_DPUPEA | USB_VBUSEN,
           PDL_IO_PORT_OUTPUT
           );

    /* Port 3 - JTAG (P30, P31, P34), CAN (P32=Tx, P33=Rx), NMI (P35) */
    err &= R_IO_PORT_Set
           (
           CAN_WIFI_TX,
           PDL_IO_PORT_OUTPUT
           );

    /* Port 4 - Switches (P40-P42), AIN (P43-P47)  */
    err &= R_IO_PORT_Set
           (
           SW1 | SW2 | SW3 | JN1_AN3 | 
           POTENTIOMETER | MICROPHONE | AUDIO_IN_R | PWMLP_IN,
           PDL_IO_PORT_INPUT | PDL_IO_PORT_INPUT_BUFFER_ON
           );

    /* Port 5 - Audio (P55,P54), BCLK (P53), SCI (P52=Rx, P50=Tx), LCD-RS (P51) */
    err &= R_IO_PORT_Set
           (
           LCD_RS | LCD_A0 | BCLK | PWMLP_OUT | AMP_SHDN,
           PDL_IO_PORT_OUTPUT
           );

    /* Port A outputs all LOW to start  */
    err &= R_IO_PORT_Write
           (
           PDL_IO_PORT_A,
           0
           );

    /* Port A - Expansion (PA0-PA2), Ether (PA3-PA5), Audio (PA6-PA7) */
    err &= R_IO_PORT_Set
           (
           JN2_SSLA1_B | JN2_SSLA2_B | JN2_SSLA3_B | ETH_MDIO | 
           ETH_MDC     | ETH_LINK    | AUDIO_OUT_R | AUDIO_OUT_L ,
           PDL_IO_PORT_OUTPUT
           );

    /* Port B outputs all LOW to start  */
    err &= R_IO_PORT_Write
           (
           PDL_IO_PORT_B,
           0
           );

    /* Port B - Ether */
    err &= R_IO_PORT_Set
           (
           RMII_TX_EN | RMII_TXD0 | RMII_TXD1,
           PDL_IO_PORT_OUTPUT
           );

    /* Port C - Chip selects, clock = high; IO reset = low */
    err &= R_IO_PORT_Write 
           (
           PDL_IO_PORT_C,
           0xF7
           );

    /* Port C - SPI (PC0-2, PC4-7), IO reset (PC3) */
    err &= R_IO_PORT_Set
           (
           FLASH_CS   | WIFI_CS     | LCD_CS | LCD_RESET |
           SDMICRO_CS | RSPI_SCLOCK | RSPI_MOSI,
           PDL_IO_PORT_OUTPUT
           );

    /* Port D - All LED's off */
    err &= R_IO_PORT_Write 
           (
           PDL_IO_PORT_D, 
           0xFF
           );

    /* Port D - LED's */
    err &= R_IO_PORT_Set
           (
           LED14 | LED10 | LED6  | LED12 |
           LED8  | LED4  | LED15 | LED11,
           PDL_IO_PORT_OUTPUT
           );

    /* Port E - All LED's off */
    err &= R_IO_PORT_Write
           (
           PDL_IO_PORT_E,
           0x0F
           );

    /* Port E - LED's */
    err &= R_IO_PORT_Set
           (
           LED7 | LED13 | LED9 | LED5,
           PDL_IO_PORT_OUTPUT
           );

    /* Halt in while loop when RPDL errors detected */    
    while (!err);
}
/******************************************************************************
* End of function ConfigureOutputPorts
******************************************************************************/

/******************************************************************************
* Outline       : ConfigureInterrupts
* Description   : Configures interrupts used
* Argument      : none
* Return value  : none
******************************************************************************/
void ConfigureInterrupts(void)
{
    /* Configure switch interrupts */
    InitialiseSwitchInterrupts();
}
/******************************************************************************
* End of function ConfigureInterrupts
******************************************************************************/

/******************************************************************************
* Outline       : EnablePeripheralModules
* Description   : Enables and configures peripheral devices on the MCU
* Argument      : none
* Return value  : none
******************************************************************************/
void EnablePeripheralModules(void)
{

}
/******************************************************************************
* End of function EnablePeripheralModules
******************************************************************************/