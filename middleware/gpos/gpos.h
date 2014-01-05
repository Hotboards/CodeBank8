/*--------------------------------------------------------------------------------------------------
  "THE BEER-WARE LICENSE" (Revision 42):
  <phk@FreeBSD.ORG> wrote this file. As long as you retain this notice you
  can do whatever you want with this stuff. If we meet some day, and you think
  this stuff is worth it, you can buy me a beer in return to Hotboards crew 
  (Beer-ware license created by Poul-Henning Kamp)

  \file         gpos.h
  \author       Diego
  \email        diego.perez@hotboards.org
  \ver          1.0
  \date         July 25, 2013
  \target       8-bit uC

  \brief        El driver gpos permite tener en los pines digitales del micro configurados de manera
                que oscilen continuamente, se mantengan encendidas o apagadas por cierto tiempo o
                totalmente apagadas o encendidas sin cambiar nunca, este driver no utiliza ningún
                hardware del uC y solo es eficiente con periodos de tiempo mayores a 1ms..

                Example:
                #define BUZZER_SHORT()			Gpos_SetMode(0, GPOS_DELAYLOW_MODE, 5)
                #define BUZZER_LONG()			Gpos_SetMode(1, GPOS_DELAYLOW_MODE, 50)

                Gpos_Init()
                ............
                for(;;)
                {
                    Gpos_Task();
                }
                ..............................
                somewhere in code...
                BUZZER_SHORT();
 -------------------------------------------------------------------------------------------------*/
#ifndef __GPOS_H__
#define __GPOS_H__


    /*-- Includes --*/
    #include "types.h"
     

    /*-- Defines --*/
    /**---------------------------------------------------------------------------------------------
      \def      GPOS_N_PINS
      \brief    Numero de salidas a utilizar por la aplicacion
                Esta definicion es interna unicamente, actuando solo como valor por default cuando
                no se define GPOS_N_PINS en hardware_profile.h
    ----------------------------------------------------------------------------------------------*/
    //#define GPOS_N_PINS                       0

    /**---------------------------------------------------------------------------------------------
      \def      GPOS_Px
      \brief    Asigna los pines ncesarios a cada salida que deba actualizarse
                incrementa el valor de x con cadad nueva salida
                Estas definiciones se trasladan al archivo hardware_profile.h
    ----------------------------------------------------------------------------------------------*/
    //#define GPOS_P0_P                         GPIOS_PORTA     /* PORT */
    //#define GPOS_P0_B                  	1               /* BYTE */

    /**---------------------------------------------------------------------------------------------
      \def      _eGPOS_MODES
      \brief    Estos son los diferentes modos en los que pueden operar las salidas
    ----------------------------------------------------------------------------------------------*/
    typedef enum
    {
        GPOS_OSC_MODE = 0u,             /*|-|_|-|_|-|_|-|_|  siempre oscilando*/
        GPOS_HIGH_MODE,                 /*-----------------  siempre en alto*/
        GPOS_LOW_MODE,                  /*_________________  siempre en bajo*/
        GPOS_DELAYHIGH_MODE,            /*--------|________  un momento en alto*/
        GPOS_DELAYLOW_MODE              /*________|--------  un momento en bajo*/
    }_eGPOS_MODES;

    
    /*-- Macros --*/
    /**--------------------------------------------------------------------------------------------- 
      \def        Macro
      \brief      Write a nice description for your macros
    ----------------------------------------------------------------------------------------------*/ 
    
    
    /*-- Functions --*/
    /**---------------------------------------------------------------------------------------------
      \brief      Inicializa las salidas seleccionadas
      \param	  None
      \return     None
      \warning	  None
    ----------------------------------------------------------------------------------------------*/
    void Gpos_Init(void);

    /**---------------------------------------------------------------------------------------------
      \brief      Actualiza el estado de las salidas con cada ejecución
      \param	  None
      \return     None
      \warning	  Esta función debe ser ejecutada de manera periódica, se recomienda cada 10ms
    ----------------------------------------------------------------------------------------------*/
    void Gpos_Task(void);

    /**---------------------------------------------------------------------------------------------
      \brief      Establece el modo de operación la salida seleccionada
      \param	  u8Gpo.- Pin seleccionado
      \param	  u8Status.- Modo de operacion de la salida
      \param	  u8Counter.- Numero de ejecuciones de Gpos_Task en los que permanecera uns estado el pin
      \return     None
      \warning	  None
    ----------------------------------------------------------------------------------------------*/
    void Gpos_SetMode(_U08 u8Gpo, _eGPOS_MODES eModes, _U08 u8Counter);


#endif   
 