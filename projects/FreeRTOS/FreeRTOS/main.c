/*******************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only 
* intended for use with Renesas products. No other uses are authorized. This 
* software is owned by Renesas Electronics Corporation and is protected under
* all applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT
* LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE 
* AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.
* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS 
* ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE 
* FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR
* ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE
* BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software
* and to discontinue the availability of this software. By using this software,
* you agree to the additional terms and conditions found by accessing the 
* following link:
* http://www.renesas.com/disclaimer
*******************************************************************************/
/*******************************************************************************
* Copyright (C) 2011 Renesas Electronics Corporation. All rights reserved.    */ 
/*******************************************************************************
* File Name    : main.c
* Version      : 1.00
* Device       : R5F562N8
* Tool Chain   : RX Family C Compiler
* OS           : FreeRTOS
* H/W Platform : YRDKRX62N
* Description  : A very simple demonstration of FreeRTOS tasks interacting with
*                the RDK LCD and two LEDs.
*                Creates two tasks and one queue.  One task (the queue receive
*                task) blocks on the queue to wait for data to arrive, toggling
*                an LED each time '100' is received.  The other task (the queue
*                send task) repeatedly blocks for a fixed period then toggles
*                its red LED, LED7, then sends '100' to the queue causing the
*                second task to run and toggle the LED8, the green LED. 
* Operation     : 1. Build and download the sample code to the YRDKRX62N
*
*                 2. Click 'Reset Go' to start the software.
*                 
*                 3. A brief explanation of the demonstration will be displayed
*                    on the LCD.
*                    
*                 4. Two LEDs blink, one red, LED7, and one green, LED8. The
*                    blink rate is different for the LEDs.
*
*******************************************************************************/
/*******************************************************************************
* History	   : 06.Jan.2011 Ver. 1.00 First Release
*******************************************************************************/

/* Hardware specific includes. */
#include "YRDKRX62N.h"
#include "r_pdl_io_port.h"
#include "iodefine.h"

/* Kernel includes. */
#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

/* Demo includes. */
#include "lcd.h"

// RPDL Includes
/* IO Port RPDL function definitions */
#include "r_pdl_io_port.h"
/* General RPDL function definitions */
#include "r_pdl_definitions.h"

/* Priorities at which the tasks are created. */
#define configQUEUE_RECEIVE_TASK_PRIORITY	( tskIDLE_PRIORITY + 1 )
#define	configQUEUE_SEND_TASK_PRIORITY		( tskIDLE_PRIORITY + 2 )

/* The rate at which data is sent to the queue, specified in milliseconds. */
#define mainQUEUE_SEND_FREQUENCY_MS			( 500 / portTICK_RATE_MS )

/* The number of items the queue can hold.  This is 1 as the receive task
will remove items as they are added so the send task should always find the
queue empty. */
#define mainQUEUE_LENGTH					( 1 )

/*
 * The tasks as defined later in this file.
 */
static void prvQueueReceiveTask( void *pvParameters );
static void prvQueueSendTask( void *pvParameters );

/* The queue used by both tasks. */
static xQueueHandle xQueue = NULL;

/*******************************************************************************
* Outline      : main
* Description  : Main program function
* Argument     : none
* Return value : none
*******************************************************************************/
void main(void)
{
    /* Initialize the SPI bus for the LCD */
	YRDKRX62N_RSPI_Init(RSPI_CHANNEL_0);
	InitialiseLCD();

	/* Set the initial state of the LED to off */
	R_IO_PORT_Write( LED7, LED_OFF );
	R_IO_PORT_Write( LED8, LED_OFF );
	
	/* Display description of this demo on the LCD */
    DisplayLCD(LCD_LINE1, "RX FreeRTOS");
	DisplayLCD(LCD_LINE2, "Run 2 Tasks,");
	DisplayLCD(LCD_LINE3, "ea. w/a LED");
	DisplayLCD(LCD_LINE4, "IPC w/Queue");
	DisplayLCD(LCD_LINE5, " ");
	DisplayLCD(LCD_LINE6, "LEDs blink");
	DisplayLCD(LCD_LINE7, "@ rate their");
	DisplayLCD(LCD_LINE8, "task runs");

	/* Create the queue. */
	xQueue = xQueueCreate( mainQUEUE_LENGTH, sizeof( unsigned long ) );

    /* If the queue is successfully created continue with tasks that use it */
	if( xQueue != NULL )
	{
		/* Create the two tasks as described at the top of this file. */
		xTaskCreate( prvQueueReceiveTask, "Rx", configMINIMAL_STACK_SIZE, NULL, configQUEUE_RECEIVE_TASK_PRIORITY, NULL );
		xTaskCreate( prvQueueSendTask,    "TX", configMINIMAL_STACK_SIZE, NULL, configQUEUE_SEND_TASK_PRIORITY,    NULL );

		/* Start the tasks running. */
		vTaskStartScheduler();
	}
	
	/* If all is well we will never reach here as the scheduler will now be
	running.  If we do reach here then it is likely that there was insufficient
	heap available for the idle task to be created. */
   	DisplayLCD(LCD_LINE6, "** ERROR **");
	DisplayLCD(LCD_LINE7, "Scheduler");
	DisplayLCD(LCD_LINE8, "failed");    
	while (1) ;	
}

/*******************************************************************************
* Outline      : prvQueueSendTask
* Description  : The task that sends data through the queue
* Argument     : pvParameters - Documented by FreeRTOS task 
* Return value : none
*******************************************************************************/
static void prvQueueSendTask( void *pvParameters )
{
portTickType xNextWakeTime;
const unsigned long ulValueToSend = 100UL;
int loopCounter  = 0 ;

	/* Initialise xNextWakeTime - this only needs to be done once. */
	xNextWakeTime = xTaskGetTickCount();

	while(1)
	{
		/* Place this task in the blocked state until it is time to run again. 
		The block state is specified in ticks, the constant used converts ticks
		to ms. */
		vTaskDelayUntil( &xNextWakeTime, mainQUEUE_SEND_FREQUENCY_MS );

		R_IO_PORT_Modify(LED7, PDL_IO_PORT_XOR, 1) ;

		if (++loopCounter == 10)
		{
			loopCounter = 0 ;
		    /* Send to the queue - causing the queue receive task to flash its LED.  0
	       is used so the send does not block - it shouldn't need to as the queue
	       should always be empty here. */
		    xQueueSend( xQueue, &ulValueToSend, 0 );
		}
	}
}
/*-----------------------------------------------------------*/

/*******************************************************************************
* Outline      : prvQueueReceiveTask
* Description  : The task that receives data through the queue
* Argument     : pvParameters - Documented by FreeRTOS task 
* Return value : none
*******************************************************************************/
static void prvQueueReceiveTask( void *pvParameters )
{
unsigned long ulReceivedValue;

	while(1)
	{
		/* Wait until something arrives in the queue - this will block 
		indefinitely provided INCLUDE_vTaskSuspend is set to 1 in
		FreeRTOSConfig.h. */
		xQueueReceive( xQueue, &ulReceivedValue, portMAX_DELAY );

		/*  To get here something must have arrived, but is it the expected
		value?  If it is, toggle this task's the LED. */
		if( ulReceivedValue == 100UL )
		{
			R_IO_PORT_Modify(LED8, PDL_IO_PORT_XOR, 1);
		}
	}
}

/*******************************************************************************
* Outline      : vApplicationSetupTimerInterrupt
* Description  : Setup the Tick timer for the OS - A requirement for FreeRTOS
* Argument     : none 
* Return value : none
*******************************************************************************/
void vApplicationSetupTimerInterrupt( void )
{
	/* Enable compare match timer 0. */
	MSTP( CMT0 ) = 0;
	
	/* Interrupt on compare match. */
	CMT0.CMCR.BIT.CMIE = 1;
	
	/* Set the compare match value. */
	CMT0.CMCOR = ( unsigned short ) ( ( ( configPERIPHERAL_CLOCK_HZ / configTICK_RATE_HZ ) - 1 ) / 8 );
	
	/* Divide the PCLK by 8. */
	CMT0.CMCR.BIT.CKS = 0;
	
	/* Enable the interrupt... */
	_IEN( _CMT0_CMI0 ) = 1;
	
	/* ...and set its priority to the application defined kernel priority. */
	_IPR( _CMT0_CMI0 ) = configKERNEL_INTERRUPT_PRIORITY;
	
	/* Start the timer. */
	CMT.CMSTR0.BIT.STR0 = 1;
}
/*-----------------------------------------------------------*/


/*******************************************************************************
* Outline      : vApplicationMallocFailedHook
* Description  : Hook function for Malloc failures - A requirement for FreeRTOS
* Argument     : none 
* Return value : none
*******************************************************************************/
void vApplicationMallocFailedHook( void )
{
	for( ;; );
}
/*-----------------------------------------------------------*/

/*******************************************************************************
* Outline      : vApplicationStackOverflowHook
* Description  : Hook function for Stack Overflow failures - A requirement for
*                FreeRTOS
* Argument     : xTaskHandle pointer to the task, signed char * to the task name
*                as documented in FreeRTOS
* Return value : none
*******************************************************************************/
void vApplicationStackOverflowHook( xTaskHandle *pxTask, signed char *pcTaskName )
{
	for( ;; );
}
/*-----------------------------------------------------------*/

/*******************************************************************************
* End of file main.c
*******************************************************************************/

