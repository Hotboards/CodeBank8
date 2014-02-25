/*--------------------------------------------------------------------------------------------------
  "THE BEER-WARE LICENSE" (Revision 42):
  <phk@FreeBSD.ORG> wrote this file. As long as you retain this notice you
  can do whatever you want with this stuff. If we meet some day, and you think
  this stuff is worth it, you can buy me a beer in return to Hotboards crew 
  (Beer-ware license created by Poul-Henning Kamp)

  \file         gpis.h
  \author       Diego
  \email        diego.perez@hotboards.org
  \ver          1.0
  \date         July 25, 2013
  \target       8-bit uC (Generic)

  \brief        Esta pieza de código maneja la complejidad detrás de la activación de interruptores
                mecánicos, toma en cuenta la eliminación de rebotes, detección de flancos de subida,
                tiempo de presionado y el estado actual de cada botón.

                Example:
                Gpis_Init()
		............
		for(;;)
		{
                    Gpis_Task();
		}
                ..............................
                somewhere in code...
                if(Gpis_bGetInput(2)==1u)
                {
                   do something..........
                }
 -------------------------------------------------------------------------------------------------*/
#ifndef __GPIS_H__
#define __GPIS_H__


    /*-- Includes --*/
    #include "types.h"
     

    /*----------------------------------------------------------------------------------------------
      \def      GPIS_N_INPUTS
      \brief    Numero de entradas a sensar, 16 es el valor maximo
                Esta definicion es interna unicamente, actuando solo como valor por default cuando
                no se define GPIS_N_INPUTS en hardware_profile.h
    ----------------------------------------------------------------------------------------------*/
    //#define GPIS_N_INPUTS                   0

    /*----------------------------------------------------------------------------------------------
      \def      GPIS_ON
      \brief    Nivel logico en el cual se activaran las entradas (0 o 1)
                Esta definicion es interna unicamente, actuando solo como valor por default cuando
                no se define GPIS_ON en hardware_profile.h
    ----------------------------------------------------------------------------------------------*/
    //#define GPIS_ON                         0

    /*----------------------------------------------------------------------------------------------
      \def      GPIS_DEBOUNCE
      \brief    Numero de ejecuiones de la funcion Gpis_Task necesarias para determinar la activacion
                de una entrada, el valor minimo es 3
                Esta definicion es interna unicamente, actuando solo como valor por default cuando
                no se define GPIS_DEBOUNCE en hardware_profile.h
    ----------------------------------------------------------------------------------------------*/
    //#define GPIS_DEBOUNCE                   5

    /*----------------------------------------------------------------------------------------------
      \def      GPIS_PULLUPS
      \brief    Activacion de pullups internas (0 o 1)
                Esta definicion es interna unicamente, actuando solo como valor por default cuando
                no se define GPIS_PULLUPS en hardware_profile.h
    ----------------------------------------------------------------------------------------------*/
    //#define GPIS_PULLUPS                    0

    /*----------------------------------------------------------------------------------------------
      \def      GPIS_Bx
      \brief    Asigna los pines ncesarios a cada entrada que deba sensarse
                incrementa el valor de x con cadad nueva entrada
                Esta definiciones se trasladan al archivo hardware_profile.h
    ----------------------------------------------------------------------------------------------*/
    //#define GPIS_B0_P                       GPIOS_PORTA
    //#define GPIS_B0_B                       0


    /*-- Macros --*/
    /**---------------------------------------------------------------------------------------------
      \def      Macro
      \brief    Write a nice description for your macros
    ----------------------------------------------------------------------------------------------*/


    /*-- Functions --*/
    /**---------------------------------------------------------------------------------------------
      \brief      Inicializa los pines de entradas en los que estarán conectados los interruptores
      \param	  None
      \return     None
      \warning	  None
    ----------------------------------------------------------------------------------------------*/
    void Gpis_Init(void);

    /**---------------------------------------------------------------------------------------------
      \brief      Lee el estado de los pines de entrada, los procesa para eliminar rebotes y detecta
				  flancos de subida (momentos de activación)
      \param	  None
      \return     None
      \warning	  Esta función debe ser llamada de manera periódica, el tiempo recomendad es de 10ms
    ----------------------------------------------------------------------------------------------*/
    void Gpis_Task(void);

    /**---------------------------------------------------------------------------------------------
      \brief      Habilita o deshabilita una entrada mecánica (botón, interruptor, etc.)
      \param	  u8Gpi.- Entrada seleccionada (de 0 hasta GPIS_N_INPUTS-1)
      \param      bStatus.- 1 para activar 0 para desactivar
      \return     None
      \warning	  Por defaul la función Gpis_Init habilita todas las entradas
    ----------------------------------------------------------------------------------------------*/
    void Gpis_Enable(_U08 u8Gpi, _BOOL bStatus);

    /**---------------------------------------------------------------------------------------------
      \brief      Establece un nuevo valor de debounce
      \param	  u8Gpi.- Entrada seleccionada (de 0 hasta GPIS_N_INPUTS-1)
      \param      u8Debounce.- El numero de ejecuciones de Gpis_Task necesarias para dar una activación
      \return     None
      \warning	  El valor mínimo de debounce es de 3
    ----------------------------------------------------------------------------------------------*/
    void Gpis_Debounce(_U08 u8Gpi, _U08 u8Debounce);

    /**---------------------------------------------------------------------------------------------
      \brief      Pregunta si una entrada a tenido una activación
      \param	  u8Gpi.- Entrada seleccionada (de 0 hasta GPIS_N_INPUTS-1)
      \return     '1' si la entrada ha sido activada
      \warning	  La función limpia internamente la bandera de activación cuando esta regresa un '1'
    ----------------------------------------------------------------------------------------------*/
    _BOOL Gpis_bGetInput(_U08 u8Gpi);

    /**---------------------------------------------------------------------------------------------
      \brief      Regresa el estado actual en el que se encuentra la entrada
      \param	  u8Gpi.- Entrada seleccionada (de 0 hasta GPIS_N_INPUTS-1)
      \return     '1' si la entrada esta activa ("presionado") o '0' si esta en estado no activo
      \warning	  None
    ----------------------------------------------------------------------------------------------*/
    _BOOL Gpis_bGetStatus(_U08 u8Gpi);


#endif
 