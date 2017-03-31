/* ###################################################################
**     Filename    : Events.c
**     Project     : Semaforo.0
**     Processor   : MKL25Z128VLK4
**     Component   : Events
**     Version     : Driver 01.00
**     Compiler    : GNU C Compiler
**     Date/Time   : 2017-03-13, 14:43, # CodeGen: 0
**     Abstract    :
**         This is user's event module.
**         Put your event handler code here.
**     Settings    :
**     Contents    :
**         Cpu_OnNMIINT - void Cpu_OnNMIINT(void);
**
** ###################################################################*/
/*!
** @file Events.c
** @version 01.00
** @brief
**         This is user's event module.
**         Put your event handler code here.
*/         
/*!
**  @addtogroup Events_module Events module documentation
**  @{
*/         
/* MODULE Events */

#include "Cpu.h"
#include "Events.h"

#ifdef __cplusplus
extern "C" {
#endif 


/* User includes (#include below this line is not maintained by Processor Expert) */

/*
** ===================================================================
**     Event       :  Cpu_OnNMIINT (module Events)
**
**     Component   :  Cpu [MKL25Z128LK4]
*/
/*!
**     @brief
**         This event is called when the Non maskable interrupt had
**         occurred. This event is automatically enabled when the [NMI
**         interrupt] property is set to 'Enabled'.
*/
/* ===================================================================*/
void Cpu_OnNMIINT(void)
{
  /* Write your code here ... */
}

/*
** ===================================================================
**     Event       :  TI1_OnInterrupt (module Events)
**
**     Component   :  TI1 [TimerInt]
**     Description :
**         When a timer interrupt occurs this event is called (only
**         when the component is enabled - <Enable> and the events are
**         enabled - <EnableEvent>). This event is enabled only if a
**         <interrupt service/event> is enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

//Declaração de variáveis externas
extern int estados;
extern int temp_add;
extern int counter_night;
extern int counter_day;
extern long stateLDR; 

void TI1_OnInterrupt(void)
{
  /* Write your code here ... */

	/*-----------------------Mudança de estados a partir de uma interrupção a cada 500ms-----------------------*/
	
	// Observação relevante: os estados 0, 1 e 2 realizam funções similares, mas com tempos diferentes e sequencialmente
	if(estados == 0){					// Estado Zero: Verde para carros e vermelho para pedestre
		temp_add++;						// Incrementa-se a variável de contagem
		if(temp_add == 12){				// Após 6 segundos, entra no loop
			estados = 1;				// Mudança de estado para 1
			temp_add = 0;				// Zera-se o contador para o próximo estado
		}
	}
	if(estados == 1){					// Estado Um: Amarelo para carros e vermelho para pedestre
		temp_add++;						// Incrementa-se a variável de contagem
		if(temp_add == 6){				// Após 3 segundos, entra no loop
			estados = 2;				// Mudança de estado para 2
			temp_add = 0;				
		}
	}
	if(estados == 2){					// Estado Dois: Vermelho para carros e verde para pedestre
		temp_add++;						// Incrementa-se a variável de contagem
		if(temp_add == 12){				// Após 6 segundos, entra no loop
			estados = 3;				// Mudança de estado para 3
			temp_add = 0;				
		}
	}
	if(estados == 3){					// Estado Três: Vermelho para carros e alerta pedestre com vermelho piscante
		temp_add++;
		redpassLED_NegVal();			// Faz o led vermelho para pedestre piscar a cada 500ms 
		if(temp_add == 10){				// Após 5 segundos, entra no loop
			estados = 0;				// Volta para o estado zero reiniciando o ciclo do semáforo
			temp_add = 0;
		}
	}
	
	/*-----------------------Verificação do estado do LDR-----------------------*/
	/* Observação: foi feito um divisor de tensão com um resistor de 10k:
	   Com luz, baixa impedância: tensão no pino cai (próximo de 0.5V)
	   Sem luz, alta impedância: tensão no pino sobe (próximo de 3.3V)
	   Resolução de 16 bits: 2^16-1 = 65.535 */
	if(stateLDR > 53000 && estados != 4){ 	// Se o ADC detectar uma tensão mais alta - (indica LDR no escuro)
		counter_night++;					// Inicia contagem de segurança
		if(counter_night == 10){			// Caso estiver escuro por mais de 5 segundos significa que está "noite"
			estados = 4;					// Mudança de estado para 4
			counter_night = 0;				// Zera o contador para 0
			yellowLED_ClrVal();				// Assegura que os LEDs piscarão simultaneamente: limpa estados dos leds
			redpassLED_ClrVal();			// amarelo e vermelho para pedestre
		}
	}
	
	 if(estados == 4){						// Estado 4: pisca o amarelo para carros e pisca vermelho para pedestre
		 if (stateLDR > 53000){				// Zera o contador de dia
			counter_day = 0; 
		 }
		 if (stateLDR <= 53000){			// Verifica se o LDR detecta luz por mais de 5 segundos
		  	counter_day ++;
		 }
		 if (counter_day >= 10){			// Se sim, volta para o estado zero indicando "dia"
		  	estados = 0;
		  	counter_day = 0;
		  	temp_add = 0;
		 }
		  	redLED_ClrVal();				// Configuração dos LEDs para estado 4
		  	greenLED_ClrVal();
		  	greenpassLED_ClrVal();
		  	redpassLED_NegVal();
		  	yellowLED_NegVal();
		  			
		 }
	
}

/*
** ===================================================================
**     Event       :  ButtonInt_OnInterrupt (module Events)
**
**     Component   :  ButtonInt [ExtInt]
**     Description :
**         This event is called when an active signal edge/level has
**         occurred.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void ButtonInt_OnInterrupt(void)
{
	/*-----------------------Interrupção de alta prioridade-----------------------*/
	if(estados == 0){				// Caso estiver verde para carros, diminui o tempo deste aceso, caso necessário
		if (temp_add < 10) {		
			temp_add = 10;
		}
	}
	else{							// Se tempo já estiver próximo de ser fechado, nada acontece
	}
}

/*
** ===================================================================
**     Event       :  AD1_OnEnd (module Events)
**
**     Component   :  AD1 [ADC]
**     Description :
**         This event is called after the measurement (which consists
**         of <1 or more conversions>) is/are finished.
**         The event is available only when the <Interrupt
**         service/event> property is enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void AD1_OnEnd(void)
{
  /* Write your code here ... */
	
}

/*
** ===================================================================
**     Event       :  AD1_OnCalibrationEnd (module Events)
**
**     Component   :  AD1 [ADC]
**     Description :
**         This event is called when the calibration has been finished.
**         User should check if the calibration pass or fail by
**         Calibration status method./nThis event is enabled only if
**         the <Interrupt service/event> property is enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void AD1_OnCalibrationEnd(void)
{
  /* Write your code here ... */
}

/* END Events */

#ifdef __cplusplus
}  /* extern "C" */
#endif 

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
