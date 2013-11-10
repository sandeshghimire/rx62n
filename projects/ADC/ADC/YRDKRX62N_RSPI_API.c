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
* File Name     : YRDKRX62N_RSPI_API.C
* Version       : 1.00
* Device(s)     : R5F562N8
* Tool-Chain    : Renesas RX Standard Toolchain 1.0.1
* OS            : None
* H/W Platform  : YRDKRX62N
* Description   : Functions for using RSPI devices on the YRDKRX62N
*******************************************************************************
* History : DD.MMM.YYYY     Version     Description
*         : 02.Dec.2010     1.00        First release
*******************************************************************************/

/**********************************************************************************
System Includes
***********************************************************************************/
#include <stdint.h>

/**********************************************************************************
User Includes
***********************************************************************************/

/* IO Port RPDL function definitions */
#include "r_pdl_io_port.h"

/* SCI RPDL function definitions */
#include "r_pdl_spi.h"

/* General RPDL function definitions */
#include "r_pdl_definitions.h"

/* Defines for the YRDKRX62N hardware */
#include "YRDKRX62N.h"

/* Defines for YRDKRX62N RSPI support */
#include "YRDKRX62N_RSPI_API.h"


static uint16_t YRDKRX62N_RSPI_Access_Locked_By = NO_DEVICE_SELECTED; // use spi_cs enum for pids

/*””FUNC COMMENT””*************************************************************
* Function Name: YRDKRX62N_RSPI_Select
* Description  : Enable device chip select on SPI channel
* Arguments    : uint16_t SPI chip_select defined in rdkrx62n.h
* Return Value : none
*””FUNC COMMENT END””*********************************************************/
void YRDKRX62N_RSPI_Select(uint16_t chip_select)
{
    uint16_t    rspi_cs;

    switch ( chip_select)
    {
    case SDMICRO_SELECTED:          /* Enable the SD card */
        rspi_cs = SDMICRO_CS;
        break;
    case FLASH_SELECTED:            /* Enable Micron flash */
        rspi_cs = FLASH_CS;    
        break;
    case WIFI_SELECTED:             /* Enable Redpine WiFi card */
        rspi_cs = WIFI_CS;    
        break;
    case LCD_SELECTED:              /* Enable Okaya display */
        rspi_cs = LCD_CS;    
        break;
    default:
        return;
    }     

    /* Assert slave select */
    R_IO_PORT_Write(rspi_cs, 0);
}
/******************************************************************************
End of function YRDKRX62N_RSPI_Select
******************************************************************************/


/*””FUNC COMMENT””*************************************************************
* Function Name: YRDKRX62N_RSPI_Deselect
* Description  : Disables device chip select
* Arguments    : uint16_t SPI chip_select defined in rdkrx62n.h
* Return Value : none
*””FUNC COMMENT END””*********************************************************/
void YRDKRX62N_RSPI_Deselect(uint16_t chip_select)
{
    uint16_t    rspi_cs;

    switch ( chip_select)
    {
    case SDMICRO_SELECTED:          /* Disable the SD card */
        rspi_cs = SDMICRO_CS;
        break;
    case FLASH_SELECTED:            /* Disable the Micron flash */
        rspi_cs = FLASH_CS;    
        break;
    case WIFI_SELECTED:             /* Disable the Redpine WiFi card */
        rspi_cs = WIFI_CS;    
        break;
    case LCD_SELECTED:              /* Disable the Okaya display */
        rspi_cs = LCD_CS;    
        break;
    default:
        return;
    }     

    /* Disable slave select */
    R_IO_PORT_Write(rspi_cs, 1);
}
/******************************************************************************
End of function YRDKRX62N_RSPI_Deselect
******************************************************************************/


/*””FUNC COMMENT””*************************************************************
* Function Name: YRDKRX62N_RSPI_Init
* Description  : Initializes SPI channel
* Arguments    : uint8_t RSPI channel number, 0 or 1
* Return Value : none
*””FUNC COMMENT END””*********************************************************/
void YRDKRX62N_RSPI_Init(uint8_t rspi_channel)
{
    bool        err = true;
    uint32_t    pins;

    if ( rspi_channel == RSPI_CHANNEL_0 )
    {
        pins = PDL_SPI_PIN_A;
    }
    else if ( rspi_channel == RSPI_CHANNEL_1 )
    {
        pins = PDL_SPI_PIN_B;
    }
    else
    {
        err &= false;    
    }


    /* Initialize SPI channel 1 in SPI master mode, on the 'A' pins,  
       keep all SSLs disabled */
    err &= R_SPI_Create( rspi_channel,
                         PDL_SPI_FULL_DUPLEX | PDL_SPI_MODE_SPI_MASTER | pins,
                         PDL_SPI_FRAME_1_1,
                         PDL_NO_DATA,
                         0x80000000 );

    /* LCD Configure command 0 as using SSL1, 8 bit frame length, MSB first */
    // NOTE:  SLL chip selects will be managed by the individual SPI device drivers
    //        All devices on this channel will be sending only 8 bits at a time.
    err &= R_SPI_Command( rspi_channel,
                          0,
                          PDL_SPI_CLOCK_MODE_0 | PDL_SPI_ASSERT_SSL1 | PDL_SPI_LENGTH_8 | 
                          PDL_SPI_MSB_FIRST,
                          PDL_NO_DATA );

    /* Halt in while loop when RPDL errors detected */    
    while (!err);

}
/******************************************************************************
End of function YRDKRX62N_RSPI_Init
******************************************************************************/



/*””FUNC COMMENT””*************************************************************
* Function Name: YRDKRX62N_RSPI_Read
* Description  : Performs SPI transfers
* Arguments    : pDest - Pointer to location to put the received data 
*                        Returned value will be incremented by number
*                        of bytes received.
*                usBytes - number of bytes to be received
* Return Value : bool     true, success
*                       false, failure
*””FUNC COMMENT END””*********************************************************/
bool YRDKRX62N_RSPI_Read(uint8_t *pDest, uint16_t usBytes )
{
    bool ret = true;

    if (pDest != NULL)
    {
        /* read only */
        while (usBytes-- != 0)
        {
            ret &= R_SPI_Transfer( RSPI_CHANNEL_0,
                                   PDL_SPI_DMAC_DTC_TRIGGER_DISABLE,
                                   (uint32_t *) PDL_NO_PTR, 
								   (uint32_t *) pDest,
                                   1,
                                   PDL_NO_FUNC,
                                   0 );    
			pDest++;                    
        }                                                        
    }
    else
    {
        ret = false;
    }

    return ret;
}
/******************************************************************************
End of function YRDKRX62N_RSPI_Read
******************************************************************************/

/*””FUNC COMMENT””*************************************************************
* Function Name: YRDKRX62N_RSPI_Write
* Description  : Write to an SPI device
* Arguments    : pSrc -  Pointer to data buffer with data to be transmitted.
*                        Returned value will be incremented by number of attempted
*                        writes.
*                usBytes - number of bytes to be sent
* Return Value : bool     true, success
*                       false, failure
*””FUNC COMMENT END””*********************************************************/
bool YRDKRX62N_RSPI_Write(uint8_t *pSrc, uint16_t usBytes )
{
    bool ret = true;

    if (pSrc != NULL)
    {
        while (usBytes-- != 0)
        {
            ret &= R_SPI_Transfer( RSPI_CHANNEL_0,
                                   PDL_SPI_DMAC_DTC_TRIGGER_DISABLE,
                                   (uint32_t *) pSrc,     
								   (uint32_t *) PDL_NO_PTR,
                                   1,
                                   PDL_NO_FUNC,
                                   0 );
			pSrc++;
        }                
    }
    else
    {
        ret = false;
    }

    return ret;
}
/******************************************************************************
End of function YRDKRX62N_RSPI_Write
******************************************************************************/

/*””FUNC COMMENT””*************************************************************
* Function Name: YRDKRX62N_RSPI_Lock
* Description  : Get lock on RSPI access
* Arguments    : uint16_t    pid - unique program ID
* Return Value : bool - true, lock acquired
*                       false, lock not acquired
*””FUNC COMMENT END””*********************************************************/
bool YRDKRX62N_RSPI_Lock(uint16_t pid)
{
    bool ret = true;

    if ( YRDKRX62N_RSPI_Access_Locked_By == NO_DEVICE_SELECTED )
    {
        YRDKRX62N_RSPI_Access_Locked_By = pid;
    }
    else
    {
        ret = false;
    }

    return ret;


}
/******************************************************************************
End of function YRDKRX62N_RSPI_Lock
******************************************************************************/

/*””FUNC COMMENT””*************************************************************
* Function Name: YRDKRX62N_RSPI_Unlock
* Description  : Unlock RSPI access
* Arguments    : uint16_t pid -- pid that locked access.
* Return Value : true -- correct pid
*                false -- wrong pid     
*””FUNC COMMENT END””*********************************************************/
bool YRDKRX62N_RSPI_Unlock(uint16_t pid)
{
    bool ret = true;

    if ( YRDKRX62N_RSPI_Access_Locked_By == pid )
    {
        YRDKRX62N_RSPI_Access_Locked_By = NO_DEVICE_SELECTED;
    }
    else
    {
        ret = false;
    }

    return ret;
}
/******************************************************************************
End of function YRDKRX62N_RSPI_Unlock
******************************************************************************/


/*””FUNC COMMENT””*************************************************************
* Function Name: YRDKRX62N_RSPI_Tx_Ready
* Description  : Make sure the transmit register is empty, blocks until it is.
* Arguments    : none
* Return Value : none
*””FUNC COMMENT END””*********************************************************/
void YRDKRX62N_RSPI_TxReady(void)
{
    while (RSPI0.SPSR.BIT.IDLNF)
	{
	}
}
/******************************************************************************
End of function YRDKRX62N_RSPI_Tx_Ready
******************************************************************************/