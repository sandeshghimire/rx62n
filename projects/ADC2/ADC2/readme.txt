-------- PROJECT GENERATOR --------
PROJECT NAME :	WDT
PROJECT DIRECTORY :	C:\WorkSpace\RSKRX62N_Draft\Samples\WDT
CPU SERIES :	RX600
CPU TYPE :	RX610
TOOLCHAIN NAME :	Renesas RX Standard Toolchain
TOOLCHAIN VERSION :	1.0.0.0
GENERATION FILES :
    C:\WorkSpace\RSKRX62N_Draft\Samples\WDT\dbsct.c
        Setting of B,R Section
    C:\WorkSpace\RSKRX62N_Draft\Samples\WDT\typedefine.h
        Aliases of Integer Type
    C:\WorkSpace\RSKRX62N_Draft\Samples\WDT\sbrk.c
        Program of sbrk
    C:\WorkSpace\RSKRX62N_Draft\Samples\WDT\iodefine.h
        Definition of I/O Register
    C:\WorkSpace\RSKRX62N_Draft\Samples\WDT\intprg.c
        Interrupt Program
    C:\WorkSpace\RSKRX62N_Draft\Samples\WDT\vecttbl.c
        Initialize of Vector Table
    C:\WorkSpace\RSKRX62N_Draft\Samples\WDT\vect.h
        Definition of Vector
    C:\WorkSpace\RSKRX62N_Draft\Samples\WDT\resetprg.c
        Reset Program
    C:\WorkSpace\RSKRX62N_Draft\Samples\WDT\WDT.c
        Main Program
    C:\WorkSpace\RSKRX62N_Draft\Samples\WDT\sbrk.h
        Header file of sbrk file
    C:\WorkSpace\RSKRX62N_Draft\Samples\WDT\stacksct.h
        Setting of Stack area
START ADDRESS OF SECTION :
 H'1000	B_1,R_1,B_2,R_2,B,R,SU,SI
 H'FFFF8000	PResetPRG
 H'FFFF8100	C_1,C_2,C,C$*,D*,P,PIntPRG,W*
 H'FFFFFFD0	FIXEDVECT

* When the user program is executed,
* the interrupt mask has been masked.
* 
* Program start 0xFFFF8000.
* RAM start 0x1000.

SELECT TARGET :
    RX600 E1/E20 SYSTEM
DATE & TIME : 26/07/2010 16:47:06
