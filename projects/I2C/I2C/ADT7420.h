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
* File Name     : ADT7420.h
* Version       : 1.0
* Device(s)     : R5F562N8
* Tool-Chain    : Renesas RX Standard Toolchain 1.0.1
* OS            : None
* H/W Platform  : YRDKRX62N
* Description   : Declarations for ADT7420 temperature sensor.
*******************************************************************************
* History : DD.MMM.YYYY     Version     Description
*         : 02.Dec.2010     1.00        First release
*******************************************************************************/

/*******************************************************************************
* Project Includes
*******************************************************************************/
#include <stdint.h>
#include <stdbool.h>

/*******************************************************************************
* Macro Definitions
*******************************************************************************/
/* Multiple inclusion prevention macro */
#ifndef IIC_ADT7420_H
#define IIC_ADT7420_H

#define ADT7420_ADDR                0x90
#define ADT7420_DEVICE_ID           0xC8    // bits 7:3 of mfg identification number

#define ADT7420_TEMP_MSB_REG        0x00
#define ADT7420_TEMP_LSB_REG        0x01
#define ADT7420_STATUS_REG          0x02
#define ADT7420_CONFIG_REG          0x03
#define ADT7420_T_HIGH_MSB_REG      0x04
#define ADT7420_T_HIGH_LSB_REG      0x05
#define ADT7420_T_LOW_MSB_REG       0x06
#define ADT7420_T_LOW_LSB_REG       0x07
#define ADT7420_T_CRIT_MSB_REG      0x08
#define ADT7420_T_CRIT_LSB_REG      0x09
#define ADT7420_HIST_REG            0x0A
#define ADT7420_ID_REG              0x0B
#define ADT7420_RESET_REG           0x2F

/******************************************************************************
* Global Function Prototypes
******************************************************************************/



/* End of multiple inclusion prevention macro */
#endif
