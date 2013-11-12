/*""FILE COMMENT""*******************************************************
* System Name	: Serial Peripheral Interface API for RX62Nxx
* File Name		: r_pdl_spi_library_data_RX62Nxx.h
* Version		: 1.02
* Contents		: SPI API header
* Customer		:
* Model			:
* Order			:
* CPU			: RX
* Compiler		: RXC
* OS			: Nothing
* Programmer	:
* Note			:
************************************************************************
* Copyright, 2011. Renesas Electronics Corporation
* and Renesas Solutions Corporation
************************************************************************
* History		: 2011.04.08
*				: Ver 1.02
*				: CS-5 release.
*""FILE COMMENT END""**************************************************/

/* Local function prototypes */
void rpdl_SPI_DisableIEN(const uint8_t);
void rpdl_SPI_EnableErrorIEN(const uint8_t);
void rpdl_SPI_EnableIdleIEN(const uint8_t);
void rpdl_SPI_EnableTransferIEN(const uint8_t);

/* Calculations to find the relevant set of register addresses */
#define SPCR_ADDRESS(a)   ( (volatile  uint8_t *)&RSPI0.SPCR   + ((0x20 * a) / sizeof(uint8_t )) )
#define SSLP_ADDRESS(a)   ( (volatile  uint8_t *)&RSPI0.SSLP   + ((0x20 * a) / sizeof(uint8_t )) )
#define SPPCR_ADDRESS(a)  ( (volatile  uint8_t *)&RSPI0.SPPCR  + ((0x20 * a) / sizeof(uint8_t )) )
#define SPSR_ADDRESS(a)   ( (volatile  uint8_t *)&RSPI0.SPSR   + ((0x20 * a) / sizeof(uint8_t )) )
#define SPDRW_ADDRESS(a)  ( (volatile uint16_t *)&RSPI0.SPDR   + ((0x20 * a) / sizeof(uint16_t)) )
#define SPDRL_ADDRESS(a)  ( (volatile uint32_t *)&RSPI0.SPDR   + ((0x20 * a) / sizeof(uint32_t)) )
#define SPSCR_ADDRESS(a)  ( (volatile  uint8_t *)&RSPI0.SPSCR  + ((0x20 * a) / sizeof(uint8_t )) )
#define SPSSR_ADDRESS(a)  ( (volatile  uint8_t *)&RSPI0.SPSSR  + ((0x20 * a) / sizeof(uint8_t )) )
#define SPBR_ADDRESS(a)   ( (volatile  uint8_t *)&RSPI0.SPBR   + ((0x20 * a) / sizeof(uint8_t )) )
#define SPDCR_ADDRESS(a)  ( (volatile  uint8_t *)&RSPI0.SPDCR  + ((0x20 * a) / sizeof(uint8_t )) )
#define SPCKD_ADDRESS(a)  ( (volatile  uint8_t *)&RSPI0.SPCKD  + ((0x20 * a) / sizeof(uint8_t )) )
#define SSLND_ADDRESS(a)  ( (volatile  uint8_t *)&RSPI0.SSLND  + ((0x20 * a) / sizeof(uint8_t )) )
#define SPND_ADDRESS(a)   ( (volatile  uint8_t *)&RSPI0.SPND   + ((0x20 * a) / sizeof(uint8_t )) )
#define SPCR2_ADDRESS(a)  ( (volatile  uint8_t *)&RSPI0.SPCR2  + ((0x20 * a) / sizeof(uint8_t )) )
#define SPCMDn_ADDRESS(a) ( (volatile uint16_t *)&RSPI0.SPCMD0 + ((0x20 * a) / sizeof(uint16_t)) )
#define PFxSPI_ADDRESS(a) \
( \
( (a) == 0 ) ? (volatile uint8_t *)&IOPORT.PFGSPI : \
               (volatile uint8_t *)&IOPORT.PFHSPI \
)
#define	IPR_ADDRESS(a) ( (volatile uint8_t *)&ICU.IPR[IPR_RSPI0_ + a])
#define	SPEI_ADDRESS(a) ( (volatile uint8_t *)&ICU.IR[IR_RSPI0_SPEI0] + ((4 * a) / sizeof(uint8_t)) )
#define	SPRI_ADDRESS(a) ( (volatile uint8_t *)&ICU.IR[IR_RSPI0_SPRI0] + ((4 * a) / sizeof(uint8_t)) )
#define	SPTI_ADDRESS(a) ( (volatile uint8_t *)&ICU.IR[IR_RSPI0_SPTI0] + ((4 * a) / sizeof(uint8_t)) )
#define	SPII_ADDRESS(a) ( (volatile uint8_t *)&ICU.IR[IR_RSPI0_SPII0] + ((4 * a) / sizeof(uint8_t)) )
#define SPRI_DTCER_ADDRESS(a) (	( volatile uint8_t *)&ICU.DTCER[DTCE_RSPI0_SPRI0]+ ((4 * a) / sizeof(uint8_t)) )
#define SPTI_DTCER_ADDRESS(a) (	( volatile uint8_t *)&ICU.DTCER[DTCE_RSPI0_SPTI0]+ ((4 * a) / sizeof(uint8_t)) )

/* End of file */
