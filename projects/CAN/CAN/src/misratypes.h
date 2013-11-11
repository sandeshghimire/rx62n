#ifndef _MISRA_H_INCLUDED_
#define _MISRA_H_INCLUDED_
/**************************************************************************************
 DISCLAIMER	: We (Renesas Electronics Europe Limited) do NOT warrant that the Software
			  is free from claims by a third party of copyright, patent, trademark,
			  trade secret or any other intellectual property infringement.

			  Under no circumstances are we liable for any of the following:

			  1. third-party claims against you for losses or damages;
			  2. loss of, or damage to, your records or data; or
			  3. economic consequential damages (including lost profits or savings)
			  	 or incidental damages, even if we are informed of their possibility.

			  We do not warrant uninterrupted or error free operation of the Software.
			  We have no obligation to provide service, defect, correction, or any
			  maintenance for the Software. We have no obligation to supply any
			  Software updates or enhancements to you even if such are or later become
			  available.

			  IF YOU DOWNLOAD OR USE THIS SOFTWARE YOU AGREE TO THESE TERMS.

			  THERE ARE NO WARRANTIES, EXPRESS OR IMPLIED, INCLUDING THE IMPLIED
			  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.

FILE NAME  	: MisraTypes.h

DESCRIPTION	: Data Types for MISRA compliant C code

***********************************************************************************/

/***********************************************************************************
Revision History
DD.MM.YYYY OSO-UID Description
18.04.2007 RTE-NPS First Release
***********************************************************************************/

/***********************************************************************************
 System Includes
***********************************************************************************/
/***********************************************************************************
 User Includes
***********************************************************************************/
/***********************************************************************************
 Defines
***********************************************************************************/
/***********************************************************************************
 Constant Macros
***********************************************************************************/
/***********************************************************************************
 Function Macros
***********************************************************************************/
/***********************************************************************************
 Typedefs
***********************************************************************************/
typedef          char		char_t;
typedef unsigned char		uint8_t;
typedef unsigned short int  uint16_t;
typedef unsigned long int   uint32_t;
typedef signed   char		int8_t;
typedef signed   short int	int16_t;
typedef signed   long int	int32_t;
typedef          float		float32_t;
typedef          double		float64_t;


/***********************************************************************************
 Enumerated Types
***********************************************************************************/
#ifdef TRUE
#undef TRUE
#endif

#ifdef FALSE
#undef FALSE
#endif

typedef enum bool {
	FALSE = 0,
	TRUE = 1
} bool_t;

/***********************************************************************************
 Function Prototypes
***********************************************************************************/

#endif /* _MISRATYPES_H_INCLUDED_ */
