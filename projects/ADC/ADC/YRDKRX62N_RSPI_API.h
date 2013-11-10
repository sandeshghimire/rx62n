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
* File Name     : YRDKRX62N_RSPI_API.h
* Version       : 1.0
* Device(s)     : R5F562N8
* Tool-Chain    : Renesas RX Standard Toolchain 1.0.1
* OS            : None
* H/W Platform  : YRDKRX62N
* Description   : Board specific definitions
* Limitations   : None
*******************************************************************************
* History : DD.MMM.YYYY     Version     Description
*         : 02.Dec.2010     1.00        First release
*******************************************************************************/
#ifndef YRDKRX62N_RSPI_API_HEADER_FILE
#define YRDKRX62N_RSPI_API_HEADER_FILE

// enumeration for chips selects associated with YRDKRX62N RSPI-0 
typedef enum {
    NO_DEVICE_SELECTED=0,
    SDMICRO_SELECTED,
    FLASH_SELECTED,
    WIFI_SELECTED,
    LCD_SELECTED
} device_selected ;


/******************************************************************************
Prototypes for the YRDKRX62N board specific APIs to share the RSPI hardware
******************************************************************************/
void YRDKRX62N_RSPI_Select(uint16_t chip_select);
void YRDKRX62N_RSPI_Deselect(uint16_t chip_select);
void YRDKRX62N_RSPI_Init(uint8_t rspi_channel);
bool YRDKRX62N_RSPI_Read(uint8_t *pDest, uint16_t usBytes);
bool YRDKRX62N_RSPI_Write(uint8_t *pSrc, uint16_t usBytes);
bool YRDKRX62N_RSPI_Lock(uint16_t pid);
bool YRDKRX62N_RSPI_Unlock(uint16_t pid);
void YRDKRX62N_RSPI_TxReady(void);

#endif /* YRDKRX62N_RSPI_API_HEADER_FILE */

