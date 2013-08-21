/*--------------------------------------------------------------------------------------------------
  "THE BEER-WARE LICENSE" (Revision 42):
  <phk@FreeBSD.ORG> wrote this file. As long as you retain this notice you
  can do whatever you want with this stuff. If we meet some day, and you think
  this stuff is worth it, you can buy me a beer in return to Hotboards crew 
  (Beer-ware license created by Poul-Henning Kamp)

  \file         adc.c
  \author       Diego
  \email        diego.perez@hotboards.org
  \ver          1.0
  \date         August 13, 2013
  \target       PIC18FJ
  \brief        Pieza de código que maneja el convertidor análogo digital interno de el uC. mediante
                la función de inicializacion se puede elegir resoluciones de 8 y 10 bits, ademas de
                escoger el canal que se leerá cada que se efectué una operación de lectura. Debido a
                la cantidad tan pequeña de tiempo (en el orden de los micro segundos), no se utilizan
                interrupciones.

                El tiempo de adquisición del modulo adc se ajusta automáticamente dependiendo de la
                frecuencia de operación del CPU, y el tiempo de conversión se fija a 12 veces el
                tiempo de adquisición.
 -------------------------------------------------------------------------------------------------*/
/*-- Includes --*/
#include "adc.h"
#include <p18cxxx.h>
#include "bsp_profile.h"


/*-- Private Definitions--*/
#ifndef BSP_CLOCK
    #define ADC_CLOCK           12000000
#else
    #define ADC_CLOCK           BSP_CLOCK
#endif

#if ADC_CLOCK <= 1000000
    #define TOSC        0b011
#elif ADC_CLOCK < 3000000 && ADC_CLOCK > 1000000
    #define TOSC        0b000
#elif ADC_CLOCK < 6000000 && ADC_CLOCK > 2999999
    #define TOSC        0b100
#elif ADC_CLOCK < 12000000 && ADC_CLOCK > 5999999
    #define TOSC        0b001
#elif ADC_CLOCK < 23000000 && ADC_CLOCK > 11999999
    #define TOSC        0b101
#elif ADC_CLOCK < 47000000 && ADC_CLOCK > 22999999
    #define TOSC        0b010
#elif ADC_CLOCK < 48000000 && ADC_CLOCK > 46999999
    #define TOSC        0b110
#else
    #define TOSC        0b000
#endif


/*-- Global variables --*/
static _U08 gu8Flags;


/*-- Private Macros --*/
#define ADC_INIT_CHANNEL(channel)           (SET_8BIT(ANCON0, (channel)))
#define ADC_SET_CHANNEL(channel)            (ADCON0 |= ((channel)<<2))
#define ADC_START()                         (SET_8BIT(ADCON0, 1u))
#define ADC_FLAG()                          (QUERY_8BIT(ADCON0, 1u))
#define ADC_BUFFER()                        (((_U16)ADRESH<<(_U16)8)|(_U16)ADRESL)


/*-- Private functions prototypes --*/


/*-- External functions --*/
/**-----------------------------------------------------------------------------------------------*/
void Adc_Init(_U08 u8Flags)
{
    ADCON0 |= 0x01;//SET_8BIT(ADCON0, 0);        /*turn on adc*/
    ADCON1 |= (TOSC | 0b101000);
    gu8Flags = u8Flags;
    if(u8Flags == ADC_10BITS)
    {
        SET_8BIT(ADCON1, 7);
    }
    else
    {
        CLEAR_8BIT(ADCON1, 7);
    }
}
/**-----------------------------------------------------------------------------------------------*/

/**-----------------------------------------------------------------------------------------------*/
_U16 Adc_u16Read(_U08 u8Channel)
{
    if(u8Channel < 13)
    {
        ADC_INIT_CHANNEL(u8Channel);
        ADC_SET_CHANNEL(u8Channel);
        ADC_START();
        while(ADC_FLAG()==1);
        if(gu8Flags == ADC_10BITS)
        {
            return ADC_BUFFER();
        }
        else
        {
            return ADRESH;
        }
    }
    return 0;
}
/**-----------------------------------------------------------------------------------------------*/


/*-- Private functions --*/
/**-------------------------------------------------------------------------------------------------    
  \brief        None
  \param        None
  \return       None
  \warning      None       
--------------------------------------------------------------------------------------------------*/
