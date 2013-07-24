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
  \date         July 16, 2013
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
/*-- Includes --*/
#include "swtimers.h"
#include <p18cxxx.h>
#include "bsp_profile.h"


/*-- Private Definitions--*/
#ifndef BSP_CLOCK
    #define TIMERS_CLOCK                    12000000
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


#if TIMERS_PREESCALER == 2
    #define TIMERS_PRE					(_U08)0x00
#elif TIMERS_PREESCALER == 4
    #define TIMERS_PRE					(_U08)0x01
#elif TIMERS_PREESCALER == 8
    #define TIMERS_PRE					(_U08)0x02
#elif TIMERS_PREESCALER == 16
    #define TIMERS_PRE					(_U08)0x03	
#elif TIMERS_PREESCALER == 32
    #define TIMERS_PRE					(_U08)0x04	
#elif TIMERS_PREESCALER == 64
    #define TIMERS_PRE					(_U08)0x05	
#elif TIMERS_PREESCALER == 128
    #define TIMERS_PRE					(_U08)0x06	
#elif TIMERS_PREESCALER == 256
    #define TIMERS_PRE					(_U08)0x07	
#else
    #error "Prescaler invalid value"
#endif


/*-- Global variables --*/
const _U16 timers_ms  = (_U16)TIMERS_BASE_TIME;
static _U16 gua16Timers[(_U08)TIMERS_N_CHANNELS];
static _U16 gua16ModValue = 0;


/*-- Private Macros --*/
#define TIMERS_VALUE()              (_U16)((_U32)0xFFFF - ((((_U32)TIMERS_CLOCK / (_U32)4)/((_U32)TIMERS_PREESCALER * (_U32)1000)) * (_U32)TIMERS_BASE_TIME))
#define TIMERS_MOD(time)            TMR0H = (_U16)(time)>>(_U16)8;TMR0L = (_U16)(time)
#define TIMERS_INT()                SET_8BIT(INTCON, 5u)
#define TIMERS_SC()                 SET_8BIT(T0CON, 7u)
#define TIMERS_CLEAR_FLAG()         CLEAR_8BIT(INTCON,2u)
#define TIMERS_SET_PREESCALER()     T0CON = TIMERS_PRE


/*-- Private functions prototypes --*/


/*-- External functions --*/
/**-----------------------------------------------------------------------------------------------*/
void Timers_Init(void)
{
    _U08 i;

    TIMERS_CLEAR_FLAG();        /* clear interrupt flag */
    gua16ModValue = TIMERS_VALUE(); /*calculate the value must set on TMR0*/
    TIMERS_MOD(gua16ModValue);	/* set the max count allow for the base time value */
    TIMERS_SET_PREESCALER();    /* set the preescaler value */
    TIMERS_SC();                /* set status and control register */   
    CLEAR_8BIT(INTCON2, 2u);    /* set to low priority interrupt by the fault */
    TIMERS_INT();               /* enable module interrupt */


    for(i=0;i<TIMERS_N_CHANNELS;i++)
    {
        gua16Timers[i] = 0u;
    }
}
/**-----------------------------------------------------------------------------------------------*/

/**-----------------------------------------------------------------------------------------------*/
void Timers_InterruptPriority(const _BOOL bPriority)
{

    if(bPriority == 0)  /*low priority interrupt*/
    {
        CLEAR_8BIT(INTCON2, 2u);
    }
    else                /*set to high priority interrupt*/
    {
        SET_8BIT(INTCON2, 2u);
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
void Timers_SetTime(const _U08 u8Timer, const _U16 u16Time)
{
    if(u8Timer > TIMERS_N_CHANNELS) return; /*this condition is only to prevent errors*/
    gua16Timers[u8Timer] = u16Time;
}
/**-----------------------------------------------------------------------------------------------*/

/**-----------------------------------------------------------------------------------------------*/
void Timers_Isr(void)
{
    volatile _U08 u8Channels = (_U08)TIMERS_N_CHANNELS;

    if(QUERY_8BIT(INTCON,2u) != 0u) /*enter this interrupt only if the int was cause by the TMR0 overflow*/
    {
        TIMERS_CLEAR_FLAG();/*clear interrupts flags*/
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
}
/**-----------------------------------------------------------------------------------------------*/


/*-- Private functions --*/
/**-------------------------------------------------------------------------------------------------
  \brief        None
  \param        None
  \return       None
  \warning      None
--------------------------------------------------------------------------------------------------*/
