/***********************************************************************/
/*                                                                     */
/*  FILE        :intprg.c                                              */
/*  DATE        :Fri, Jun 12, 2009                                     */
/*  DESCRIPTION :Interrupt Program                                     */
/*  CPU TYPE    :Other                                                 */
/*                                                                     */
/*  This file is generated by Renesas Project Generator (Ver.4.13).    */
/*                                                                     */
/***********************************************************************/
                  
#include <machine.h>
#include "vect.h"
#pragma section IntPRG

// Exception(Supervisor Instruction)
void Excep_SuperVisorInst(void){/* brk(); */}

// Exception(Access)
void Excep_Access(void){/* brk(); */}

// Exception(Undefined Instruction)
void Excep_UndefinedInst(void){/* brk(); */}

// Exception(Floating Point)
void Excep_FloatingPoint(void){/* brk(); */}

// NMI
void NonMaskableInterrupt(void){/* brk(); */}

// Dummy
void Dummy(void){/* brk(); */}

// BRK
void Expep_BRK(void){ wait(); }

