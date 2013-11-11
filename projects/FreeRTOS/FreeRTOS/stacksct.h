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
* File Name     : stacksct.h
* Version       : 1.10
* Device(s)     : R5F562N8
* Tool-Chain    : Renesas RX Standard Toolchain 1.0.1
* OS            : None
* H/W Platform  : YRDKRX62N
* Description   : Defines the MCU stack size
* Limitations   : None
*******************************************************************************
* History : DD.MMM.YYYY     Version     Description
*         : 08.Oct.2010     1.00        First release
*         : 02.Dec.2010     1.10        Second YRDK release
*******************************************************************************/

/*******************************************************************************
* Macro Definitions
*******************************************************************************/
/* Multiple inclusion prevention macro */
#ifndef STACKSCT_H
#define STACKSCT_H

/* Define the MCU stack size */
#pragma stacksize su=0x300      
#pragma stacksize si=0x100      

/* End of multiple inclusion prevention macro */
#endif