/*--------------------------------------------------------------------------------------------------
  "THE BEER-WARE LICENSE" (Revision 42):
  <phk@FreeBSD.ORG> wrote this file. As long as you retain this notice you
  can do whatever you want with this stuff. If we meet some day, and you think
  this stuff is worth it, you can buy me a beer in return to Hotboards crew 
  (Beer-ware license created by Poul-Henning Kamp)

  \file         delays.h
  \author       Microchip Technology, Inc
  \email        
  \ver          2.0
  \date         June 22, 2014
  \target       PIC18F series
  
  \brief        C�digo para generar retardos por software sin utilizar ning�n perif�rico del 
                microcontrolador. Los retardos los realiza mediante decrementos acorde a la frecuencia 
                de operaci�n del CPU. Se debe tomar en cuenta que el uso de interrupciones puede 
                afectar la duraci�n de los retardos.
 -------------------------------------------------------------------------------------------------*/
#ifndef __DELAYS_H__
#define __DELAYS_H__


    /*-- Includes --*/
    #include "types.h"
    #include "hardware_profile.h"
     

    /*-- Defines --*/
    /**--------------------------------------------------------------------------------------------- 
      \def        _XTAL_FREQ
      \brief      Esta definicion es interna unicamente, autuando solo como valor por default cuando
                  cuando no se define BSP_CLOCK en hardware_profile.h
    ----------------------------------------------------------------------------------------------*/ 
    #ifndef BSP_CLOCK
        #define _XTAL_FREQ                 12000000
    #else
        #define _XTAL_FREQ                 BSP_CLOCK
    #endif
    #include <xc.h>


    /*-- Macros --*/
    /**--------------------------------------------------------------------------------------------- 
      \def        Macro
      \brief      Write a nice description for your macros
    ----------------------------------------------------------------------------------------------*/ 
    
    
    /*-- Functions --*/
    /**---------------------------------------------------------------------------------------------
      \brief      Esta funci�n realiza retardos en m�ltiplos de micro segundos
      \param      us.- numero de micro segundos a retardar.
      \return     None
      \warning    Esta funci�n se debe usar con las interrupciones desactivadas
    ----------------------------------------------------------------------------------------------*/
    #define Delays_us(us)               __delay_us(us)

    /**---------------------------------------------------------------------------------------------    
      \brief      Esta funci�n realiza retardos en m�ltiplos de 10 micro segundos
      \param      us.- numero de decenas de micro segundos a retardar.
      \return     None
      \warning    Esta funci�n es obsoleta y sera removida en futuras versiones
    ----------------------------------------------------------------------------------------------*/
    #define Delays_10us(us)             __delay_us(10*us)

    /**---------------------------------------------------------------------------------------------
      \brief      Esta funci�n realiza retardos en lapsos de mili segundos
      \param      ms.- numero de mili segundos a retardar
      \return     None
      \warning    Esta funci�n se debe usar con las interrupciones desactivadas
    ----------------------------------------------------------------------------------------------*/
    void Delays_ms(_U16 ms);


#endif   
 