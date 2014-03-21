/*--------------------------------------------------------------------------------------------------
  "THE BEER-WARE LICENSE" (Revision 42):
  <phk@FreeBSD.ORG> wrote this file. As long as you retain this notice you
  can do whatever you want with this stuff. If we meet some day, and you think
  this stuff is worth it, you can buy me a beer in return to Hotboards crew
  (Beer-ware license created by Poul-Henning Kamp)

  \file         pwm.c
  \author       Diego
  \email        diego.perez@hotboards.org
  \ver          1.0
  \date         Marzo 19, 2014
  \target       PIC18FJ

  \brief        Driver para controlar la generación de señales PWM por hardware, las señales son
                generadas usando los módulos ECCP del microcontrolador (el cual posee dos). Además de
                estos módulos, se depende de dos timer del micro, el Timer2 lo usa el módulo ECCP1 y
                el Timer4 lo usa el módulo ECCP2.
--------------------------------------------------------------------------------------------------*/
/*-- Includes --*/
#include "pwm.h"
#include <p18cxxx.h>
#include <string.h>


/*-- Private Definitions--*/
#ifndef BSP_CLOCK
    #define PWM_CLOCK                 12000000
#else
    #define PWM_CLOCK                 BSP_CLOCK
#endif

#ifndef PWM_DIVISOR
    #define PWM_DIVISOR               16
#endif

#if PWM_DIVISOR == 1
    #define PWM_DIV                   0x04
#elif PWM_DIVISOR == 4
    #define PWM_DIV                   0x05
#elif PWM_DIVISOR == 16
    #define PWM_DIV                   0x06
#else
    #error "Valor invalido de divisor"
#endif


/*-- Global variables --*/
static volatile near _U08 *gau8CCPCON[3] = {NULL, &CCP1CON, &CCP2CON};
static volatile near _U08 *gau8PSTRCON[3] = {NULL, &PSTR1CON, &PSTR2CON};
static volatile near _U08 *gau8CCPRL[3]  = {NULL, &CCPR1L, &CCPR2L};
static volatile near _U08 *gau8TCON[3]  = {NULL, &T2CON, &T4CON};
static volatile near _U08 *gau8PR[3]  = {NULL, &PR2, &PR4};
static const _U08 gau8TCLKCON[3]  = {0x00, 0x01, 0x02};


/*-- Private Macros --*/


/*-- Private functions prototypes --*/


/*-- External functions --*/
/**-----------------------------------------------------------------------------------------------*/
void Pwm_Init(_U08 u8Pwm, _U32 u32Frec)
{
    if((u8Pwm == PWM_PORT1) || (u8Pwm == PWM_PORT2))
    {
        TCLKCON = gau8TCLKCON[u8Pwm]; /*ECCPx feeded by timer2 or timer4*/
        *gau8CCPRL[u8Pwm] = 0x00;     /*valor incial de cero al duty cycle*/
        
        *gau8PR[u8Pwm] = ((((_U32)PWM_CLOCK/(_U32)4))/(u32Frec*(_U32)PWM_DIVISOR))-(_U32)1;
        *gau8TCON[u8Pwm] = PWM_DIV;    /*divisor del timer de 16*/
        
        *gau8CCPCON[u8Pwm] = 0x0f;  /*modo de operacion en nivel bajo idle*/
    }
}
/**-----------------------------------------------------------------------------------------------*/

/**-----------------------------------------------------------------------------------------------*/
void Pwm_DutyCycle(_U08 u8Pwm, _U08 u8Channel, _U08 u8Duty)
{
    _U08 CCPL;
    _U32 u32Duty;

    if((u8Pwm == PWM_PORT1) || (u8Pwm == PWM_PORT2))
    {
        /*calculamos el valor a cargar el los registros de -Duty Cycle*/
        u32Duty = ((_U16)PR2<<(_U16)2) | 0x03;
        u32Duty = (u32Duty*(_U32)u8Duty)/(_U32)100;
        CCPL = (_U08)u32Duty & 0x03;

        /*Configuramos los registros para generar el duty cycle deseado*/
        *gau8CCPRL[u8Pwm] = (_U16)u32Duty>>(_U16)2;
        *gau8CCPCON[u8Pwm] &= ~(2<<4);
        *gau8CCPCON[u8Pwm] |= (CCPL<<4);
        *gau8PSTRCON[u8Pwm] = u8Channel;
    }
}
/**-----------------------------------------------------------------------------------------------*/


/*-- Private functions --*/
/**---------------------------------------------------------------------------------------------    
  \brief    None
  \param    None
  \return   None
  \warning  None   	
----------------------------------------------------------------------------------------------*/
