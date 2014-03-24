/*--------------------------------------------------------------------------------------------------
  "THE BEER-WARE LICENSE" (Revision 42):
  <phk@FreeBSD.ORG> wrote this file. As long as you retain this notice you
  can do whatever you want with this stuff. If we meet some day, and you think
  this stuff is worth it, you can buy me a beer in return to Hotboards crew
  (Beer-ware license created by Poul-Henning Kamp)

  \file         tc74ax.h
  \author       Diego
  \email        diego.perez@hotboards.org
  \ver          1.0
  \date         March 24, 2014
  \target       8-bits Generic

  \brief        Pieza de código para controlar el sensor digital de temperaura ambiental Microchip
                TC74. El sensor se controla a travez de una interfaz I2C, por lo que es necesario
                utilizar el driver de i2c del banco de codigo. Es posible leer toda la serie de
                sensores pertenecientes a esta familia los cuales poseen diferentes direcciones.
                El codigo permite poner al sensor en modo de bajo consumo y despertarlo.
 -------------------------------------------------------------------------------------------------*/
#ifndef __TC74AX_H__
#define __TC74AX_H__


    /*-- Includes --*/
    #include "types.h"
     

    /*-- Defines --*/
    /**---------------------------------------------------------------------------------------------
      \def        _eTC74AX_ADDR
      \brief      Direcciones de los sensores de temperatura de la serie TC74 de Microchip
    ----------------------------------------------------------------------------------------------*/
    typedef enum
    {
        TC74A0 = 0b1001000,
        TC74A1 = 0b1001001,
        TC74A2 = 0b1001010,
        TC74A3 = 0b1001011,
        TC74A4 = 0b1001100,
        TC74A5 = 0b1001101,
        TC74A6 = 0b1001110,
        TC74A7 = 0b1001111
    }_eTC74AX_ADDR;

    /**---------------------------------------------------------------------------------------------
      \def        _eTC74AX_STANDBY
      \brief      Estados en los que puede estar el sensor de tmeperatura
    ----------------------------------------------------------------------------------------------*/
    typedef enum
    {
        TC74AX_WAKEUP = 0,
        TC74AX_STANDBY       
    }_eTC74AX_STANDBY;


    /*-- Macros --*/
    /**--------------------------------------------------------------------------------------------- 
      \def      Define
      \brief    Write a nice descripción for your macros
    ----------------------------------------------------------------------------------------------*/ 
    
    
    /*-- Functions --*/
    /**---------------------------------------------------------------------------------------------
      \brief      Lee la bandera de status del sensor
      \param	  u8Sensor.- Sensor a leer (solo usar enums _eTC74AX_ADDR)
      \return     1 si el sensor esta listo, 0 si no lo esta
      \warning	  La funcion invade el bus i2c
    ----------------------------------------------------------------------------------------------*/
    _BOOL Tc74ax_bReadyFlag(const _U08 u8Sensor);

    /**---------------------------------------------------------------------------------------------    
      \brief      Lee la temperatura ya digital
      \param	  u8Sensor.- Sensor a leer (solo usar enums _eTC74AX_ADDR)
      \return     Temperatura medida, cantidad entera con signo
      \warning	  se recomienda preguntar el estado del sensor nates de usar la funcion, en especial
                  si el sensor se acaba de encender
    ----------------------------------------------------------------------------------------------*/
    _S08 Tc74ax_ReadTemp(const _U08 u8Sensor);

    /**---------------------------------------------------------------------------------------------    
      \brief      Coloca al sensor en modo de bajo consumo o lo despierta
      \param	  u8Sensor.- Sensor a apagar/encender (solo usar enums _eTC74AX_ADDR)
      \param	  bStatus.- Estado del sensor (usar valores de enum _eTC74AX_STANDBY)
      \return     None
      \warning	  None
    ----------------------------------------------------------------------------------------------*/
    void Tc74ax_Standby(const _U08 u8Sensor, _BOOL bStatus);


#endif   
 