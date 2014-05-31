/*--------------------------------------------------------------------------------------------------
  "THE BEER-WARE LICENSE" (Revision 42):
  <phk@FreeBSD.ORG> wrote this file. As long as you retain this notice you
  can do whatever you want with this stuff. If we meet some day, and you think
  this stuff is worth it, you can buy me a beer in return to Hotboards crew 
  (Beer-ware license created by Poul-Henning Kamp)

  \file         swtimers.h
  \author       Diego
  \email        diego.perez@hotboards.org
  \ver          2.0
  \date         May 30, 2014
  \target       PIC18F series
  
  \brief        El driver swtimers permite generar per�odos de tiempo sin la necesidad de ciclar el 
                CPU haciendo nada. Mediante el uso del TIMER0 del PIC18F se puede multiplicar el 
                n�mero de canales que representar� un periodo de tiempo diferente. De esta manera se 
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
      \brief      Inicializa el timer interno TMR0 para que genere una interrupci�n cada TIMERS_BASE_TIME
      \param      None
      \return     None
      \warning    None      
    ----------------------------------------------------------------------------------------------*/
    void Timers_Init(void);

    /**---------------------------------------------------------------------------------------------
      \brief      Asigna prioridad a la interrupci�n que generara el driver
      \param      bPriority.- valor de la prioridad _HIGH o _LOW
      \return     None
      \warning    Por default al llamarse la funci�n Timers_Init la interrupci�n tiene un valor _LOW
    ----------------------------------------------------------------------------------------------*/
    void Timers_InterruptPriority(const _BOOL bPriority);

    /**---------------------------------------------------------------------------------------------
      \brief      Regresa el valor actual del timer seleccionado
      \param      u8Timer.- Canal a regresar su valor de timepo, valores de 0 a TIMERS_CHANNELS-1
      \return     Valor de la cuenta actual
      \warning    None      
    ----------------------------------------------------------------------------------------------*/
    _U16 Timers_u16GetTime(const _U08 u8Timer);

    /**---------------------------------------------------------------------------------------------    
      \brief      Asigna un nuevo periodo de tiempo a decrementar al canal asignado
      \param      u8Timer.- Canal a recargar, valores de 0 a TIMERS_CHANNELS-1 
      \param      u16Time.-  Cantidad e tiempo a decrementar, solo valores m�ltiplos de TIMERS_BASE_TIME
      \return     None
      \warning    se puede convertir un valor constante a ms si se usa la constante timers_ms, siempre 
                  y cuando el valor sea m�ltiplo de TIMERS_BASE_TIME       
    ----------------------------------------------------------------------------------------------*/
    void Timers_SetTime(const _U08 u8Timer, _U16 u16Time);

    /**---------------------------------------------------------------------------------------------
      \brief      Funci�n de interrupci�n la cual decrementa en uno los canales activos.
      \param      None
      \return     None
      \warning    Esta funci�n se debe mandar llamar en uno de los dos vectores de interrupci�n
    ----------------------------------------------------------------------------------------------*/
    void Timers_Isr(void);

    
#endif   
 