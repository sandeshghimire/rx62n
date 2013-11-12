Renesas RX600 RDK Demonstration of FreeRTOS

The accompanying HEW project is a simplication of the FreeRTOS.org website
demonstration. It is specifically for the RDK and is a simple multi-tasking
example. The FreeRTOS demos are a more thorough exploration of FreeRTOS
performance with Ethernet connectivity and we do encourage you to explore
them after running this demonstration.

To run the demo double click on the .hws file.

You do not need to know the following information to run the demo, it is included
so that when you work with updated revisions of FreeRTOS you'll have some guidance
in the changes required.

Configuration Differences from FreeRTOS Release V6.1.0

1) Use of the Renesas Peripheral Device Library (RPDL)
   a.) RPDL has default handlers for interrupt numbers 27 and 28.
       Disable, with “#if 0…#endif”, the handlers in the RPDL Interrupt_????.c
       files. They are Interrupt_INTC.c for VECT_ICU_SWINT and Interrupt_CMT.c for
       VECT_CMT0_CMI0
   b.) The use of RPDL makes it simple to utilize the LCD on the RDK board.
       There's additional support code in the YRDKRX62N_RSPI_API.c file and the
       accompanying r_pdl_<*>.h files.

2) HEW Project Builder framework.
   a.) HardwareSetup() is called in the startup code differently. From the HEW
       Project Builder the call to HardwareSetup() in resetprg.c whereas in the
       FreeRTOS release it is in main() 
   b.) In resetprg.c the Change_PSW_PM_to_UserMode() function shall not be called
       because the FreeRTOS kernel needs to execute privileged instructions.
       Remove the code, or comment the code or disable the code with a preprocessor
       "#if 0 .... #endif" change.

3) There are many files in the FreeRTOS releases that are not pertinent to running
   on the Rx600 RDK. The other chip architecture's directories have been deleted.
   Files have also been moved to keep the number of levels of directory/folder
   hierarchy to a minimum.

4) Renesas uses the YRDKRX62N.h for defines of the processor whereas the FreeRTOS
   release referenced "yrdkrx62ndef.h" in FreeRTOSConfig.h

5) Files from FreeRTOS source have #error "#include..." which is legal C but are
   indicated as HEW Dependency-preprocessor errors have been modified with a space
   between the '#' and the word include. The files with this change are
   include\croutine.h, include\queue.h, include\semphr.h, and include\task.h.
 
6) portable.h from FreeRTOS was modified such that between
   OPEN_WATCOM_INDUSTRIAL_PC_PORT and _16FX_SOFTUNE are commented out using "/*"
   and "*/". The #ifdef BCC_INDUSTRIAL_PC_PORT was modified so that nested
   commenting won't occur so it was commented in place for it's two #includes. The
   #ifdef BCC_FLASH_LITE_186_PORT change is similar to the BCC_INDUSTRIAL_PC_PORT.
   The __GNUC__ to __IAR_78KOR_Kx3L++ are commented out with /* and */.

