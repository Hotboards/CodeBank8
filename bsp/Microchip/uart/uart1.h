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
      \def      UART1_CLOCK
      \brief    Frecuencia de cpu que alimenta este periferico
                Esta definicion es interna unicamente, actuando solo como valor por default cuando
                no se define BSP_CLOCK en hardware_profile.h
    ----------------------------------------------------------------------------------------------*/ 
    //#define UART1_CLOCK              12000000

    /**---------------------------------------------------------------------------------------------
      \def      UART1_ENABLE_RX
      \brief    Definicion para habilitar la recepcion por interrupciones, setear a 1 (no hay por poleo)
                Esta definicion es interna unicamente, actuando solo como valor por default cuando
                no se define UART1_ENABLE_RX en hardware_profile.h
    ----------------------------------------------------------------------------------------------*/
    //#define UART1_ENABLE_RX          0

    /**---------------------------------------------------------------------------------------------
      \def      UART1_TX_BUFFER
      \brief    Se indica la cantidad maxima de datos que se pueden trasmitir por interrupciones
                Esta definicion es interna unicamente, actuando solo como valor por default cuando
                no se define UART1_TX_BUFFER en hardware_profile.h
    ----------------------------------------------------------------------------------------------*/
    //#define UART1_TX_BUFFER          30


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
    _U32 Uart1_Init(const _U32 u32BaudRate);

    /**---------------------------------------------------------------------------------------------
      \brief      Cambia la prioridad de interrupción de Tx.
      \param      bPriority.- prioridad _HIGH o _LOW
      \return     None
      \warning    Por default la función Uart_Init  pone la prioridad en _LOW
    ----------------------------------------------------------------------------------------------*/
    void Uart1_TxInterruptProprity(const _BOOL bPriority);

    /**---------------------------------------------------------------------------------------------
      \brief      Cambia la prioridad de interrupción de Rx.
      \param      bPriority.- prioridad _HIGH o _LOW
      \return     None
      \warning    Por default la función Uart_Init  pone la prioridad en _LOW
    ----------------------------------------------------------------------------------------------*/
    void Uart1_RxInterruptProprity(const _BOOL bPriority);

    /**---------------------------------------------------------------------------------------------
      \brief      Transmite un solo carácter de 8 bits por puerto serial
      \param      u8Char.- Carácter a transmitir
      \return     None
      \warning    La función se espera a que el buffer de trasmisión este libre para mandar el carácter
    ----------------------------------------------------------------------------------------------*/
    void Uart1_PutChar(const _U08 u8Char);

    /**---------------------------------------------------------------------------------------------
      \brief      Transmite una cadena de caracteres terminada en cero
      \param      strString.- Cadena de caracteres a transmitir
      \return     None
      \warning    la función traba al procesador hasta que la cadena es enviada por completo y solo
                  funciona con cadenas almacenadas en memoria flash
    ----------------------------------------------------------------------------------------------*/
    void Uart1_PutString(const rom _S08 *strString);

    /**---------------------------------------------------------------------------------------------
      \brief      Transmite un arreglo de datos mediante interrupciones
      \param      pu8Char.- puntero al arreglo de datos
      \param      pu8Lenght.- numero de elementos a transmitir
      \return     None
      \warning    la Función activa interrupción por tx y deja que la interrupción transmita los datos
                  la función Uart_TxBusy debe regresar cero antes de llamar esta función
    ----------------------------------------------------------------------------------------------*/
    void Uart1_TxBuffer(const _U08 *pu8Char, const _U08 u8Lenght);

    /**---------------------------------------------------------------------------------------------
      \brief      Transmite un arreglo de datos almacenados en memoria flash mediante interrupciones
      \param      pu8Char.- puntero al arreglo de datos
      \param      pu8Lenght.- numero de elementos a transmitir
      \return     None
      \warning    la Función activa interrupción por tx y deja que la interrupción transmita los datos
                  la función Uart_TxBusy debe regresar cero antes de llamar esta función
    ----------------------------------------------------------------------------------------------*/

    void Uart1_TxFlashBuffer(const rom _U08 *pu8Char, const _U08 u8Lenght);
    /**---------------------------------------------------------------------------------------------
      \brief      Revisa si el puerto serial esta ocupado trasmitiendo
      \param      None
      \return     Regresa un uno si esta ocupado y un cero si esta libre para tx
      \warning    None
    ----------------------------------------------------------------------------------------------*/
    _BOOL Uart1_TxBusy(void);

    /**---------------------------------------------------------------------------------------------
      \brief      Función de interrupción por transmisión de datos en puerto serial, esta función es
                  complemento de las función Uart1_TxBuffer y Uart1_TxFlashBuffer
      \param      None
      \return     None
      \warning    Indispensable mandar llamar esta función en alguno de los vectores de interrupción
    ----------------------------------------------------------------------------------------------*/
    void Uart1_TxIsr(void);

    /**---------------------------------------------------------------------------------------------
      \brief      Función de interrupción por recepción en puerto serial, lo único que se realiza aquí
                  es mandar llamar la funcion Uart1_CallbackRx y pasarle el dato llegado
      \param      None
      \return     None
      \warning    Indispensable mandar llamar esta función en alguno de los vectores de interrupción
    ----------------------------------------------------------------------------------------------*/
    void Uart1_RxIsr(void);

    /**---------------------------------------------------------------------------------------------
      \brief      Función callback mandada llamar por la función Uart1_RxIsr, es necesario que la
                  aplicación defina esta funcion y establezca que haser con dad parametro que llegue
      \param      u8Data.- dato de 8 bits llegado por puerto serial
      \return     None
      \warning    Es importante recordar que esta funcion se ejecuta dentro de la interrupcion por rx
                  y debe ser lo mas eficiente posible. se debe definir en bsp_profile.h
                  #define UART1_ENABLE_RX          1
    ----------------------------------------------------------------------------------------------*/
    extern void Uart1_CallbackRx(_U08 u8Data);

    /**---------------------------------------------------------------------------------------------
      \brief      Regresa un carácter llegado del puerto serial
      \param      None
      \return     Carácter llegado del puerto serial
      \warning    Esta función traba al procesador hasta recibir un carácter por serial. Es
                  conveniente solo utilizarla para propósitos de depuración y pruebas.
    ----------------------------------------------------------------------------------------------*/
    _U08 Uart1_u8GetChar(void);


#endif   

