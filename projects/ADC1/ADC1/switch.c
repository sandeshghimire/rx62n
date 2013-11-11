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
* File Name     : switch.c
* Version       : 1.1
* Device(s)     : R5F562N8
* Tool-Chain    : Renesas RX Standard Toolchain 1.0.1
* OS            : None
* H/W Platform  : YRDKRX62N
* Description   : Defines interrupt service routine functions for the switch
*                 interrupts. Debounces switch interrupts and sets a key press
*                 flag. 
*******************************************************************************
* History : DD.MMM.YYYY     Version     Description
*         : 08.Oct.2010     1.00        First release
*         : 02.Dec.2010     1.10        Second YRDK release
*******************************************************************************/

/******************************************************************************
* Project Includes
******************************************************************************/
/* CGC RPDL function definitions */
#include "r_pdl_intc.h"
/* CMT RPDL function definitions */
#include "r_pdl_cmt.h"
/* General RPDL function definitions */
#include "r_pdl_definitions.h"
/* Switch handler function definitions */
#include "switch.h"

/******************************************************************************
* Global Variables
******************************************************************************/
/* Switch flag global variable */
volatile uint8_t gSwitchFlag = 0x00;
/* Switch standby ready global variable */
volatile bool gSwitchStandbyReady = true;
/* Switch press callback pointer declaration */
void (*gSwitchPressCallbackFunc)(void) = NULL;
/* Switch release callback pointer declaration */
void (*gSwitchReleaseCallbackFunc)(void) = NULL;

/******************************************************************************
* Local Function Prototypes
******************************************************************************/
/* Switch 1 interrupt callback function prototype */
void SW1_InterruptCB(void);
/* Switch 2 interrupt callback function prototype */
void SW2_InterruptCB(void);
/* Switch 3 interrupt callback function prototype */
void SW3_InterruptCB(void);
/* Switch debounce callback function prototype */
void SwitchDebounceCB(void);

/******************************************************************************
* Outline       : InitialiseSwitchInterrupts
* Description   : Configures the IRQ pins connected to switches SW1-SW3 to detect
*                 switch presses, and invoke an ISR.
* Argument      : none
* Return value  : none
******************************************************************************/
void InitialiseSwitchInterrupts(void)
{
    /* Declare error flag */
    bool err = true;

    /* Reset the switch flag */
    gSwitchFlag = 0x00;

    /* Reset the switch standby ready flag */
    gSwitchStandbyReady = true;

    /* Configure SW1 interrupt */
    err &=  R_INTC_CreateExtInterrupt
            (
            PDL_INTC_IRQ8,
            PDL_INTC_FALLING,
            SW1_InterruptCB,
            7
            );

    /* Configure SW2 interrupt */
    err &=   R_INTC_CreateExtInterrupt
             (
             PDL_INTC_IRQ9,
             PDL_INTC_FALLING, 
             SW2_InterruptCB,
             7
             );

    /* Configure SW3 interrupt */
    err &=  R_INTC_CreateExtInterrupt
            (
            PDL_INTC_IRQ10,
            PDL_INTC_FALLING, 
            SW3_InterruptCB,
            7
            );      

    /* Halt in while loop when RPDL errors detected */  
    while (!err);
}
/******************************************************************************
* End of InitialiseSwitchInterrupts
******************************************************************************/

/******************************************************************************
* Outline       : SetSwitchPressCallback
* Description   : Takes a pointer to a function, and sets it as the callback
*                 function for the switch interrupts. The passed function is
*                 executed everytime any switch is pressed down.
* Argument      : pointer to callback function (set to NULL to disable)
* Return value  : none
******************************************************************************/
void SetSwitchPressCallback(void(*callback)(void))
{
    /* Store the callback function pointer into the global variable */
    gSwitchPressCallbackFunc = callback;
}
/******************************************************************************
* End of SetSwitchPressCallback
******************************************************************************/

/******************************************************************************
* Outline       : SetSwitchReleaseCallback
* Description   : Takes a pointer to a function, and sets it as the callback
*                 function for the switch interrupts. The passed function is
*                 executed everytime any switch is released.
* Argument      : pointer to callback function (set to NULL to disable)
* Return value  : none
******************************************************************************/
void SetSwitchReleaseCallback(void(*callback)(void))
{
    /* Store the callback function pointer into the global variable */
    gSwitchReleaseCallbackFunc = callback;
}
/******************************************************************************
* End of SetSwitchReleaseCallback
******************************************************************************/

/******************************************************************************
* Outline       : ControlSwitchInterrupts
* Description   : Enables or disables the switch IRQ interrupts, based on the
*                 input variable, control.
* Argument      : bool - true  : Enables switch interrupts.
*                 false : Disables switch interrupts.
* Return value  : none
******************************************************************************/
void ControlSwitchInterrupts(bool control)
{
    /* Declare error flag */
    bool err = true;

    /* Check if control input is true */
    if (control)
    {
        /* Enable SW1 interrupts */
        err &=   R_INTC_ControlExtInterrupt
                 (
                 PDL_INTC_IRQ8,
                 PDL_INTC_ENABLE | PDL_INTC_FALLING
                 );

        /* Enable SW2 interrupts */
        err &=  R_INTC_ControlExtInterrupt
                (
                PDL_INTC_IRQ9,
                PDL_INTC_ENABLE | PDL_INTC_FALLING
                );

        /* Enable SW3 interrupts */
        err &=  R_INTC_ControlExtInterrupt
                (
                PDL_INTC_IRQ10,
                PDL_INTC_ENABLE | PDL_INTC_FALLING
                );
    }
    /* Control input is false */
    else
    {
        /* Disable SW1 interrupts */
        err &=  R_INTC_ControlExtInterrupt
                (
                PDL_INTC_IRQ8,
                PDL_INTC_DISABLE
                );

        /* Disable SW2 interrupts */
        err &=  R_INTC_ControlExtInterrupt
                (
                PDL_INTC_IRQ9,
                PDL_INTC_DISABLE
                );

        /* Disable SW3 interrupts */
        err &=  R_INTC_ControlExtInterrupt
                (
                PDL_INTC_IRQ10,
                PDL_INTC_DISABLE
                );
    }

    /* Halt in while loop when RPDL errors detected */  
    while (!err);  
}  
/******************************************************************************
* End of function ControlSwitchInterrupts
******************************************************************************/  

/******************************************************************************
* Outline       : SW1_InterruptCB
* Description   : Switch 1 callback ISR function. The function disables switch
*                 interrupts, then uses a oneshot timer to re-enable them after
*                 a 10ms debounce timer period.
* Argument      : none
* Return value  : none
******************************************************************************/
void SW1_InterruptCB(void)
{
    /* Declare error flag */
    bool err = true;

    /* Declare status container variable */
    uint8_t status = 0x00;

    /* Fetch input edge status */
    err &=  R_INTC_GetExtInterruptStatus
            (
            PDL_INTC_IRQ8,
            &status
            );

    /* Check if interrupt was triggered from falling edge */
    if ((status & 0x06) == 0x04)
    {
        /* Disable switch interrupts and change to rising edge detection */
        err &=  R_INTC_ControlExtInterrupt
                (
                PDL_INTC_IRQ8,
                PDL_INTC_DISABLE | PDL_INTC_RISING
                );

        /* Set global switch flag to indicate SW1 is held down */
        gSwitchFlag |= 0x08;

        /* Check if switch press callback function is not NULL */
        if (gSwitchPressCallbackFunc)
        {
            /* Execute user callback function */
            gSwitchPressCallbackFunc();
        }
    }

    /* Interrupt was triggered from a rising edge */
    else
    {
        /* Disable switch interrupts and change to falling edge detection */
        err &=  R_INTC_ControlExtInterrupt
                (
                PDL_INTC_IRQ8,
                PDL_INTC_DISABLE | PDL_INTC_FALLING
                );

        /* Clear SW1 held-down flag bit in switch flag */
        gSwitchFlag &= 0xF7;

        /* Set global switch flag to indicate SW1 press complete */
        gSwitchFlag |= 0x80;

        /* Check if switch release callback function is not NULL */
        if (gSwitchReleaseCallbackFunc)
        {
            /* Execute user callback function */
            gSwitchReleaseCallbackFunc();
        }
    }  

    /* Set standby ready flag as false */
    gSwitchStandbyReady = false;

    /* Create 10ms one-shot timer interrupt to debounce the switch input */
    err &=  R_CMT_CreateOneShot
            (
            1,
            PDL_NO_DATA,
            10E-3,
            SwitchDebounceCB,
            8
            );

    /* Halt in while loop when RPDL errors detected */  
    while (!err);
}
/******************************************************************************
* End of function SW1_InterruptCB
******************************************************************************/  

/******************************************************************************
* Outline       : SW2_InterruptCB
* Description   : Switch 2 callback ISR function. The function disables switch
*                 interrupts, then uses a oneshot timer to re-enable them after
*                 a 10ms debounce timer period.
* Argument      : none
* Return value  : none
******************************************************************************/
void SW2_InterruptCB(void)
{
    /* Declare error flag */
    bool err = true;

    /* Declare status container variable */
    uint8_t status = 0x00;

    /* Fetch input edge status */
    err &=  R_INTC_GetExtInterruptStatus
            (
            PDL_INTC_IRQ9,
            &status
            );

    /* Check if interrupt was triggered from falling edge */
    if ((status & 0x06) == 0x04)
    {
        /* Disable switch interrupts and change to rising edge detection */
        err &=  R_INTC_ControlExtInterrupt
                (
                PDL_INTC_IRQ9,
                PDL_INTC_DISABLE | PDL_INTC_RISING
                );

        /* Set global switch flag to indicate SW2 is held down */
        gSwitchFlag |= 0x04;

        /* Check if switch press callback function is not NULL */
        if (gSwitchPressCallbackFunc)
        {
            /* Execute user callback function */
            gSwitchPressCallbackFunc();
        }
    }
    else
    {
        /* Disable switch interrupts and change to falling edge detection */
        err &=  R_INTC_ControlExtInterrupt
                (
                PDL_INTC_IRQ9,
                PDL_INTC_DISABLE | PDL_INTC_FALLING
                );

        /* Clear SW2 held-down bit in switch flag */
        gSwitchFlag &= 0xFB;

        /* Set global switch flag to indicate SW2 press complete */
        gSwitchFlag |= 0x40;

        /* Check if switch release callback function is not NULL */
        if (gSwitchReleaseCallbackFunc)
        {
            /* Execute user callback function */
            gSwitchReleaseCallbackFunc();
        }
    }  

    /* Set standby ready flag as false */
    gSwitchStandbyReady = false;

    /* Create 10ms one-shot timer interrupt to debounce the switch input */
    err &=  R_CMT_CreateOneShot
            (
            1,
            PDL_NO_DATA,
            10E-3,
            SwitchDebounceCB,
            8
            );

    /* Halt in while loop when RPDL errors detected */  
    while (!err);
}
/******************************************************************************
* End of function SW2_InterruptCB
******************************************************************************/  

/******************************************************************************
* Outline       : SW3_InterruptCB
* Description   : Switch 3 callback ISR function. The function disables switch
*                 interrupts, then uses a oneshot timer to re-enable them after
*                 a 10ms debounce timer period.
* Argument      : none
* Return value  : none
******************************************************************************/
void SW3_InterruptCB(void)
{
    /* Declare error flag */
    bool err = true;

    /* Declare status container variable */
    uint8_t status = 0x00;

    /* Fetch input edge status */
    err &=  R_INTC_GetExtInterruptStatus
            (
            PDL_INTC_IRQ10,
            &status
            );

    /* Check if interrupt was triggered from falling edge */
    if ((status & 0x06) == 0x04)
    {
        /* Disable switch interrupts and change to rising edge detection */
        err &=  R_INTC_ControlExtInterrupt
                (
                PDL_INTC_IRQ10,
                PDL_INTC_DISABLE|PDL_INTC_RISING
                );

        /* Set global switch flag to indicate SW3 is held down */
        gSwitchFlag |= 0x02;

        /* Check if switch press callback function is not NULL */
        if (gSwitchPressCallbackFunc)
        {
            /* Execute user callback function */
            gSwitchPressCallbackFunc();
        }
    }

    else
    {
        /* Switch to falling edge detection */
        err &=  R_INTC_ControlExtInterrupt
                (
                PDL_INTC_IRQ10,
                PDL_INTC_FALLING
                );

        /* Clear SW3 held-down bit in switch flag */
        gSwitchFlag &= 0xFD;

        /* Set global switch flag to indicate SW3 press complete */
        gSwitchFlag |= 0x20;

        /* Check if switch release callback function is not NULL */
        if (gSwitchReleaseCallbackFunc)
        {
            /* Execute user callback function */
            gSwitchReleaseCallbackFunc();
        }
    }

    /* Set standby ready flag as false */
    gSwitchStandbyReady = false;

    /* Create 10ms one-shot timer interrupt to debounce the switch input */
    err &=  R_CMT_CreateOneShot
            (
            1,
            PDL_NO_DATA,
            10E-3,
            SwitchDebounceCB,
            8
            );

    /* Halt in while loop when RPDL errors detected */  
    while (!err);
}
/******************************************************************************
* End of function SW3_InterruptCB
******************************************************************************/  

/******************************************************************************
* Outline       : SwitchDebounceCB
* Description   : Oneshot timer callback function. This function is triggered
*                 by the timer interrupt, and re-enables switch interrupts.
* Argument      : none
* Return value  : none
******************************************************************************/
void SwitchDebounceCB(void)
{
    /* Declare error flag */
    bool err = true;

    /* Check if any switches are held down */
    if (!(gSwitchFlag & SWITCHHOLD_ALL))
    {
        /* Set standby ready flag to true */
        gSwitchStandbyReady = true;
    }

    /* Re-enable switch 1 interrupts */
    err &=  R_INTC_ControlExtInterrupt
            (
            PDL_INTC_IRQ8,
            PDL_INTC_ENABLE | PDL_INTC_CLEAR_IR_FLAG 
            );

    /* Re-enable switch 2 interrupts */
    err &=  R_INTC_ControlExtInterrupt
            (
            PDL_INTC_IRQ9,
            PDL_INTC_ENABLE | PDL_INTC_CLEAR_IR_FLAG 
            );

    /* Re-enable switch 3 interrupts */
    err &=  R_INTC_ControlExtInterrupt
            (
            PDL_INTC_IRQ10,
            PDL_INTC_ENABLE | PDL_INTC_CLEAR_IR_FLAG 
            );

    /* Halt in while loop when RPDL errors detected */  
    while (!err);
}
/******************************************************************************
* End of function SwitchDebounceCB
******************************************************************************/
