/*--------------------------------------------------------------------------------------------------
  "THE BEER-WARE LICENSE" (Revision 42):
  <phk@FreeBSD.ORG> wrote this file. As long as you retain this notice you
  can do whatever you want with this stuff. If we meet some day, and you think
  this stuff is worth it, you can buy me a beer in return to Hotboards crew 
  (Beer-ware license created by Poul-Henning Kamp)

  \file         gpios.h
  \author       Diego
  \email        diego.perez@hotboards.org
  \ver          2.0
  \date         June 22, 2014
  \target       PIC18F series

  \brief        El driver de bajo nivel gpios agrupa una colección de funciones que permiten manejar 
                los puertos de una manera más manejable y portable. Mediante las funciones descritas 
                en el archivo gpios.h, podrás establecer la dirección, escribir y/o leer, ya sea el 
                puerto entero o pin a pin.
 -------------------------------------------------------------------------------------------------*/
#ifndef __GPIOS_H__
#define __GPIOS_H__


    /*-- Includes --*/
    #include "types.h"
     

    /*-- Defines --*/
    /*----------------------------------------------------------------------------------------------
    \def    GPIOS_INPUT/OUTPUT
    \brief  constantes para definir la dirección en la que operaran los pines de los puertos 
    ----------------------------------------------------------------------------------------------*/
    #define GPIOS_OUTPUT        0
    #define GPIOS_INPUT         1

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
    extern volatile _U08 *const guap8Direction[];
    extern volatile _U08 *const guap8Output[];
    extern volatile _U08 *const guap8Input[];

    
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
      \warning    Se debe inicializar el pin como salida antes de usar esta función      
    ----------------------------------------------------------------------------------------------*/
    void Gpios_WritePin(_eGPIOS_PORT ePort, _U08 u8Pin, _BOOL bValue);

    /**---------------------------------------------------------------------------------------------
      \brief      funcion usada para colocar en uno un pin del uC
      \param      ePort: Puerto en el que se encuentra el pin (GPIOS_PORTx)
      \param      u8Pin: Numero de pin (de 0 a 7)
      \return     None
      \warning    Se debe inicializar el pin como salida antes de usar esta función
    ----------------------------------------------------------------------------------------------*/
    #define Gpios_SetPin(ePort, u8Pin)              SET_8BIT(*guap8Output[(ePort)], (u8Pin))

    /**---------------------------------------------------------------------------------------------
      \brief      funcion usada para colocar en cero un pin del uC
      \param      ePort: Puerto en el que se encuentra el pin (GPIOS_PORTx)
      \param      u8Pin: Numero de pin (de 0 a 7)
      \return     None
      \warning    Se debe inicializar el pin como salida antes de usar esta función
    ----------------------------------------------------------------------------------------------*/
    #define Gpios_ClearPin(ePort, u8Pin)            CLEAR_8BIT(*guap8Output[(ePort)], (u8Pin))

    /**---------------------------------------------------------------------------------------------    
      \brief      Funcion usada para activar las pullups internas del uC 
      \param      ePort: Puerto en el que se encuentra el pin (GPIOS_PORTx)
      \param      u8Pin: Numero de pin (de 0 a 7)    
      \param      bValue: '1' to activar pullup 0 para desactivar    
      \return     None
      \warning    Esta opción no existe en los PIC18F y solo existe por compatibilidad     
    ----------------------------------------------------------------------------------------------*/
    void Gpios_PullupPin(_eGPIOS_PORT ePort, _U08 u8Pin, _BOOL bValue);

    /**---------------------------------------------------------------------------------------------    
      \brief      Función usada para invertir el estado de un solo Pin.
      \param      ePort: Puerto en el que se encuentra el pin (GPIOS_PORTx)
      \param      u8Pin: Numero de pin (de 0 a 7)    
      \return     None
      \warning    Se debe inicializar el pin como salida antes de usar esta función   
    ----------------------------------------------------------------------------------------------*/  
    #define Gpios_TogglePin(ePort, u8Pin)           TOGGLE_8BIT(*guap8Output[(ePort)], (u8Pin))

    /**---------------------------------------------------------------------------------------------
      \brief      Función usada para leer el valor en un pin.
      \param      ePort: Puerto en el que se encuentra el pin (GPIOS_PORTx)
      \param      u8Pin: Numero de pin (de 0 a 7)   
      \return     la función solo regresa valores de 1 o 0 
      \warning    Se debe inicializar el pin como entrada antes de usar esta función        
    ----------------------------------------------------------------------------------------------*/  
    #define Gpios_bReadPin(ePort, u8Pin)            QUERY_8BIT(*guap8Input[(ePort)], (u8Pin))

    /**---------------------------------------------------------------------------------------------
      \brief      Función usada para escribir completamente en un puerto
      \param      ePort: puerto a escribir (GPIOS_PORTx)
      \param      u8Value: valor a escribir (de 0 a 255)      
      \return     None
      \warning    se debe inicializar el puerto completo como salida
    ----------------------------------------------------------------------------------------------*/
    #define Gpios_WritePort(ePort, u8Value)         *guap8Output[(ePort)] = (u8Value)

    /**---------------------------------------------------------------------------------------------
      \brief      Función usada para leer un puerto completo
      \param      ePort: puerto a leer (GPIOS_PORTx) 
      \return     Valor leído del puerto 
      \warning    None
    ----------------------------------------------------------------------------------------------*/
    #define Gpios_u8ReadPort(ePort)                 *guap8Input[(ePort)]

    /**---------------------------------------------------------------------------------------------    
      \brief      Función usada para escribir la dirección de los pines del puerto
      \param      ePort: Nombre del puerto a configurar 
      \param      u8Value: Valor a cargar al registro TRISx
      \return     None
      \warning    None      
    ----------------------------------------------------------------------------------------------*/
    #define Gpios_WriteTris(ePort, u8Value)         *guap8Direction[(ePort)] = (u8Value)

    /**---------------------------------------------------------------------------------------------
      \brief      Funcion usada para leer la configuración de un puerto completo
      \param      ePort: Nombre del puerto
      \return     Valor interno del registro                   
      \warning    None
    ----------------------------------------------------------------------------------------------*/
    #define Gpios_u8ReadTris(ePort)                 *guap8Direction[(ePort)]

    /**---------------------------------------------------------------------------------------------
      \brief      Funcion usada para activar las resistencias de pullup de un puerto completo
      \param      ePort: Nombre del puerto
      \return     None                    
      \warning    Esta opción no existe en los PIC18F y solo existe por compatibilidad 
    ----------------------------------------------------------------------------------------------*/
    void Gpios_SetPullups(_eGPIOS_PORT ePort, _U08 u8Value);

    /**---------------------------------------------------------------------------------------------
      \brief      Funcion usada para leer las resistencias pullup activas de un puerto
      \param      ePort: Nombre del puerto 
      \return     Resistencias e pullup activas                     
      \warning    Esta opción no existe en los PIC18F y solo existe por compatibilidad 
    ----------------------------------------------------------------------------------------------*/
    _U08 Gpios_u8ReadPullups(_eGPIOS_PORT ePort);


#endif   
 