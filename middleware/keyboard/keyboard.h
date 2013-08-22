/*--------------------------------------------------------------------------------------------------
  "THE BEER-WARE LICENSE" (Revision 42):
  <phk@FreeBSD.ORG> wrote this file. As long as you retain this notice you
  can do whatever you want with this stuff. If we meet some day, and you think
  this stuff is worth it, you can buy me a beer in return to Hotboards crew
  (Beer-ware license created by Poul-Henning Kamp)

  \file         keyboard.h
  \author       Diego
  \email        diego.perez@hotboards.org
  \ver          1.0
  \date         August 21, 2013
  \target       Generic 8-bits uC

  \brief        Este código permite manejar un teclado matricial de n x m, es decir la cantidad de
                lineas y columnas son configurables de 1 a 8, lo cual permite manejar un máximo de 64
                botones en el teclado. El driver se utiliza únicamente por poleo y es necesario mandar
                llamar una función periódica cada x tiempo. Ademas es configurable por el usuario el
                tiempo de debounce y los pines a los cuales se puede conectar.
            
                Example:

                Keyboard_Init();

                for(;;)
                {
                    Keyboard_Task();
                    if(Keyboard_bIsPresed()==1)
                    {
                        key = Keyboard_u8GetKey();
                        do something....
                    }
                }
--------------------------------------------------------------------------------------------------*/
#ifndef __KEYBOARD_H__
#define __KEYBOARD_H__


    /*-- Includes --*/
    #include "types.h"
     

    /*-- Defines --*/
    /**--------------------------------------------------------------------------------------------- 
      \def      KEYBOARD_DEBOUNCE
      \brief    Numero de ejecuiones de la funcion Keyboard_Task necesarias para determinar la activacion
                de una entrada, el valor minimo es 3
                Esta definicion es interna unicamente, actuando solo como valor por default cuando
                no se define KEYBOARD_DEBOUNCE en middleware_profile.h
    ----------------------------------------------------------------------------------------------*/ 
    //#define KEYBOARD_DEBOUNCE                	10
	
    /**--------------------------------------------------------------------------------------------- 
      \def      KEYBOARD_PULLUPS
      \brief    Activacion de pullups internas (0 o 1)
                Esta definicion es interna unicamente, actuando solo como valor por default cuando
                no se define KEYBOARD en middleware_profile.h
    ----------------------------------------------------------------------------------------------*/ 
    //#define KEYBOARD_PULLUPS                    1
    
    /**--------------------------------------------------------------------------------------------- 
      \def      KEYBOARD_ROWS
      \brief    Numero de filas en uso
                Esta definicion es interna unicamente, actuando solo como valor por default cuando
                no se define KEYBOARD_ROWS en middleware_profile.h
    ----------------------------------------------------------------------------------------------*/ 
    //#define KEYBOARD_ROWS                    	4
	
    /**--------------------------------------------------------------------------------------------- 
      \def      KEYBOARD_COLUMNS
      \brief    Numero de columnas en uso
                Esta definicion es interna unicamente, actuando solo como valor por default cuando
                no se define KEYBOARD_COLUMNS en middleware_profile.h
    ----------------------------------------------------------------------------------------------*/ 
    //#define KEYBOARD_COLUMNS                 	3
    
    /**---------------------------------------------------------------------------------------------
      \def      KEYBOARD_Rn
      \brief    Asigna los pines ncesarios a cada fila que deba sensarse
                incrementa el valor de x con cadad nueva entrada
                Esta definiciones se trasladan al archivo middleware_profile.h
    ----------------------------------------------------------------------------------------------*/ 
    //#define KEYBOARD_R0_P						GPIO_PORTB
    //#define KEYBOARD_R0_B						3
    /* Repeat the same stament as many rows you need (7 is the limit) */
    
    /**--------------------------------------------------------------------------------------------- 
      \def      KEYBOARD_Cn
      \brief    Asigna los pines ncesarios a cada columna que deba sensarse
                incrementa el valor de x con cadad nueva entrada
                Esta definiciones se trasladan al archivo middleware_profile.h
    ----------------------------------------------------------------------------------------------*/ 
    //#define KEYBOARD_C0_P						GPIO_PORTB
    //#define KEYBOARD_C0_B						0
    /* Repeat the same stament as many columns you need (7 is the limit) */

    
    /*-- Macros --*/
    /**--------------------------------------------------------------------------------------------- 
      \def      Macro
      \brief    Write a nice description for your macros
    ----------------------------------------------------------------------------------------------*/ 
    
    
    /*-- Functions --*/
    /**---------------------------------------------------------------------------------------------
      \brief      Inicializa los pines conectados el teclado como entradas y salidas
      \param	  None
      \return     None
      \warning	  None
    ----------------------------------------------------------------------------------------------*/
    void Keyboard_Init(void);

    /**---------------------------------------------------------------------------------------------
      \brief      Realiza un barrido para verificar si existe una tecla presionada
      \param	  None
      \return     None
      \warning	  Esta función se debe mandar llamar de manera periódica
    ----------------------------------------------------------------------------------------------*/
    void Keyboard_Task(void);

    /**---------------------------------------------------------------------------------------------
      \brief      Establece un nuevo tiempo de debounce para una tecla en especifico
      \param	  u8Key.- tecla a la cual modificar su tiempo de debounce
      \param	  u8Debounce.- Nuevo valor de debounce
      \return     None
      \warning	  None
    ----------------------------------------------------------------------------------------------*/
    void Keyboard_SetDebounce(const _U08 u8Key, const _U08 u8Debounce);

    /**---------------------------------------------------------------------------------------------
      \brief      Función que permite conocer si se presiono una tecla
      \param	  None
      \return     1: si se presiono una tecla (cualquiera)
      \warning	  La función limpia internamente una bandera cunado regresa una valor de uno
    ----------------------------------------------------------------------------------------------*/
    _BOOL Keyboard_bIsPressed(void);

    /**---------------------------------------------------------------------------------------------
      \brief      Función que regresa la ultima tecla presionada
      \param	  None
      \return     Ultima tecla presionada de 0 a KEYBOARD_N_KEYS - 1
      \warning	  Esta función se debe llamar si y solo si la función Keyboard_bIsPressed() == 1u
    ----------------------------------------------------------------------------------------------*/
    _U08 Keyboard_u8GetKey(void);

    /**---------------------------------------------------------------------------------------------
      \brief      Función que regresa el estado de la tecla seleccionada
      \param	  u8Key.- Tecla a interrogar
      \return     Estado de la tecla 1: presionada
      \warning	  Esta función no modifica el estado de la bandera de activación
    ----------------------------------------------------------------------------------------------*/
    _BOOL Keyboard_u8GetStatus(const _U08 u8Key);


#endif   
 