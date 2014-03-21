/*--------------------------------------------------------------------------------------------------
  "THE BEER-WARE LICENSE" (Revision 42):
  <phk@FreeBSD.ORG> wrote this file. As long as you retain this notice you
  can do whatever you want with this stuff. If we meet some day, and you think
  this stuff is worth it, you can buy me a beer in return to Hotboards crew
  (Beer-ware license created by Poul-Henning Kamp)

  \file         pwm.h
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
#ifndef __PWM_H__
#define __PWM_H__


    /*-- Includes --*/
    #include "types.h"
     

    /*-- Defines --*/
    /**---------------------------------------------------------------------------------------------
      \def      PWM_CLOCK
      \brief    Esta definicion es interna unicamente, autuando solo como valor por default cuando
                cuando no se define BSP_CLOCK en hardware_profile.h
    ----------------------------------------------------------------------------------------------*/
    //#define PWM_CLOCK                 12000000 (move to pwm.c)

    /**---------------------------------------------------------------------------------------------
      \def      PWM_DIVISOR
      \brief    valores de divisor (1,4,16)
                Esta definicion es interna unicamente, actuando solo como valor por default cuando
                no se define PWM_DIVISOR en hardware_profile.h
    ----------------------------------------------------------------------------------------------*/
    //#define PWM_DIVISOR             	16 (move to pwm.c)

    /**---------------------------------------------------------------------------------------------
      \def      _ePWM
      \brief    Moulos pwm que pueden ser usados en el driver
    ----------------------------------------------------------------------------------------------*/
    typedef enum
    {
        PWM_PORT1 = 1,
        PWM_PORT2
    }_ePWM;

    /**---------------------------------------------------------------------------------------------
      \def      _ePWM_CHANNELS
      \brief    Canales pwm que pueden ser usados en ambos PWM, se pueden usar uno por vez o demanera
                simultanea
    ----------------------------------------------------------------------------------------------*/
    typedef enum
    {
        PWM_CHANNEL_A = 0x01,
        PWM_CHANNEL_B = (1<<1),
        PWM_CHANNEL_C = (1<<2),
        PWM_CHANNEL_D = (1<<3)
    }_ePWM_CHANNELS;

    
    /*-- Macros --*/
    /**---------------------------------------------------------------------------------------------
      \def        Macro
      \brief      Write a nice description for your macros
    ----------------------------------------------------------------------------------------------*/

    
    /*-- Functions --*/
    /**---------------------------------------------------------------------------------------------
      \brief      Inicializa el periférico ECCPx en modo pwm para generar una señal con la frecuencia
                  indicada
      \param	  u8Pwm.- Modulo PWM a ser inicializado
      \param      u32Frec.- Frecuencia a la cual se generara el pulso del PWM
      \return     None
      \warning	  Debes tomar en cuenta que el valor de la frecuencia se calcula apartir de la
                  siguiente formula y que PR solo puede tener valores de 0 a 255:
                             (BSP_CLOCK / 4)
                   PR = ------------------------  - 1
                          u32Frec * PWM_DIVISOR
    ----------------------------------------------------------------------------------------------*/
    void Pwm_Init(_U08 u8Pwm, _U32 u32Frec);

    /**---------------------------------------------------------------------------------------------
      \brief      Establece el duty cycle de la señal en el canal seleccionado
      \param	  u8Pwm.- Modulo PWM con el canal a configurar
      \param      u8Channel.- Canal a setear su duty cycle (solo usar los valores en _ePWM_CHANNELS)
      \param      u8Duty.- Porcentaje de la señal en nivl alto, valores de 0 a 100 unicamente
      \return     None
      \warning	  En el mismo modulo pwm no se puede setear canales con duty cycle diferente
    ----------------------------------------------------------------------------------------------*/
    void Pwm_DutyCycle(_U08 u8Pwm, _U08 u8Channel, _U08 u8Duty);


#endif   
 