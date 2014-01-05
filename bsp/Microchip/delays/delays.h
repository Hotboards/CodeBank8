/*--------------------------------------------------------------------------------------------------
  "THE BEER-WARE LICENSE" (Revision 42):
  <phk@FreeBSD.ORG> wrote this file. As long as you retain this notice you
  can do whatever you want with this stuff. If we meet some day, and you think
  this stuff is worth it, you can buy me a beer in return to Hotboards crew 
  (Beer-ware license created by Poul-Henning Kamp)

  \file         delays.h
  \author       Microchip Technology, Inc
  \email        
  \ver          1.0
  \date         July 25, 2013
  \target       PIC18F series
  
  \brief        Código para generar retardos por software sin utilizar ningún periférico del 
                microcontrolador. Los retardos los realiza mediante decrementos acorde a la frecuencia 
                de operación del CPU. Se debe tomar en cuenta que el uso de interrupciones puede 
                afectar la duración de los retardos.
 -------------------------------------------------------------------------------------------------*/
#ifndef __DELAYS_H__
#define __DELAYS_H__


    /*-- Includes --*/
    #include "types.h"
     

    /*-- Defines --*/
    /**--------------------------------------------------------------------------------------------- 
      \def        DELAYS_CLOCK
      \brief      Esta definicion es interna unicamente, autuando solo como valor por default cuando
                  cuando no se define BSP_CLOCK en hardware_profile.h
    ----------------------------------------------------------------------------------------------*/ 
    //#define DELAYS_CLOCK                 12000000 (move to delays.c)


    /*-- Macros --*/
    /**--------------------------------------------------------------------------------------------- 
      \def        Macro
      \brief      Write a nice description for your macros
    ----------------------------------------------------------------------------------------------*/ 
    
    
    /*-- Functions --*/
    /**---------------------------------------------------------------------------------------------    
      \brief      Esta función realiza retardos en múltiplos de 10 micro segundos
      \param      us.- numero de decenas de micro segundos a retardar.
      \return     None
      \warning    Esta función se debe usar con las interrupciones desactivadas
    ----------------------------------------------------------------------------------------------*/
    void Delays_10us(_U32 us);

    /**---------------------------------------------------------------------------------------------
      \brief      Esta función realiza retardos en lapsos de mili segundos
      \param      ms.- numero de mili segundos a retardar
      \return     None
      \warning    Esta función se debe usar con las interrupciones desactivadas
    ----------------------------------------------------------------------------------------------*/
    void Delays_ms(_U16 ms);


#endif   
 