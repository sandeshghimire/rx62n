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
* File Name     : resetprg.c
* Version       : 1.1
* Device(s)     : R5F562N8
* Tool-Chain    : Renesas RX Standard Toolchain 1.0.1
* OS            : None
* H/W Platform  : YRDKRX62N
* Description   : Defines post-reset routines that are used to configure the
*                 MCU prior to the main program starting. This is were the 
*                 program counter starts on power-up or reset. 
*******************************************************************************
* History : DD.MMM.YYYY     Version     Description
*         : 08.Oct.2010     1.00        First release
*         : 02.Dec.2010     1.10        Second YRDK release
*******************************************************************************/

/******************************************************************************
* Project Includes
******************************************************************************/
/* Defines machine level functions used in this file */
#include  <machine.h>
/* Defines MCU configuration functions used in this file */
#include  <_h_c_lib.h>
/* Defines variable types used in this file */
#include  "typedefine.h"
/* Defines standard boolean variable types used in this file */
#include  <stdbool.h>
/* Defines the size of the stack which configured in this file */
#include  "stacksct.h"
/* Defines hardware setup functions that used to configure the MCU prior to
   the main program starting. */
#include  "hwsetup.h"

/******************************************************************************
* Macro Defintitions
******************************************************************************/
#define PSW_init  0x00010000
#define FPSW_init 0x00000100

/******************************************************************************
* Pre-processor Directives
******************************************************************************/
/* Declare the contents of the function 'Change_PSW_PM_to_UserMode' as
   assembler to the compiler */
#pragma inline_asm Change_PSW_PM_to_UserMode
/* Set this as the location of the 'ResetPRG' section */
#pragma section ResetPRG
/* Set this as the entry point from a power-on reset */
#pragma entry PowerON_Reset_PC

/******************************************************************************
* Local Function Prototypes
******************************************************************************/
/* MCU usermode switcher function declaration */
static void Change_PSW_PM_to_UserMode(void);
/* Power-on reset function declaration */
void PowerON_Reset_PC(void);
/* Main program function delcaration */
void main(void);

/******************************************************************************
* Outline       : PowerON_Reset_PC
* Description   : This program is the MCU's entry point from a power-on reset.
*                 The function configures the MCU stack, then calls the
*                 HardwareSetup function and main function sequentially.
* Argument      : none
* Return value  : none
******************************************************************************/
void PowerON_Reset_PC(void)
{
    /* Initialise the MCU processor word */
#if __RENESAS_VERSION__ >= 0x01010000    
    set_intb((void *)__sectop("C$VECT"));
#else
    set_intb((unsigned long)__sectop("C$VECT"));
#endif    
    set_fpsw(FPSW_init);

    /* Initialise the MCU stack area */
    _INITSCT();

    /* Configure the MCU and YRDK hardware */
    HardwareSetup();

    /* Change the MCU's usermode from supervisor to user */        
    nop();
    set_psw(PSW_init);      
    Change_PSW_PM_to_UserMode();

    /* Call the main program function */
    main();

    /* Invoke a break interrupt */
    brk();
}
/******************************************************************************
* End of function PowerON_Reset_PC
******************************************************************************/

/******************************************************************************
* Outline       : Change_PSW_PM_to_UserMode
* Description   : Assembler function, used to change the MCU's usermode from
*                 supervisor to user.
* Argument      : none
* Return value  : none
******************************************************************************/
static void Change_PSW_PM_to_UserMode(void)
{
    MVFC   PSW,R1
    OR     #00100000h,R1
    PUSH.L R1
    MVFC   PC,R1
    ADD    #10,R1
    PUSH.L R1
    RTE
    NOP
    NOP
}

/******************************************************************************
* End of function Change_PSW_PM_to_UserMode
******************************************************************************/