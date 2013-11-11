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
* File Name     : id_code.c
* Version       : 1.1
* Device(s)     : R5F562N8
* Tool-Chain    : Renesas RX Standard Toolchain 1.0.1
* OS            : None
* H/W Platform  : YRDKRX62N
* Description   : Defines the 128-bit MCU ROM protection code
*******************************************************************************
* History : DD.MMM.YYYY     Version     Description
*         : 12.May.2010     1.00        First release
*         : 02.Dec.2010     1.10        Second YRDK release
*******************************************************************************/

/******************************************************************************
User Includes (Project level includes)
******************************************************************************/
/* Defines standard typedefs used in this file */
#include <stdint.h>

/******************************************************************************
System Definitions & Global Variables
******************************************************************************/
/* Do not modify this entry  */
#pragma address id_code = 0xFFFFFFA0

/* No ID code protection */
const uint32_t id_code[4] = {
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFF
};

/* Example: ID code protection, method 1.  Replace nn.... with the required code */
/*
const uint32_t id_code[4] = {
    0x45nnnnnn,
    0xnnnnnnnn,
    0xnnnnnnnn,
    0xnnnnnnnn
};
*/