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
#ifndef __PRINTF_H__
#define __PRINTF_H__


    /*-- Includes --*/
    #include "types.h"
     

    /*-- Defines --*/          

    
    /*-- Macros --*/
    /**--------------------------------------------------------------------------------------------- 
      \def        Macro
      \brief      Write a nice description for your macros
    ----------------------------------------------------------------------------------------------*/ 
    
    
    /*-- Functions --*/
    /**---------------------------------------------------------------------------------------------
      \brief      Establece el sistema de salida que usaran el resto de las funciones por default
      \param      func.- puntero a funci�n que se encarga de mandar un car�cter por vez a la salida
      \return     None
      \warning    Tipicamente usas una funci�n "putc"
    ----------------------------------------------------------------------------------------------*/
    void xdev_out( void(*func)(const unsigned char) );

    /**---------------------------------------------------------------------------------------------
      \brief      Manda un car�cter por pantalla
      \param      c.- car�cter a mandar a la salida
      \return     None
      \warning    None
    ----------------------------------------------------------------------------------------------*/
    void xputc(char c);

    /**---------------------------------------------------------------------------------------------
      \brief      Manda una cadena de caracteres terminada en cero por pantalla
      \param      str.- puntero a cadena de caracteres
      \return     None
      \warning    Esta funci�n solo trabaja con cadenas almacenadas en flash
    ----------------------------------------------------------------------------------------------*/
    void xputs(const char *str );

    /**---------------------------------------------------------------------------------------------
      \brief      Manda una cadena de caracteres terminada en cero a trav�s de una salida especifica
      \param      func.- salida especifica para mandar los caracteres
      \param      str.- puntero a cadena de caracteres
      \return     None
      \warning    Esta funci�n solo trabaja con cadenas almacenadas en flash
    ----------------------------------------------------------------------------------------------*/
    void xfputs(void(*func)(const unsigned char), const char *str);

    /**---------------------------------------------------------------------------------------------
      \brief      Emulaci�n de funci�n printf
      \param      fmt.- puntero a cadena de caracteres
      \param      ...	par�metros opcionales
      \return     None
      \warning    Esta funci�n solo trabaja con cadenas almacenadas en flash
    ----------------------------------------------------------------------------------------------*/
    void xprintf(const char *fmt, ...);

    /**---------------------------------------------------------------------------------------------
      \brief      Almacena la cadena de salida formateada en un buffer de memoria
      \param      buff.- puntero a buffer donde se almacenara la cadena de caracteres
      \param      fmt.- puntero a cadena de caracteres
      \param      ...	par�metros opcionales
      \return     None
      \warning    Esta funci�n solo trabaja con cadenas almacenadas en flash
    ----------------------------------------------------------------------------------------------*/
    void xsprintf(char* buff, const char* fmt, ...);

    /**---------------------------------------------------------------------------------------------
      \brief      Similar a la funci�n printf, solo que aqu� se indica la opci�n de salida de caracteres
      \param      func.- funci�n que se usara para desplegar los caracteres
      \param      fmt.- puntero a cadena de caracteres
      \param      ...	par�metros opcionales
      \return     None
      \warning    Esta funci�n solo trabaja con cadenas almacenadas en flash
    ----------------------------------------------------------------------------------------------*/
    void xfprintf(void (*func)(const unsigned char), const char *fmt, ...);

    /**---------------------------------------------------------------------------------------------
      \brief      Despliega el contenido de memoria apuntado
      \param      buff.- puntero al buffer que contiene los datos a desplegar
      \param      addr.- direcci�n de inicio de los datos
      \param      len.-	cantidad de datos a desplegar
      \param      width.- ancho de la palabra de los datos (bytes)
      \return     None
      \warning    para el ultimo par�metros utilizar sizeof(char),sizeof(short) o sizeof(long)
    ----------------------------------------------------------------------------------------------*/
    void put_dump(const void *buff, unsigned long addr, int len, int width);


#endif   
 