/*--------------------------------------------------------------------------------------------------
  "THE BEER-WARE LICENSE" (Revision 42):
  <phk@FreeBSD.ORG> wrote this file. As long as you retain this notice you
  can do whatever you want with this stuff. If we meet some day, and you think
  this stuff is worth it, you can buy me a beer in return to Hotboards crew 
  (Beer-ware license created by Poul-Henning Kamp)

  \file         system.h
  \author       Diego
  \email        diego.perez@hotboards.org
  \ver          2.0
  \date         June 14, 2014
  \target       ATmega series

  \brief        Este driver maneja operaciones internas realizadas por el CPU tales como activacion/
                desactivacion de interrupciones, reset del contador de watchdog, reset por software,
                y la activacion interna del PLL para que aumente la frecuencia de operacion a 48MHz
 -------------------------------------------------------------------------------------------------*/
#ifndef __SYSTEM_H__
#define __SYSTEM_H__


    /*-- Includes --*/
    #include <avr/interrupt.h>
    #include <avr/cpufunc.h>
    #include "types.h"


    /*-- Defines --*/
    /**--------------------------------------------------------------------------------------------- 
      \def        _ePPS
      \brief      perifericos que pueden ser reubicados en los diferentes pines "RPn"
    ----------------------------------------------------------------------------------------------*/ 
   


    /*-- Macros --*/
    /**--------------------------------------------------------------------------------------------- 
      \def        ENABLE_INTERRUPTS/DISABLE_INTERRUPTS
      \brief      Habilita o desabilita las interrupciones globales del uC
    ----------------------------------------------------------------------------------------------*/ 
    #define __ENABLE_INTERRUPTS()                 sei()
    #define __DISABLE_INTERRUPTS()                cli()

    /**---------------------------------------------------------------------------------------------
      \def        __KICK_THE_DOG()
      \brief      Resetea el contador del Watchdog timer
    ----------------------------------------------------------------------------------------------*/
    #define __KICK_THE_DOG()                      

    /**---------------------------------------------------------------------------------------------
      \def        __RESET()
      \brief      Provoca un reset del uC
    ----------------------------------------------------------------------------------------------*/
    #define __RESET()                             

    /**---------------------------------------------------------------------------------------------
      \def        __NOP()
      \brief      Ejecuta una instruccion que consume un ciclo en el procesador
    ----------------------------------------------------------------------------------------------*/
    #define __NOP()                               __NOP()

    
    /*-- Functions --*/
    /**---------------------------------------------------------------------------------------------
      \brief      Activa el PLL interno para que la frecuecia interne aumente a 48MHz
      \param      None
      \return     None
      \warning    Esta funcion ciclara al uC por espacio de 2ms
    ----------------------------------------------------------------------------------------------*/


#endif   
 