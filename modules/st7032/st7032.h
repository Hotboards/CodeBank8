/*--------------------------------------------------------------------------------------------------
  "THE BEER-WARE LICENSE" (Revision 42):
  <phk@FreeBSD.ORG> wrote this file. As long as you retain this notice you
  can do whatever you want with this stuff. If we meet some day, and you think
  this stuff is worth it, you can buy me a beer in return to Hotboards crew
  (Beer-ware license created by Poul-Henning Kamp)

  \file         st7032.h
  \author       Diego
  \email        diego.perez@hotboards.org
  \ver          1.0
  \date         Febrero 25, 2014
  \target       8-bits Generic

  \brief        Pieza de código para controlar un lcd alfanumérico externo con el controlador Sitronix
                ST7032 el cual posee una interfaz serial spi, su funcionamiento es muy parecido
                al controlador hitachi hd44780 solo que este esta limitado a 2x16 caracteres,
                pero coincide en los comandos que se le pueden enviar.
                al controlarse mediante una interfaz multiesclavo, se puede llegar a controlar mas
                de un lcd mediante esta pieza de codigo. (Es necesario incializar antes el modulo
                spi usado con el lcd)
 -------------------------------------------------------------------------------------------------*/
#ifndef __ST7032_H__
#define __ST7032_H__


    /*-- Includes --*/
    #include "types.h"
     

    /*-- Defines --*/
    /**---------------------------------------------------------------------------------------------
      \def        _eST7032_MEMORIES
      \brief      Displays que pueden ser manejadas por esta pieza de codigo
    ----------------------------------------------------------------------------------------------*/
    typedef enum
    {
        ST7032_LCD0 = 0,
        ST7032_LCD1,
        ST7032_LCD2,
        ST7032_LCD3
    }_eST7032_MEMORIES;


    /*-- Macros --*/
    /**--------------------------------------------------------------------------------------------- 
      \def      Define
      \brief    Write a nice description for your macros
    ----------------------------------------------------------------------------------------------*/ 
    
    
    /*-- Functions --*/
    /**---------------------------------------------------------------------------------------------    
      \brief      Inicializa el controlador st7032, sin cursor, dos lineas y fuente 5x8
      \param	  None
      \return     None
      \warning	  Esta funcion usa de retardos y ciclara al uC al menos 230ms
    ----------------------------------------------------------------------------------------------*/
    void ST7032_Init(const _U08 u8Lcd);
    
    /**---------------------------------------------------------------------------------------------    
      \brief      mueve el curso a columna y fila especifica
      \param	  u8Row.- fila (0 o 1)
      \param	  u8Column.- columna (de 0 a 15)
      \return     None
      \warning	  Trava al uC por espacio de 30us
    ----------------------------------------------------------------------------------------------*/
    void ST7032_SetCursor(const _U08 u8Lcd, const _U08 u8Row, const _U08 u8Col);

    /**---------------------------------------------------------------------------------------------
      \brief      manda un comando al controlador
      \param	  u8Data.- dato a enviar
      \return     None
      \warning	  trava al uC por 30us exepto si es un clear screen, le tomara 300uS
    ----------------------------------------------------------------------------------------------*/
    void ST7032_WriteCommand(const _U08 u8Lcd, const _U08 u8Data);

    /**---------------------------------------------------------------------------------------------    
      \brief      manda un simple dato a la memoria interna de datos
      \param	  u8Data.- dato a enviar
      \return     None
      \warning	  trava al uC por 30us
    ----------------------------------------------------------------------------------------------*/
    void ST7032_WriteData(const _U08 u8Lcd, const _U08 u8Data);

    /**---------------------------------------------------------------------------------------------    
      \brief      Escribe una cadena terminada en cero, almacenada en flash,
      \param	  strString.- puntero a cadena terminada en cero
      \return     None
      \warning	  Trava al uC 30us por cada caracter enviado
    ----------------------------------------------------------------------------------------------*/
    void ST7032_WriteString(const _U08 u8Lcd, const rom _S08 *strString);


#endif   
 