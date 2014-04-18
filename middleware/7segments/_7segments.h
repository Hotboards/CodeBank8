/*--------------------------------------------------------------------------------------------------
  "THE BEER-WARE LICENSE" (Revision 42):
  <phk@FreeBSD.ORG> wrote this file. As long as you retain this notice you
  can do whatever you want with this stuff. If we meet some day, and you think
  this stuff is worth it, you can buy me a beer in return to Hotboards crew
  (Beer-ware license created by Poul-Henning Kamp)

  \file         _7segments.h
  \author       Diego
  \email        diego.perez@hotboards.org
  \ver          1.0
  \date         April 17, 2014
  \target       8-bits Generic

  \brief        Esta pieza de código controla el multiplexado de display de 7 segemtnos mediante un 
                bus de datos comun. eL driver puede controlar de uno hasta cuatro display en
                configuracion anodo comun o catodo comun, requiere de un puerto completo para el bus
                de datos y un pin por cada display.
                Es necesario mandar llamar una fucion periodica al menos cada 5ms para 4 displays.
                Una simple funcion se encarga de setear los alorse que se desplegaran por display, lo
                que pemrite desplegar cualquier caracter que se desee combiando los 7 segmentos
                (mas un punto), donde los segmentos a = bit0, b = bit1, ..., g = bit6 y . = bit7
                            a
                           ---
                        f | g | b
                           ---
                        e | d | c
                           --- .
 -------------------------------------------------------------------------------------------------*/
#ifndef __7SEGMENTS__H__
#define __7SEGMENTS__H__


    /*-- Includes --*/
    #include "types.h"
     

    /*-- Defines --*/
    /**---------------------------------------------------------------------------------------------
      \def      _7SEGMENTS_DIGI_N
      \brief    Numero de digitos que conforman el display. maximo 4, minimo 1
                Es obligatorio escribir esta definicion en el archivo hardware_profile.h
    ----------------------------------------------------------------------------------------------*/
    //#define _7SEGMENTS_DIGI_N                   1

    /**---------------------------------------------------------------------------------------------
      \def      _7SEGMENTS_MODE
      \brief    Seleciona si los display son anodo(1) o catodo(0) comun, por default se toman como
                catodo comun
                Es obligatorio escribir esta definicion en el archivo hardware_profile.h
    ----------------------------------------------------------------------------------------------*/
    //#define _7SEGMENTS_MODE                     0

    /**---------------------------------------------------------------------------------------------
      \def      _7SEGMENTS_PORT
      \brief    puerto completo que se encargara de mandar los datos a los display
                Es obligatorio escribir esta definicion en el archivo hardware_profile.h
    ----------------------------------------------------------------------------------------------*/
    //#define _7SEGMENTS_PORT                     GPIOS_PORTB

    /**--------------------------------------------------------------------------------------------- 
      \def      _7SEGMENTS_DIGIx_n
      \brief    pin utilizado para controlar cada uno de los digitos
                Es obligatorio escribir esta definicion en el archivo hardware_profile.h
    ----------------------------------------------------------------------------------------------*/ 
    //#define _7SEGMENTS_DIGI0_P                  GPIOS_PORTD
    //#define _7SEGMENTS_DIGI0_B                  4
    //(repetir hasta el numero 3 si es necesario)

    /**---------------------------------------------------------------------------------------------
      \def      _e7SEGMENTS_NUMBERS
      \brief    Valores que representan los numeros decimales desplegados en los displays
    ----------------------------------------------------------------------------------------------*/
    typedef enum
    {
        _0 = 0b00111111,
        _1 = 0b00000110,
        _2 = 0b01011011,
        _3 = 0b01001111,
        _4 = 0b01100110,
        _5 = 0b01101101,
        _6 = 0b01111101,
        _7 = 0b00000111,
        _8 = 0b01111111,
        _9 = 0b01101111
    }_e7SEGMENTS_NUMBERS;

    
    /*-- Macros --*/
    /**--------------------------------------------------------------------------------------------- 
      \def      Define
      \brief    Write a nice description for your macros
    ----------------------------------------------------------------------------------------------*/ 
    
    
    /*-- Functions --*/
    /**---------------------------------------------------------------------------------------------
      \brief      Inicializa como salidas los pines que se usaran con el display
      \param	  None
      \return     None
      \warning	  Se deben declarar los pines a usar en el archivo middleware_profile.h
    ----------------------------------------------------------------------------------------------*/
    void _7segments_Init(void);

    /**---------------------------------------------------------------------------------------------
      \brief      Carga el valor con los segemtnos a encender en cada display
      \param	  u8Display.- numero de display uqe desplegara el valor seleccionado (de 0 a _7SEGMENTS_DIGI_N-1)
      \param	  u8Value.- valor con los segmentos que se deberan encender
      \return     None
      \warning	  Un bit en uno represetna un segmento encendido. La funcion invertira el valor si el
                  el driver esta configurado como anodo comun.
    ----------------------------------------------------------------------------------------------*/
    void _7segments_SetDisplay(const _U08 u8Display, const _U08 u8Value);

    /**---------------------------------------------------------------------------------------------
      \brief      Regresa el valor con los segemtnos a encendidos en el display
      \param	  u8Display.- numero de display que se le obtendra su valor (de 0 a _7SEGMENTS_DIGI_N-1)
      \return     Valor en binario representando cada segmento encendido
      \warning	  Un bit en uno represetna un segmento encendido.
    ----------------------------------------------------------------------------------------------*/
    _U08 _7segments_u8GetDisplay(const _U08 u8Display);

    /**---------------------------------------------------------------------------------------------
      \brief      Despliega un numero decimal de hasta 4 digitos en los displays
      \param	  u16Value.- numero a desplegar en los displays
      \return     None
      \warning	  La funcion toma en cuenta el numero de displays que se ha configurado. No se toma
                  en cuenta el punto
    ----------------------------------------------------------------------------------------------*/
    void _7segments_SetNumber(const _U16 u16Value);

    /**---------------------------------------------------------------------------------------------
      \brief      Actualiza el valor en los display y multiplexa al siguiente display
      \param	  None
      \return     None
      \warning	  Esta funcion se deb mandar llamar de manera periodica al menos cada 5ms
    ----------------------------------------------------------------------------------------------*/
    void _7segments_Task(void);

    
#endif   
