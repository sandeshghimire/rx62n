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
* File Name     : switch.h
* Version       : 1.10
* Device(s)     : R5F562N8
* Tool-Chain    : Renesas RX Standard Toolchain 1.0.1
* OS            : None
* H/W Platform  : YRDKRX62N
* Description   : Provides declarations for functions defined in switch.c.
* Limitations   : None
*******************************************************************************
* History : DD.MMM.YYYY     Version     Description
*         : 08.Oct.2010     1.00        First release
*         : 02.Dec.2010     1.10        Second YRDK release
*******************************************************************************/

/*******************************************************************************
* Project Includes
*******************************************************************************/
/* Defines standard boolean variable types used in this file */
#include <stdbool.h>
/* Defines standard variable types used in this file */
#include <stdint.h>

/*******************************************************************************
* Macro Definitions
*******************************************************************************/
/* Multi inclusion prevention macro */
#ifndef SWITCH_H
#define SWITCH_H

/* Switch 1 pressed flag mask */
#define SWITCHPRESS_1        0x80
/* Switch 1 held down flag mask */
#define SWITCHHOLD_1         0x08
/* Switch 2 pressed flag mask */
#define SWITCHPRESS_2        0x40
/* Switch 2 held down flag mask */
#define SWITCHHOLD_2         0x04
/* Switch 3 pressed flag mask */
#define SWITCHPRESS_3        0x20
/* Switch 3 held down flag mask */
#define SWITCHHOLD_3         0x02
/* Any switch pressed flag mask */
#define SWITCHPRESS_ALL      0xE0
/* Any switch held down flag mask */
#define SWITCHHOLD_ALL       0x0E
    
/******************************************************************************
* Global Variables
******************************************************************************/
/* Switch standby ready global flag */
extern volatile bool gSwitchStandbyReady;
/* Switch flag global variable. Switch status bits:
        b7 : Switch 1 press complete flag*
        b6 : Switch 2 press complete flag*
        b5 : Switch 3 press complete flag*
        b4 : Unused
        b3 : Switch 1 held-down status flag
        b2 : Switch 2 held-down status flag
        b1 : Switch 3 held-down status flag
        b0 : Unused    
         * Switch press complete flags must be cleared manually     */
extern volatile uint8_t gSwitchFlag;

/******************************************************************************
* Global Function Prototypes
******************************************************************************/
/* Switch initialisation function prototype */
void InitialiseSwitchInterrupts(void);
/* Switch enable/disable function prototype */
void ControlSwitchInterrupts(bool);
/* Switch callback function initialisation function prototype */
void SetSwitchPressCallback(void(*callBack)(void));
/* Switch callback function initialisation function prototype */
void SetSwitchReleaseCallback(void(*callBack)(void));


/* End of multiple inclusion prevention macro */
#endif