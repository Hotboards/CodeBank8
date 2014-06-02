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
  \brief        Pieza de c�digo que maneja el convertidor an�logo digital interno de el uC. mediante
                la funci�n de inicializacion se puede elegir resoluciones de 8 y 10 bits, ademas de
                escoger el canal que se leer� cada que se efectu� una operaci�n de lectura. Debido a
                la cantidad tan peque�a de tiempo (en el orden de los micro segundos), no se utilizan
                interrupciones.

                El tiempo de adquisici�n del modulo adc se ajusta autom�ticamente dependiendo de la
                frecuencia de operaci�n del CPU, y el tiempo de conversi�n se fija a 12 veces el
                tiempo de adquisici�n.
 -------------------------------------------------------------------------------------------------*/
/*-- Includes --*/
#include "adc.h"
#include <xc.h>
#include "hardware_profile.h"


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
#elif ADC_CLOCK < 46000000 && ADC_CLOCK > 22999999
    #define TOSC        0b010
#elif ADC_CLOCK <= 48000000 && ADC_CLOCK > 45999999
    #define TOSC        0b110
#else
    #define TOSC        0b000
#endif


/*-- Global variables --*/
static _U08 gu8Flags;


/*-- Private Macros --*/
#define ADC_SET_CHANNEL(channel)            ADCON0 &= 0b11000011; ADCON0 |= ((channel)<<2)
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
        if(u8Channel < 8)
        {/*canales del 0 al 7*/
            CLEAR_8BIT(ANCON0, u8Channel);
        }
        else
        {/*canales del 8 al 12*/
            CLEAR_8BIT(ANCON1, u8Channel - 8);
        }
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
