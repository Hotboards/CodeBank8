/*--------------------------------------------------------------------------------------------------
  "THE BEER-WARE LICENSE" (Revision 42):
  <phk@FreeBSD.ORG> wrote this file. As long as you retain this notice you
  can do whatever you want with this stuff. If we meet some day, and you think
  this stuff is worth it, you can buy me a beer in return to Hotboards crew 
  (Beer-ware license created by Poul-Henning Kamp)

  \file         swtimers.h
  \author       Diego
  \email        diego.perez@hotboards.org
  \ver          1.0
  \date         July 25, 2013
  \target       PIC18F series
  
  \brief        El driver swtimers permite generar períodos de tiempo sin la necesidad de ciclar el 
                CPU haciendo nada. Mediante el uso del TIMER0 del PIC18F se puede multiplicar el 
                número de canales que representará un periodo de tiempo diferente. De esta manera se 
                pueden ejecutar diferentes tareas en el microcontrolador a diferente tiempo sin causar 
                un overhead excesivo en el CPU.
                
                Example:
                Timers_Init();
                __ENABLE_INTERRUPTS();
                .................
                Timers_SetTime(0, _100ms);
                ...............
   
                for(;;) 
                {        
                    if(Timers_u16GetTime(0)==0u)        
                    {
                        Timers_SetTime(0, _100ms);
                        do something............    
                    }
                }
--------------------------------------------------------------------------------------------------*/
#ifndef __SWTIMERS_H__
#define __SWTIMERS_H__


    /*-- Includes --*/
    #include "types.h"

    /*-- Defines --*/
    /**--------------------------------------------------------------------------------------------- 
      \def      TIMERS_BASE_TIME
      \brief    value in milliseconds
                Esta definicion es interna unicamente, actuando solo como valor por default cuando
                no se define TIMERS_BASE_TIME en hardware_profile.h
    ----------------------------------------------------------------------------------------------*/ 
    //#define TIMERS_BASE_TIME              	5
	
    /**--------------------------------------------------------------------------------------------- 
      \def      TIMERS_BUSFREC
      \brief    bus clock frequency in Hz that feed this peripheral
                Esta definicion es interna unicamente, actuando solo como valor por default cuando
                no se define BSP_CLOCK en hardware_profile.h
    ----------------------------------------------------------------------------------------------*/ 
    //#define TIMERS_CLOCK                	12000000
	
    /**--------------------------------------------------------------------------------------------- 
      \def      TIMERS_PREESCALER
      \brief    preescaler value (2,4,8,16,32,64,128,256)
                Esta definicion es interna unicamente, actuando solo como valor por default cuando
                no se define TIMERS_PREESCALER en hardware_profile.h
    ----------------------------------------------------------------------------------------------*/ 
    //#define TIMERS_PREESCALER             	8     		
	
    /**--------------------------------------------------------------------------------------------- 
      \def      TIMERS_N_CHANNELS
      \brief    define max number of channels allow
                Esta definicion es interna unicamente, actuando solo como valor por default cuando
                no se define TIMERS_CHANNELS en hardware_profile.h
    ----------------------------------------------------------------------------------------------*/ 
    //#define TIMERS_N_CHANNELS                 9
	
    /**---------------------------------------------------------------------------------------------
      Formula used to calculate the value need it in the comparator register
      NOTE: make sure that "timer_mod_register" value dosen't exceed a value bigger than 0xFFFF
      
                                   //            CLOCK / 4         \                    \
      timer_mod_register = 65535 - ||------------------------------| * TIMERS_BASE_TIME |
                                   \\ BSP_TIMERS_PREESCALER * 1000 /                    /
    ----------------------------------------------------------------------------------------------*/

    /**---------------------------------------------------------------------------------------------
      \def      timers_ms
      \brief    Constante para convertir valores enteros en mili segundos
                100/timers_ms = 100ms, el valor numerico no debera ser menor a TIMERS_BASE_TIME
    ----------------------------------------------------------------------------------------------*/ 
    extern const _U16 timers_ms;
    
    
    /*-- Macros --*/
    
    
    /*-- Functions --*/
    /**---------------------------------------------------------------------------------------------    
      \brief      Inicializa el timer interno TMR0 para que genere una interrupción cada TIMERS_BASE_TIME
      \param      None
      \return     None
      \warning    None      
    ----------------------------------------------------------------------------------------------*/
    void Timers_Init(void);

    /**---------------------------------------------------------------------------------------------
      \brief      Asigna prioridad a la interrupción que generara el driver
      \param      bPriority.- valor de la prioridad _HIGH o _LOW
      \return     None
      \warning    Por default al llamarse la función Timers_Init la interrupción tiene un valor _LOW
    ----------------------------------------------------------------------------------------------*/
    void Timers_InterruptPriority(const _BOOL bPriority);

    /**---------------------------------------------------------------------------------------------
      \brief      Regresa el valor actual del timer seleccionado
      \param      eTimers.- Canal a regresar su valor de timepo, valores de 0 a TIMERS_CHANNELS-1
      \return     Valor de la cuenta actual
      \warning    None      
    ----------------------------------------------------------------------------------------------*/
    _U16 Timers_u16GetTime(const _U08 u8Timer);

    /**---------------------------------------------------------------------------------------------    
      \brief      Asigna un nuevo periodo de tiempo a decrementar al canal asignado
      \param      eTimers.- Canal a recargar, valores de 0 a TIMERS_CHANNELS-1 
      \param      u16Time.-  Cantidad e tiempo a decrementar, solo valores múltiplos de TIMERS_BASE_TIME
      \return     None
      \warning    se puede convertir un valor constante a ms si se usa la constante timers_ms, siempre 
                  y cuando el valor sea múltiplo de TIMERS_BASE_TIME       
    ----------------------------------------------------------------------------------------------*/
    void Timers_SetTime(const _U08 u8Timer, const _U16 u16Time);

    /**---------------------------------------------------------------------------------------------
      \brief      Función de interrupción la cual decrementa en uno los canales activos.
      \param      None
      \return     None
      \warning    Esta función se debe mandar llamar en uno de los dos vectores de interrupción
    ----------------------------------------------------------------------------------------------*/
    void Timers_Isr(void);

    
#endif   
 