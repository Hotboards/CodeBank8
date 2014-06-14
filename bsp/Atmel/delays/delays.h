/*--------------------------------------------------------------------------------------------------
  "THE BEER-WARE LICENSE" (Revision 42):
  <phk@FreeBSD.ORG> wrote this file. As long as you retain this notice you
  can do whatever you want with this stuff. If we meet some day, and you think
  this stuff is worth it, you can buy me a beer in return to Hotboards crew 
  (Beer-ware license created by Poul-Henning Kamp)

  \file         delays.h
  \author       Atmel
  \email        
  \ver          1.0
  \date         June 13, 2014
  \target       ATmega series
  
  \brief        Código para generar retardos por software sin utilizar ningún periférico del 
                microcontrolador. Los retardos los realiza mediante decrementos acorde a la frecuencia 
                de operación del CPU. Se debe tomar en cuenta que el uso de interrupciones puede 
                afectar la duración de los retardos.
 -------------------------------------------------------------------------------------------------*/
#ifndef __DELAYS_H__
#define __DELAYS_H__


    /*-- Includes --*/
    #include "types.h"
    #include "hardware_profile.h"
     

    /*-- Defines --*/
    /**--------------------------------------------------------------------------------------------- 
      \def        F_CPU
      \brief      Esta definicion es interna unicamente, actuando solo como valor por default cuando
                  cuando no se define BSP_CLOCK en hardware_profile.h
    ----------------------------------------------------------------------------------------------*/ 
    #ifndef BSP_CLOCK
        #define F_CPU                 8000000
    #else
        #define F_CPU                 BSP_CLOCK
    #endif
    #include <util/delay.h>


    /*-- Macros --*/
    /**--------------------------------------------------------------------------------------------- 
      \def        Macro
      \brief      Write a nice description for your macros
    ----------------------------------------------------------------------------------------------*/ 
    
    
    /*-- Functions --*/
    /**---------------------------------------------------------------------------------------------    
      \brief      Esta función realiza retardos en lapsos de micro segundos
      \param      us.- numero de micro segundos a retardar.
      \return     None
      \warning    Esta función se debe usar con las interrupciones desactivadas
    ----------------------------------------------------------------------------------------------*/
    #define Delays_us(us)                   _delay_us((us))
    
    /**---------------------------------------------------------------------------------------------    
      \brief      Esta función realiza retardos en múltiplos de 10 micro segundos
      \param      us.- numero de decenas de micro segundos a retardar.
      \return     None
      \warning    Esta función se debe usar con las interrupciones desactivadas
    ----------------------------------------------------------------------------------------------*/
    #define Delays_10us(us)                   _delay_us((us)*10)

    /**---------------------------------------------------------------------------------------------
      \brief      Esta función realiza retardos en lapsos de mili segundos
      \param      ms.- numero de mili segundos a retardar
      \return     None
      \warning    Esta función se debe usar con las interrupciones desactivadas
    ----------------------------------------------------------------------------------------------*/
    void Delays_ms(_U16 ms);


#endif   
 