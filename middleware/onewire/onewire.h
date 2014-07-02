/*-------------------------------------------------------------------------------------------------/
/  Universal string handler for user console interface
/--------------------------------------------------------------------------------------------------/
/
/  Copyright (C) 2011, ChaN, all right reserved.
/
/ * This software is a free software and there is NO WARRANTY.
/ * No restriction on use. You can use, modify and redistribute it for
/   personal, non-profit or commercial products UNDER YOUR RESPONSIBILITY.
/ * Redistributions of source code must retain the above copyright notice.
/
/-------------------------------------------------------------------------------------------------*/
#ifndef __ONEWIRE_H__
#define __ONEWIRE_H__


    /*-- Includes --*/
    #include "types.h"
     

    /*-- Defines --*/
    /**---------------------------------------------------------------------------------------------
      \def      ONEWIRE_N_PORTS
      \brief    Numero de puertos one wire que se utilizaran en la aplicacion
                Es obligatorio escribir esta definicion en el archivo hardware_profile.h
    ----------------------------------------------------------------------------------------------*/
    //#define ONEWIRE_N_PORTS               1

    /**---------------------------------------------------------------------------------------------
      \def      ONEWIRE_PORT_Px
      \brief    Enumeraciones usadas para identificar el puerto que se quiere manipular
    ----------------------------------------------------------------------------------------------*/
    //#define ONEWIRE_PORT_P0               GPIOS_PORTB
    //#define ONEWIRE_PORT_B0               4

    /**---------------------------------------------------------------------------------------------
      \def      _eONEWIRE_PORTS
      \brief    Enumeraciones usadas para identificar el puerto que se quiere manipular
    ----------------------------------------------------------------------------------------------*/
    typedef enum
    {
        ONEWIRE_PORT0 = 0,
        ONEWIRE_PORT1,
        ONEWIRE_PORT2,
        ONEWIRE_PORT3
    }_eONEWIRE_PORTS;


    /*-- Macros --*/
    /**--------------------------------------------------------------------------------------------- 
      \def        Macro
      \brief      Write a nice description for your macros
    ----------------------------------------------------------------------------------------------*/ 
    
    
    /*-- Functions --*/
    /**---------------------------------------------------------------------------------------------
      \brief      Manda un carácter por pantalla
      \param      c.- carácter a mandar a la salida
      \return     None
      \warning    None
    ----------------------------------------------------------------------------------------------*/
    _BOOL OneWire_bResetPulse( _U08 u8Port );

    /**---------------------------------------------------------------------------------------------
      \brief      Manda un carácter por pantalla
      \param      c.- carácter a mandar a la salida
      \return     None
      \warning    None
    ----------------------------------------------------------------------------------------------*/
    void OneWire_WriteByte( _U08 u8Port, _U08 u8Byte );

    /**---------------------------------------------------------------------------------------------
      \brief      Manda una cadena de caracteres terminada en cero por pantalla
      \param      str.- puntero a cadena de caracteres
      \return     None
      \warning    Esta función solo trabaja con cadenas almacenadas en flash
    ----------------------------------------------------------------------------------------------*/
    _U08 OneWire_u8ReadByte( _U08 u8Port );


#endif   
 