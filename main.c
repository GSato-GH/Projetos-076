/* ###################################################################
**     Filename    : main.c
**     Project     : Semaforo.0
**     Processor   : MKL25Z128VLK4
**     Version     : Driver 01.01
**     Compiler    : GNU C Compiler
**     Date/Time   : 2017-03-13, 14:43, # CodeGen: 0
**     Abstract    :
**         Main module.
**         This module contains user's application code.
**     Settings    :
**     Contents    :
**         No public methods
**
** ###################################################################*/
/*!
** @file main.c
** @version 01.01
** @brief
**         Main module.
**         This module contains user's application code.
*/         
/*!
**  @addtogroup main_module main module documentation
**  @{
*/         
/* MODULE main */


/* Including needed modules to compile this module/procedure */
#include "Cpu.h"
#include "Events.h"
#include "greenLED.h"
#include "BitIoLdd1.h"
#include "yellowLED.h"
#include "BitIoLdd2.h"
#include "redLED.h"
#include "BitIoLdd3.h"
#include "TI1.h"
#include "TimerIntLdd1.h"
#include "TU1.h"
#include "greenpassLED.h"
#include "BitIoLdd4.h"
#include "redpassLED.h"
#include "BitIoLdd5.h"
#include "ButtonInt.h"
#include "ExtIntLdd1.h"
#include "AD1.h"
#include "AdcLdd1.h"
/* Including shared modules, which are used for whole project */
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"

/* User includes (#include below this line is not maintained by Processor Expert) */
int estados = 0;
int temp_add = 0;
int counter_night = 0;
int counter_day = 0;
static uint16_t stateaux;
long stateLDR;
/*lint -save  -e970 Disable MISRA rule (6.3) checking. */
int main(void)
/*lint -restore Enable MISRA rule (6.3) checking. */
{
  /* Write your local variable definition here */

  /*** Processor Expert internal initialization. DON'T REMOVE THIS CODE!!! ***/
  PE_low_level_init();
  /*** End of Processor Expert internal initialization.                    ***/

  /* Write your code here */
  /* For example: for(;;) { } */
  for(;;){
	  /*-----------------------Laço infinito da main-----------------------*/
	  // Leitura do LDR a partir do ADC
	  (void)AD1_Measure(TRUE);
	  (void)AD1_GetValue16(&stateaux);
	  stateLDR = stateaux; 			 // Mudança de base - static unint16_t para long
	  if(estados == 0){ 			 // Estado 0 - Inicia o sistema: verde para carros e vermelho para pedestre
		  redLED_ClrVal();
		  yellowLED_ClrVal();
		  greenLED_SetVal();
		  greenpassLED_ClrVal();
		  redpassLED_SetVal();
	  }
	  else if(estados == 1){ 		// Estado 1: acende o amarelo para carros e mantém vermelho para pedestre
		  redLED_ClrVal();
		  greenLED_ClrVal();
		  yellowLED_SetVal();
	  }
	  else if(estados == 2){ 		// Estado 2: acende o vermelho para carros e verde para pedestre
		  greenLED_ClrVal();
		  yellowLED_ClrVal();
		  redLED_SetVal();
		  redpassLED_ClrVal();
		  greenpassLED_SetVal();
	  }
	  else if(estados == 3){ 		// Estado 3: apaga o verde (o vermelho para pedestre irá piscar em Events)
		  greenpassLED_ClrVal();
	  }
	 
  }
  
  /*** Don't write any code pass this line, or it will be deleted during code generation. ***/
  /*** RTOS startup code. Macro PEX_RTOS_START is defined by the RTOS component. DON'T MODIFY THIS CODE!!! ***/
  #ifdef PEX_RTOS_START
    PEX_RTOS_START();                  /* Startup of the selected RTOS. Macro is defined by the RTOS component. */
  #endif
  /*** End of RTOS startup code.  ***/
  /*** Processor Expert end of main routine. DON'T MODIFY THIS CODE!!! ***/
  for(;;){}
  /*** Processor Expert end of main routine. DON'T WRITE CODE BELOW!!! ***/
} /*** End of main routine. DO NOT MODIFY THIS TEXT!!! ***/

/* END main */
/*!
** @}
*/
/*
** ###################################################################
**
**     This file was created by Processor Expert 10.3 [05.09]
**     for the Freescale Kinetis series of microcontrollers.
**
** ###################################################################
*/
