/*--------------------------------------------------------------------------------------------------
  "THE BEER-WARE LICENSE" (Revision 42):
  <phk@FreeBSD.ORG> wrote this file. As long as you retain this notice you
  can do whatever you want with this stuff. If we meet some day, and you think
  this stuff is worth it, you can buy me a beer in return to Hotboards crew 
  (Beer-ware license created by Poul-Henning Kamp)

  \file         gpios.h
  \author       Diego
  \email        diego.perez@hotboards.org
  \ver          1.0
  \date         June 12, 2014
  \target       ATmega series

  \brief        El driver de bajo nivel gpios agrupa una colecci�n de funciones que permiten manejar 
                los puertos de una manera m�s manejable y portable. Mediante las funciones descritas 
                en el archivo gpios.h, podr�s establecer la direcci�n, escribir y/o leer, ya sea el 
                puerto entero o pin a pin.
 -------------------------------------------------------------------------------------------------*/
#ifndef __GPIOS_H__
#define __GPIOS_H__


    /*-- Includes --*/
    #include "types.h"
     

    /*-- Defines --*/
    /*----------------------------------------------------------------------------------------------
    \def    GPIOS_INPUT/OUTPUT
    \brief  constantes para definir la direcci�n en la que operaran los pines de los puertos 
    ----------------------------------------------------------------------------------------------*/
    #define GPIOS_OUTPUT        1
    #define GPIOS_INPUT         0

    /**--------------------------------------------------------------------------------------------- 
      \def  _eGPIOS_PORT
      \brief    Enumeraciones usadas para identificar el puerto que se quiere manipular                        
    ----------------------------------------------------------------------------------------------*/     
    typedef enum 
    {
        GPIOS_PORTA = 0, 
        GPIOS_PORTB, 
        GPIOS_PORTC,
        GPIOS_PORTD,
        GPIOS_PORTE,
        GPIOS_PORTF,
        GPIOS_PORTG
    }_eGPIOS_PORT;
    
    /*-- Macros --*/
    /**--------------------------------------------------------------------------------------------- 
      \def        Macro
      \brief      Write a nice description for your macros
    ----------------------------------------------------------------------------------------------*/ 
    
    
    /*-- Functions --*/
    /**---------------------------------------------------------------------------------------------    
      \brief      Iniaciliza un pin del uC a la vez 
      \param      ePort: Puerto donde se encuentra el pin (GPIOS_PORTx)
      \param      u8Pin:  Numero de pin (de 0 to 7)
      \param      bDirection: Modo entrada (GPIOS_INPUT) o salida (GPIOS_OUTPUT)
      \return     None
      \warning    None    
    ----------------------------------------------------------------------------------------------*/
    void Gpios_PinDirection(_eGPIOS_PORT ePort, _U08 u8Pin, _BOOL bDirection);

    /**---------------------------------------------------------------------------------------------    
      \brief      funcion usada para escribir valores en un pin del uC  
      \param      ePort: Puerto en el que se encuentra el pin (GPIOS_PORTx)
      \param      u8Pin: Numero de pin (de 0 a 7)    
      \param      bValue: valor de salida (0 o 1)    
      \return     None
      \warning    Se debe inicializar el pin como salida antes de usar esta funci�n      
    ----------------------------------------------------------------------------------------------*/
    void Gpios_WritePin(_eGPIOS_PORT ePort, _U08 u8Pin, _BOOL bValue);

    /**---------------------------------------------------------------------------------------------    
      \brief      Funcion usada para activar las pullups internas del uC 
      \param      ePort: Puerto en el que se encuentra el pin (GPIOS_PORTx)
      \param      u8Pin: Numero de pin (de 0 a 7)    
      \param      bValue: '1' to activar pullup 0 para desactivar    
      \return     None
      \warning    Esta opci�n no existe en los PIC18F y solo existe por compatibilidad     
    ----------------------------------------------------------------------------------------------*/
    void Gpios_PullupPin(_eGPIOS_PORT ePort, _U08 u8Pin, _BOOL bValue);

    /**---------------------------------------------------------------------------------------------    
      \brief      Funci�n usada para invertir el estado de un solo Pin.
      \param      ePort: Puerto en el que se encuentra el pin (GPIOS_PORTx)
      \param      u8Pin: Numero de pin (de 0 a 7)    
      \return     None
      \warning    Se debe inicializar el pin como salida antes de usar esta funci�n   
    ----------------------------------------------------------------------------------------------*/  
    void Gpios_TogglePin(_eGPIOS_PORT ePort, _U08 u8Pin);

    /**---------------------------------------------------------------------------------------------
      \brief      Funci�n usada para leer el valor en un pin.
      \param      ePort: Puerto en el que se encuentra el pin (GPIOS_PORTx)
      \param      u8Pin: Numero de pin (de 0 a 7)   
      \return     la funci�n solo regresa valores de 1 o 0 
      \warning    Se debe inicializar el pin como entrada antes de usar esta funci�n        
    ----------------------------------------------------------------------------------------------*/  
    _BOOL Gpios_bReadPin(_eGPIOS_PORT ePort, _U08 u8Pin);

    /**---------------------------------------------------------------------------------------------
      \brief      Funci�n usada para escribir completamente en un puerto
      \param      ePort: puerto a escribir (GPIOS_PORTx)
      \param      u8Value: valor a escribir (de 0 a 255)      
      \return     None
      \warning    se debe inicializar el puerto completo como salida
    ----------------------------------------------------------------------------------------------*/
    void Gpios_WritePort(_eGPIOS_PORT ePort, _U08 u8Value);

    /**---------------------------------------------------------------------------------------------
      \brief      Funci�n usada para leer un puerto completo
      \param      ePort: puerto a leer (GPIOS_PORTx) 
      \return     Valor le�do del puerto 
      \warning    None
    ----------------------------------------------------------------------------------------------*/
    _U08 Gpios_u8ReadPort(_eGPIOS_PORT ePort);

    /**---------------------------------------------------------------------------------------------    
      \brief      Funci�n usada para escribir la direcci�n de los pines del puerto
      \param      ePort: Nombre del puerto a configurar 
      \param      u8Value: Valor a cargar al registro TRISx
      \return     None
      \warning    None      
    ----------------------------------------------------------------------------------------------*/
    void Gpios_WriteTris(_eGPIOS_PORT ePort, _U08 u8Value);

    /**---------------------------------------------------------------------------------------------
      \brief      Funcion usada para leer la configuraci�n de un puerto completo
      \param      ePort: Nombre del puerto
      \return     Valor interno del registro                   
      \warning    None
    ----------------------------------------------------------------------------------------------*/
    _U08 Gpios_u8ReadTris(_eGPIOS_PORT ePort);

    /**---------------------------------------------------------------------------------------------
      \brief      Funcion usada para activar las resistencias de pullup de un puerto completo
      \param      ePort: Nombre del puerto
      \return     None                    
      \warning    Esta opci�n no existe en los PIC18F y solo existe por compatibilidad 
    ----------------------------------------------------------------------------------------------*/
    void Gpios_SetPullups(_eGPIOS_PORT ePort, _U08 u8Value);

    /**---------------------------------------------------------------------------------------------
      \brief      Funcion usada para leer las resistencias pullup activas de un puerto
      \param      ePort: Nombre del puerto 
      \return     Resistencias e pullup activas                     
      \warning    Esta opci�n no existe en los PIC18F y solo existe por compatibilidad 
    ----------------------------------------------------------------------------------------------*/
    _U08 Gpios_u8ReadPullups(_eGPIOS_PORT ePort);


#endif   
 