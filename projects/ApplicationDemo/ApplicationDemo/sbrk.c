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
* File Name     : sbrk.c
* Version       : 1.1
* Device(s)     : R5F562N8
* Tool-Chain    : Renesas RX Standard Toolchain 1.0.1
* OS            : None
* H/W Platform  : YRDKRX62N
* Description   : Configures the MCU heap memory.
*******************************************************************************
* History : DD.MMM.YYYY     Version     Description
*         : 08.Oct.2010     1.00        First release
*         : 02.Dec.2010     1.10        Second YRDK release
*******************************************************************************/

/******************************************************************************
* Project Includes
******************************************************************************/
/* Provides standard definitions used in this file */
#include <stddef.h>
/* Defines standard input/output functions used in this file */
#include <stdio.h>
/* Defines standard variable types used in this file */
#include "typedefine.h"
/* Declares functions defined in this file */
#include "sbrk.h"

/******************************************************************************
* Function Prototypes
******************************************************************************/
/* Memory allocation function prototype declaration */
_SBYTE  *sbrk(size_t size);

/******************************************************************************
* Global Variables
******************************************************************************/
//const size_t _sbrk_size=      /* Specifies the minimum unit of */
/* the defined heap area */
extern _SBYTE *_s1ptr;

union HEAP_TYPE
{
    _SDWORD  dummy ;            /* Dummy for 4-byte boundary */
    _SBYTE heap[HEAPSIZE];      /* Declaration of the area managed by sbrk*/
};
/* Declare memory heap area */
static union HEAP_TYPE heap_area ;
/* End address allocated by sbrk    */
static _SBYTE *brk=(_SBYTE *)&heap_area;

/*******************************************************************************
* Outline      : sbrk
* Description  : This function configures MCU memory area allocation.
* Argument     : size - assigned area size
* Return value : Start address of allocated area (pass)
*                 -1 (failure)
*******************************************************************************/
_SBYTE  *sbrk(size_t size)                      
{
    _SBYTE  *p;

    if (brk+size > heap_area.heap+HEAPSIZE)
    {
        /* Empty area size  */
        p = (_SBYTE *)-1;
    }
    else
    {
        /* Area assignment */
        p = brk;  

        /* End address update */                           
        brk += size;                           
    }

    /* Return result */
    return p;
}
/******************************************************************************
* End of function sbrk
******************************************************************************/
