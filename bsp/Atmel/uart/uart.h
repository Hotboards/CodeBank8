/*--------------------------------------------------------------------------------------------------
  "THE BEER-WARE LICENSE" (Revision 42):
  <phk@FreeBSD.ORG> wrote this file. As long as you retain this notice you
  can do whatever you want with this stuff. If we meet some day, and you think
  this stuff is worth it, you can buy me a beer in return to Hotboards crew 
  (Beer-ware license created by Poul-Henning Kamp)

  \file         uart.h
  \author       Diego
  \email        diego.perez@hotboards.org
  \ver          1.0
  \date         August 5, 2013
  \target       PIC18FJ series

  \brief        El driver de puerto serial maneja el periférico EUART para que realize transmisiones
                de 8 y 9 bits. El driver fue creado principalmente para que este funcione mediante
                interrupciones y no sea necesario ciclar al uC de manera innecesaria, sin embargo se
                crearon un par de funciones de tx de caracteres y cadenas, para cuando se requiera
                implementar una prueba rápida o si la aplicación no requiere demasiado procesamiento
                multitarea.
 -------------------------------------------------------------------------------------------------*/
#ifndef __UART_H__
#define __UART_H__


    /*-- Includes --*/
    #include "types.h"
     

    /*-- Defines --*/
    /**---------------------------------------------------------------------------------------------
      \def      _eUARTS
      \brief    Puertos uart que se pueden usar con las funciones
    ----------------------------------------------------------------------------------------------*/
    typedef enum
    {
        UART_PORT0 = 0,
        UART_PORT1,
        UART_PORT2
    }_eUARTS;

    /**--------------------------------------------------------------------------------------------- 
      \def      UART_CLOCK
      \brief    Frecuencia de cpu que alimenta este periferico
                Esta definicion es interna unicamente, actuando solo como valor por default cuando
                no se define BSP_CLOCK en hardware_profile.h
    ----------------------------------------------------------------------------------------------*/ 
    //#define UART_CLOCK              12000000

    /**---------------------------------------------------------------------------------------------
      \def      UART_ENABLE_RX1
      \brief    Definicion para habilitar la recepcion por interrupciones, setear a 1 (no hay por poleo)
                Esta definicion es interna unicamente, actuando solo como valor por default cuando
                no se define UART_ENABLE_RX1 en hardware_profile.h
    ----------------------------------------------------------------------------------------------*/
    //#define UART_ENABLE_RX1          0

    /**---------------------------------------------------------------------------------------------
      \def      UART_ENABLE_RX2
      \brief    Definicion para habilitar la recepcion por interrupciones, setear a 1 (no hay por poleo)
                Esta definicion es interna unicamente, actuando solo como valor por default cuando
                no se define UART_ENABLE_RX2 en hardware_profile.h
    ----------------------------------------------------------------------------------------------*/
    //#define UART_ENABLE_RX2          0


    /*-- Macros --*/
    /**--------------------------------------------------------------------------------------------- 
      \def        Macro
      \brief      Write a nice description for your macros
    ----------------------------------------------------------------------------------------------*/ 
    
    
    /*-- Functions --*/
    /**---------------------------------------------------------------------------------------------
      \brief      Inicializa el periférico del puerto serial uart a la velocidad seleccionada
      \param      u32BaudRate.- velocidad de tx (9600, 57600, 115200)
      \return     El valor actual que se estableció en el registro de baudrate
      \warning    Por default la configuración es tx de 8 bits, sin paridad y 1 bit de stop (_8N1)
    ----------------------------------------------------------------------------------------------*/
    _U32 Uart_Init(const _U08 u8Uart, _U32 u32BaudRate);

    /**---------------------------------------------------------------------------------------------
      \brief      Transmite un solo carácter de 8 bits por puerto serial
      \param      u8Char.- Carácter a transmitir
      \return     None
      \warning    La función se espera a que el buffer de trasmisión este libre para mandar el carácter
    ----------------------------------------------------------------------------------------------*/
    void Uart_PutChar(const _U08 u8Uart, _U08 u8Char);

    /**---------------------------------------------------------------------------------------------
      \brief      Transmite una cadena de caracteres terminada en cero
      \param      strString.- Cadena de caracteres a transmitir
      \return     None
      \warning    la función traba al procesador hasta que la cadena es enviada por completo y solo
                  funciona con cadenas almacenadas en memoria flash
    ----------------------------------------------------------------------------------------------*/
    void Uart_PutString(const _U08 u8Uart, const _U08 *strString);

    /**---------------------------------------------------------------------------------------------
      \brief      Transmite un arreglo de datos mediante interrupciones
      \param      pu8Char.- puntero al arreglo de datos
      \param      pu8Lenght.- numero de elementos a transmitir
      \return     None
      \warning    la Función activa interrupción por tx y deja que la interrupción transmita los datos
                  la función Uart_TxBusy debe regresar cero antes de llamar esta función
    ----------------------------------------------------------------------------------------------*/
    void Uart_TxBuffer(const _U08 u8Uart, const _U08 *pu8Char, _U08 u8Lenght);

    /**---------------------------------------------------------------------------------------------
      \brief      Revisa si el puerto serial esta ocupado trasmitiendo
      \param      None
      \return     Regresa un uno si esta ocupado y un cero si esta libre para tx
      \warning    None
    ----------------------------------------------------------------------------------------------*/
    _BOOL Uart_TxBusy(const _U08 u8Uart);

    /**---------------------------------------------------------------------------------------------
      \brief      Función callback mandada llamar por la función Uart1_RxIsr, es necesario que la
                  aplicación defina esta funcion y establezca que haser con el parametro que llegue
      \param      u8Data.- dato de 8 bits llegado por puerto serial 1
      \return     None
      \warning    Es importante recordar que esta funcion se ejecuta dentro de la interrupcion por rx
                  y debe ser lo mas eficiente posible. se debe definir en bsp_profile.h
                  #define UART_ENABLE_RX1          1
    ----------------------------------------------------------------------------------------------*/
    extern void Uart0_CallbackRx(_U08 u8Data);

    /**---------------------------------------------------------------------------------------------
      \brief      Función callback mandada llamar por la función Uart2_RxIsr, es necesario que la
                  aplicación defina esta funcion y establezca que haser con el parametro que llegue
      \param      u8Data.- dato de 8 bits llegado por puerto serial 2
      \return     None
      \warning    Es importante recordar que esta funcion se ejecuta dentro de la interrupcion por rx
                  y debe ser lo mas eficiente posible. se debe definir en bsp_profile.h
                  #define UART_ENABLE_RX2          1
    ----------------------------------------------------------------------------------------------*/
    extern void Uart2_CallbackRx(_U08 u8Data);

    /**---------------------------------------------------------------------------------------------
      \brief      Regresa un carácter llegado del puerto serial
      \param      None
      \return     Carácter llegado del puerto serial
      \warning    Esta función traba al procesador hasta recibir un carácter por serial. Es
                  conveniente solo utilizarla para propósitos de depuración y pruebas.
    ----------------------------------------------------------------------------------------------*/
    _U08 Uart_u8GetChar(const _U08 u8Uart);


#endif   

