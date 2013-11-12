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
* File Name     : main_uart.c
* Version       : 1.1
* Device(s)     : R5F562N8
* Tool-Chain    : Renesas RX Standard Toolchain 1.0.1
* OS            : None
* H/W Platform  : YRDKRX62N
* Description   : This sample demonstrates the SCI unit in asynchronous mode, by
*                 transmitting the numbers 0 to 9 to the terminal.
* Operation     : 1. Build and download the sample code to the YRDK.
*
*                 2. Connect the YRDK to the host PC via an RS232 serial cable.
*
*                 3. Launch a suitable terminal program (e.g. Hyperterminal) and
*                    configure it to operate on the channel the YRDK is connected
*                    to with the following settings:
*
*                                    Baud............19200
*                                    Data Bits...........8
*                                    Parity...........None
*                                    Stop Bits...........1
*                                    Flow Control.....None
*                
*                 4. Click 'Reset Go' to start the software.
*                 
*                 5. Observe the terminal window - instructions will be displayed
*                    at the top, then the numbers 0 - 9 will be written across
*                    the screen repeatedly. 
*
*                 6. Type 'z' (lowercase) into the terminal to pause the SCI
*                    transmission, and any other key to resume it.
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
/* Defines DMAC functions used in this file */
#include "async.h"
/* RSPI API library support */
#include "YRDKRX62N_RSPI_API.h"
/* Board specific definitions */
#include "YRDKRX62N.h"

/*******************************************************************************
* Outline       : main
* Description   : Main program function. This function first displays on the 
*                 debug LCD; then calls the async initialisation function, which
*                 configures and starts the SCI unit to transmit the numbers 0 to
*                 9 on the terminal untill the character 'z' is typed into the
*                 terminal. Any other key will resume the transfer.
* Argument      : none
* Return value  : none
*******************************************************************************/
void main(void)
{
    /* Initialise the LCD on the RSPI bus */
    YRDKRX62N_RSPI_Init(RSPI_CHANNEL_0);
    InitialiseLCD();

    /* Display instructions onto the LCD */
    DisplayLCD(LCD_LINE1, "Async   ");
    DisplayLCD(LCD_LINE2, "Serial  ");

    /* Initialise the SCI unit for asynchronous operation */
    Init_Async();

    /* Infinite while loop */
    while (1);

}
/*******************************************************************************
* End of function main
*******************************************************************************/

