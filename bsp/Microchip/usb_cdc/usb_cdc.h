/*--------------------------------------------------------------------------------------------------
  "THE BEER-WARE LICENSE" (Revision 42):
  <phk@FreeBSD.ORG> wrote this file. As long as you retain this notice you
  can do whatever you want with this stuff. If we meet some day, and you think
  this stuff is worth it, you can buy me a beer in return to Hotboards crew
  (Beer-ware license created by Poul-Henning Kamp)

  \file         usb_cdc.h
  \author       Diego
  \email        diego.perez@hotboards.org
  \ver          2.0
  \date         July 25, 2014
  \target       PIC18F series

  \brief        Este driver maneja la operacion del periferico USB en modo CDC es decir emula una
                una comunicacion serial. Este archivo funciona como un wrapper entre el stack oficial
                de Microchip y la libraria oficial de Hotboards, simplificando en gran medida la
                implementacion anterior. Es necesario que el micro corra a una velocidad de 48MHz
 -------------------------------------------------------------------------------------------------*/
#ifndef _USB_CDC_H_
#define	_USB_CDC_H_


    /*-- Includes --*/
    #include "types.h"


    /*-- Functions --*/
    /**---------------------------------------------------------------------------------------------
      \brief      Inicaliza la modulo usb y configura en clase CDC
      \param      u32Baud.- velocidad serial con la que sera enmulado
      \return     None
      \warning    None
    ----------------------------------------------------------------------------------------------*/
    void UsbCDC_Init( _U32 u32Baud );

    /**---------------------------------------------------------------------------------------------
      \brief      Alerta si el modulo usb a sido enumerado por el host al que se conecta
      \param      None
      \return     _TRUE si el dispositivo esta enumerado
      \warning    Es necesario manda llaamr esta funcion cada que se conecte a un host
    ----------------------------------------------------------------------------------------------*/
    _BOOL UsbCDC_bEnumerated( void );

    /**---------------------------------------------------------------------------------------------
      \brief      Tarea periodica encargada de dar servicio al modulo USB
      \param      None
      \return     None
      \warning    Es necesario llamar esta funcion cada 1.8ms, despues de que el dispositivo esta
                  enumerado, se puyede llamar cada 5ms. Si se usan interrupciones, se requiere
                  colocar esta funcion en un vector de interrupcion
    ----------------------------------------------------------------------------------------------*/
    void UsbCDC_Task(void);

    /**---------------------------------------------------------------------------------------------
      \brief      Alerta si es posible mandar datos por medio del usb
      \param      None
      \return     _TRUE si el dsipositivo no esta ocupado
      \warning    None
    ----------------------------------------------------------------------------------------------*/
    _BOOL UsbCDC_bReady( void );

    /**---------------------------------------------------------------------------------------------
      \brief      Manda una cadena terminada en cero por usb
      \param      strString.- puntero a cadea de caracteres
      \return     None
      \warning    Esta funcion solo inicia la transmision, es necesario llamar la funcion UsbCDC_bReady
                  para saber si ya termino de mandar la cadena
    ----------------------------------------------------------------------------------------------*/
    void UsbCDC_SendString( char *strString );

    /**---------------------------------------------------------------------------------------------
      \brief      Manda una serie de datos (incluido el cero) por usb
      \param      puBuffer.- puntero al arreglo donde se encuntran los datos a mandar
      \param      u8Lenght.- cantidad de datos a enviar
      \return     None
      \warning    Esta funcion solo inicia la transmision, es necesario llamar la funcion UsbCDC_bReady
                  para saber si ya termino de mandar los datos
    ----------------------------------------------------------------------------------------------*/
    void UsbCDC_TxData( _U08 *pu8Buffer, _U08 u8Lenght );

    /**---------------------------------------------------------------------------------------------
      \brief      Recive una serie de bytes por el puerto usb
      \param      pu8Buffer.- puntero a arreglo en onde se almacenara los datos recividos
      \param      u8Lenght.- cantidad de datso que se esperan recivir
      \return     numero de bytes que efectivamente se recivieron
      \warning    Esta funcion ciclara al uC por espacio de 2ms
    ----------------------------------------------------------------------------------------------*/
    _U08 UsbCDC_u8RxData( _U08 *pu8Buffer, _U08 u8Lenght );

    
#endif	/* USB_CDC_H */

