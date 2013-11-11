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
/* Copyright (C) 2010. Renesas Electronics Corp., All Rights Reserved       */

/******************************************************************************
* File Name     : rdkrx62n.h
* Version       : 0.5
* Device(s)     : RDK RX62N
* Tool-Chain    :
* OS            : None
* H/W Platform  :
* Description   :
* Operation     :
* Limitations   : None
*******************************************************************************
* History : DD.MM.YYYY    Version Description
*         : 28.June.2010  0.50    First release
*******************************************************************************/

#ifndef RDKRX62N_H
#define RDKRX62N_H

/* Local defines */
#define LED_ON        (0)
#define LED_OFF       (1)
#define SET_BIT_HIGH  (1)
#define SET_BIT_LOW   (0)
#define SET_BYTE_HIGH (0xFF)
#define SET_BYTE_LOW  (0x00)

/* Define switches to be polled if not available as interrupts */
#define SW_ACTIVE FALSE
#define SW1       PORT4.PORT.BIT.B0
#define SW2       PORT4.PORT.BIT.B1
#define SW3       PORT4.PORT.BIT.B2

#define LED1  PORTD.DR.BIT.B0
#define LED2  PORTD.DR.BIT.B1
#define LED3  PORTD.DR.BIT.B2
#define LED4  PORTD.DR.BIT.B3
#define LED5  PORTD.DR.BIT.B4
#define LED6  PORTD.DR.BIT.B5
#define LED7  PORTD.DR.BIT.B6
#define LED8  PORTD.DR.BIT.B7

#define LED9  PORTE.DR.BIT.B0
#define LED10 PORTE.DR.BIT.B1
#define LED11 PORTE.DR.BIT.B2
#define LED12 PORTE.DR.BIT.B3

#define ALL_LEDS_ON		{LED1 = LED2 = LED3 = LED4 = LED5 = LED6 = LED_ON;}
#define ALL_LEDS_OFF	{LED1 = LED2 = LED3 = LED4 = LED5 = LED6 = LED_OFF;}

#endif