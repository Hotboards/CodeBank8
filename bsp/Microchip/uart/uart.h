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

  \brief        El driver de puerto serial maneja el perif�rico EUART para que realize transmisiones
                de 8 y 9 bits. El driver fue creado principalmente para que este funcione mediante
                interrupciones y no sea necesario ciclar al uC de manera innecesaria, sin embargo se
                crearon un par de funciones de tx de caracteres y cadenas, para cuando se requiera
                implementar una prueba r�pida o si la aplicaci�n no requiere demasiado procesamiento
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
        UART_PORT1 = 1,
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
      \brief      Inicializa el perif�rico del puerto serial uart a la velocidad seleccionada
      \param      u32BaudRate.- velocidad de tx (9600, 57600, 115200)
      \return     El valor actual que se estableci� en el registro de baudrate
      \warning    Por default la configuraci�n es tx de 8 bits, sin paridad y 1 bit de stop (_8N1)
    ----------------------------------------------------------------------------------------------*/
    _U32 Uart_Init(const _U08 u8Uart, _U32 u32BaudRate);

    /**---------------------------------------------------------------------------------------------
      \brief      Cambia la prioridad de interrupci�n de Tx.
      \param      bPriority.- prioridad _HIGH o _LOW
      \return     None
      \warning    Por default la funci�n Uart_Init  pone la prioridad en _LOW
    ----------------------------------------------------------------------------------------------*/
    void Uart_TxInterruptProprity(const _U08 u8Uart, const _BOOL bPriority);

    /**---------------------------------------------------------------------------------------------
      \brief      Cambia la prioridad de interrupci�n de Rx.
      \param      bPriority.- prioridad _HIGH o _LOW
      \return     None
      \warning    Por default la funci�n Uart_Init  pone la prioridad en _LOW
    ----------------------------------------------------------------------------------------------*/
    void Uart_RxInterruptProprity(const _U08 u8Uart, const _BOOL bPriority);

    /**---------------------------------------------------------------------------------------------
      \brief      Transmite un solo car�cter de 8 bits por puerto serial
      \param      u8Char.- Car�cter a transmitir
      \return     None
      \warning    La funci�n se espera a que el buffer de trasmisi�n este libre para mandar el car�cter
    ----------------------------------------------------------------------------------------------*/
    void Uart_PutChar(const _U08 u8Uart, _U08 u8Char);

    /**---------------------------------------------------------------------------------------------
      \brief      Transmite una cadena de caracteres terminada en cero
      \param      strString.- Cadena de caracteres a transmitir
      \return     None
      \warning    la funci�n traba al procesador hasta que la cadena es enviada por completo y solo
                  funciona con cadenas almacenadas en memoria flash
    ----------------------------------------------------------------------------------------------*/
    void Uart_PutString(const _U08 u8Uart, const _U08 *strString);

    /**---------------------------------------------------------------------------------------------
      \brief      Transmite un arreglo de datos mediante interrupciones
      \param      pu8Char.- puntero al arreglo de datos
      \param      pu8Lenght.- numero de elementos a transmitir
      \return     None
      \warning    la Funci�n activa interrupci�n por tx y deja que la interrupci�n transmita los datos
                  la funci�n Uart_TxBusy debe regresar cero antes de llamar esta funci�n
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
      \brief      Funci�n de interrupci�n por transmisi�n de datos en puerto serial, esta funci�n es
                  complemento de las funci�nes Uart_TxBuffer y Uart_TxFlashBuffer cuando se usa
                  el UART_PORT1
      \param      None
      \return     None
      \warning    Indispensable mandar llamar esta funci�n en alguno de los vectores de interrupci�n
    ----------------------------------------------------------------------------------------------*/
    void Uart1_TxIsr(void);

    /**---------------------------------------------------------------------------------------------
      \brief      Funci�n de interrupci�n por transmisi�n de datos en puerto serial, esta funci�n es
                  complemento de las funci�nes Uart_TxBuffer y Uart_TxFlashBuffer cuando se usa
                  el UART_PORT2
      \param      None
      \return     None
      \warning    Indispensable mandar llamar esta funci�n en alguno de los vectores de interrupci�n
    ----------------------------------------------------------------------------------------------*/
    void Uart2_TxIsr(void);

    /**---------------------------------------------------------------------------------------------
      \brief      Funci�n de interrupci�n por recepci�n en puerto serial, lo �nico que se realiza aqu�
                  es mandar llamar la funcion Uart1_CallbackRx y pasarle el dato llegado
      \param      None
      \return     None
      \warning    Indispensable mandar llamar esta funci�n en alguno de los vectores de interrupci�n
    ----------------------------------------------------------------------------------------------*/
    void Uart1_RxIsr(void);

    /**---------------------------------------------------------------------------------------------
      \brief      Funci�n de interrupci�n por recepci�n en puerto serial, lo �nico que se realiza aqu�
                  es mandar llamar la funcion Uart2_CallbackRx y pasarle el dato llegado
      \param      None
      \return     None
      \warning    Indispensable mandar llamar esta funci�n en alguno de los vectores de interrupci�n
    ----------------------------------------------------------------------------------------------*/
    void Uart2_RxIsr(void);

    /**---------------------------------------------------------------------------------------------
      \brief      Funci�n callback mandada llamar por la funci�n Uart1_RxIsr, es necesario que la
                  aplicaci�n defina esta funcion y establezca que haser con el parametro que llegue
      \param      u8Data.- dato de 8 bits llegado por puerto serial 1
      \return     None
      \warning    Es importante recordar que esta funcion se ejecuta dentro de la interrupcion por rx
                  y debe ser lo mas eficiente posible. se debe definir en bsp_profile.h
                  #define UART_ENABLE_RX1          1
    ----------------------------------------------------------------------------------------------*/
    extern void Uart1_CallbackRx(_U08 u8Data);

    /**---------------------------------------------------------------------------------------------
      \brief      Funci�n callback mandada llamar por la funci�n Uart2_RxIsr, es necesario que la
                  aplicaci�n defina esta funcion y establezca que haser con el parametro que llegue
      \param      u8Data.- dato de 8 bits llegado por puerto serial 2
      \return     None
      \warning    Es importante recordar que esta funcion se ejecuta dentro de la interrupcion por rx
                  y debe ser lo mas eficiente posible. se debe definir en bsp_profile.h
                  #define UART_ENABLE_RX2          1
    ----------------------------------------------------------------------------------------------*/
    extern void Uart2_CallbackRx(_U08 u8Data);

    /**---------------------------------------------------------------------------------------------
      \brief      Regresa un car�cter llegado del puerto serial
      \param      None
      \return     Car�cter llegado del puerto serial
      \warning    Esta funci�n traba al procesador hasta recibir un car�cter por serial. Es
                  conveniente solo utilizarla para prop�sitos de depuraci�n y pruebas.
    ----------------------------------------------------------------------------------------------*/
    _U08 Uart_u8GetChar(const _U08 u8Uart);


#endif   

