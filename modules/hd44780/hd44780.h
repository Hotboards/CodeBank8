/*--------------------------------------------------------------------------------------------------
  "THE BEER-WARE LICENSE" (Revision 42):
  <phk@FreeBSD.ORG> wrote this file. As long as you retain this notice you
  can do whatever you want with this stuff. If we meet some day, and you think
  this stuff is worth it, you can buy me a beer in return to Hotboards crew
  (Beer-ware license created by Poul-Henning Kamp)

  \file         hd44780.h
  \author       Diego
  \email        diego.perez@hotboards.org
  \ver          1.0
  \date         August 29, 2013
  \target       8-bits Generic

  \brief        Pieza de código para controlar un lcd alfanumérico externo con el controlador Hitachi
                hd44780 o compatible. este código solo se encarga de incializar el lcd, así como mandar
                comandos y/o datos al lcd, los detalles internos de comunicación se manejan a través
                del código que maneja la interfaz 6800 en 4 u 8 bits.
 -------------------------------------------------------------------------------------------------*/
#ifndef __HD44780_H__
#define __HD44780_H__


    /*-- Includes --*/
    #include "types.h"
     

    /*-- Defines --*/
    /**--------------------------------------------------------------------------------------------- 
      \def      HD44780_ROWS
      \brief    numero de filas presentes en el display, esta definicion debera
                colocarse en el archivo hardware_profile.h
    ----------------------------------------------------------------------------------------------*/ 
    //#define HD44780_ROWS                    4

    /**--------------------------------------------------------------------------------------------- 
      \def      HD44780_COLUMNS
      \brief    en el numero de columnas presentes en el display, esta definicion debera
                colocarse en el archivo hardware_profile.h
    ----------------------------------------------------------------------------------------------*/ 
    //#define HD44780_COLUMNS                 20


    /*-- Macros --*/
    /**--------------------------------------------------------------------------------------------- 
      \def      Define
      \brief    Write a nice description for your macros
    ----------------------------------------------------------------------------------------------*/ 
    
    
    /*-- Functions --*/
    /**---------------------------------------------------------------------------------------------    
      \brief      Inicializa el controlador hd44780, sin cursor, dos lineas y fuente 5x8
      \param	  None
      \return     None
      \warning	  Esta funcion usa de retardos y ciclara al uC al menos 70ms
    ----------------------------------------------------------------------------------------------*/
    void HD44780_Init(void);
    
    /**---------------------------------------------------------------------------------------------    
      \brief      pregunta al controlador si esta ocupado o puede recivir otro dato/comando
      \param	  None
      \return     '1' si esta ocupado
      \warning	  None
    ----------------------------------------------------------------------------------------------*/
    _BOOL HD44780_bBusyFlag(void);
    
    /**---------------------------------------------------------------------------------------------    
      \brief      mueve el curso a columna y fila especifica
      \param	  u8Row.- fila (de 0 a HD44780_ROWS-1)
      \param	  u8Column.- columna (de 0 a HD44780_COLUMNS-1)
      \return     None
      \warning	  Trava al uC por espacio de 40us
    ----------------------------------------------------------------------------------------------*/
    void HD44780_SetCursor(const _U08 u8Row, const _U08 u8Col);

    /**---------------------------------------------------------------------------------------------    
      \brief      manda un simple dato a la memoria interna de datos
      \param	  u8Data.- dato a enviar
      \return     None
      \warning	  trava al uC por 40us
    ----------------------------------------------------------------------------------------------*/
    void HD44780_WriteData(const _U08 u8Data);

    /**---------------------------------------------------------------------------------------------    
      \brief      manda comandos a controlador 
      \param      u8Data.- comando a enviar (revisar hoja de datos para posibles comandos)
      \return     None
      \warning    trava al uC por 40us
    ----------------------------------------------------------------------------------------------*/
    void HD44780_WriteCommand(const _U08 u8Data);

    /**---------------------------------------------------------------------------------------------    
      \brief      Escribe una cadena terminada en cero, almacenada en flash,
      \param	  strString.- puntero a cadena terminada en cero
      \return     None
      \warning	  Trava al uC 40us por cada caracter enviado
    ----------------------------------------------------------------------------------------------*/
    void HD44780_WriteString(const rom _S08 *strString);


#endif   
 