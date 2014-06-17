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
  \date         June 14, 2014
  \target       ATmega series
  
  \brief        El driver swtimers permite generar períodos de tiempo sin la necesidad de ciclar el 
                CPU haciendo nada. Mediante el uso del TIMER1 del ATmega se puede multiplicar el 
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
/*-- Includes --*/
#include "swtimers.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include "hardware_profile.h"


/*-- Private Definitions--*/
#ifndef BSP_CLOCK
    #define TIMERS_CLOCK                    8000000
#else
    #define TIMERS_CLOCK                    BSP_CLOCK
#endif

#ifndef TIMERS_BASE_TIME
    #define TIMERS_BASE_TIME                5
#endif

#ifndef TIMERS_PREESCALER             	
    #define TIMERS_PREESCALER               8
#endif

#ifndef TIMERS_N_CHANNELS
    #define TIMERS_N_CHANNELS               9
#endif


#if TIMERS_PREESCALER == 1
    #define TIMERS_PRE					(_U08)0x01
#elif TIMERS_PREESCALER == 8
    #define TIMERS_PRE					(_U08)0x02
#elif TIMERS_PREESCALER == 64
    #define TIMERS_PRE					(_U08)0x03
#elif TIMERS_PREESCALER == 256
    #define TIMERS_PRE					(_U08)0x04	
#elif TIMERS_PREESCALER == 1024
    #define TIMERS_PRE					(_U08)0x05	
#else
    #error "Prescaler invalid value"
#endif


/*-- Global variables --*/
const _U16 timers_ms  = (_U16)TIMERS_BASE_TIME;
static _U16 gua16Timers[(_U08)TIMERS_N_CHANNELS];
static _U16 gua16ModValue = (_U16)((_U32)0xFFFF - (((_U32)TIMERS_CLOCK/((_U32)TIMERS_PREESCALER * (_U32)1000)) * (_U32)TIMERS_BASE_TIME));


/*-- Private Macros --*/
#define TIMERS_MOD(time)            TCNT1 = (time)
#define TIMERS_INT()                SET_8BIT(TIMSK1, 0)
#define TIMERS_SET_PREESCALER()     TCCR1B |= TIMERS_PRE


/*-- Private functions prototypes --*/


/*-- External functions --*/
/**-----------------------------------------------------------------------------------------------*/
void Timers_Init(void)
{
    _U08 i;

    TIMERS_MOD(gua16ModValue);	/* set the max count allow for the base time value */
    TIMERS_SET_PREESCALER();    /* set the preescaler value ant turn on timer*/
    TIMERS_INT();               /* enable module interrupt */

    for(i=0;i<TIMERS_N_CHANNELS;i++)
    {
        gua16Timers[i] = 0u;
    }
}
/**-----------------------------------------------------------------------------------------------*/

/**-----------------------------------------------------------------------------------------------*/
_U16 Timers_u16GetTime(const _U08 u8Timer)
{
    if(u8Timer > TIMERS_N_CHANNELS) return 0u; /*this condition is only to prevent errors*/
    return gua16Timers[u8Timer];
}
/**-----------------------------------------------------------------------------------------------*/

/**-----------------------------------------------------------------------------------------------*/
void Timers_SetTime(const _U08 u8Timer, _U16 u16Time)
{
    if(u8Timer > TIMERS_N_CHANNELS) return; /*this condition is only to prevent errors*/
    gua16Timers[u8Timer] = u16Time;
}
/**-----------------------------------------------------------------------------------------------*/

/**-----------------------------------------------------------------------------------------------*/
ISR(TIMER1_OVF_vect)
{
    volatile _U08 u8Channels = (_U08)TIMERS_N_CHANNELS;
   
    TIMERS_MOD(gua16ModValue);	/* set the max count allow for the base time value */
    while(u8Channels)
    {/* loop over each timer */
        u8Channels--;
        if(gua16Timers[u8Channels])
        {/* decrement channel if diferent from 0 */
            gua16Timers[u8Channels]--;
        }
    }
}
/**-----------------------------------------------------------------------------------------------*/


/*-- Private functions --*/
/**-------------------------------------------------------------------------------------------------
  \brief        None
  \param        None
  \return       None
  \warning      None
--------------------------------------------------------------------------------------------------*/
