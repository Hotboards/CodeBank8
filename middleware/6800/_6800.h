/*--------------------------------------------------------------------------------------------------
  "THE BEER-WARE LICENSE" (Revision 42):
  <phk@FreeBSD.ORG> wrote this file. As long as you retain this notice you
  can do whatever you want with this stuff. If we meet some day, and you think
  this stuff is worth it, you can buy me a beer in return to Hotboards crew
  (Beer-ware license created by Poul-Henning Kamp)

  \file         _6800.h
  \author       Diego
  \email        diego.perez@hotboards.org
  \ver          1.1
  \date         June 22, 2014
  \target       8-bits Generic

  \brief        Esta pieza de código emula la interfaz paralela 6800 mediante simples pines del uC,
                las tres señales de control (E, RS y RW) utilizadas en esta interfaz se generan
                mediante el driver gpios. La amplitud del bus solo puede ser de 4 u 8 bits y para
                ello se utiliza un puerto completo del uC, en caso de 4 bits solo se usan los bits
                mas significativos.
 -------------------------------------------------------------------------------------------------*/
#ifndef __6800_H__
#define __6800_H__


    /*-- Includes --*/
    #include "types.h"
    #include "gpios/gpios.h"
     

    /*-- Defines --*/
    /**--------------------------------------------------------------------------------------------- 
      \def      _6800_BUSLENGHT
      \brief    tamaño del bus de datos (4 u 8)
                Esta definicion es interna unicamente, actuando solo como valor por default cuando
                cuando no se define _6800_BUSLENGHT en hardware_profile.h
    ----------------------------------------------------------------------------------------------*/ 
    //#define _6800_BUSLENGHT               4

    /**--------------------------------------------------------------------------------------------- 
      \def      _6800_DATAPORT
      \brief    Selecciona el puerto del uC que sera utilizado como bus de datos, en caso de la opcion
                de 4 bits, solo se utilizara la parte mas significativa.
                Es obligatorio escribir esta definicion en el archivo hardware_profile.h
    ----------------------------------------------------------------------------------------------*/ 
    //#define _6800_DATAPORT              GPIOS_PORTD

    /**--------------------------------------------------------------------------------------------- 
      \def      6800_E_n 
      \brief    pin que manipulara la señal de estrobo (enable)
                Es obligatorio escribir esta definicion en el archivo hardware_profile.h
    ----------------------------------------------------------------------------------------------*/ 
    //#define _6800_E_P               	GPIOS_PORTC
    //#define _6800_E_B               	6

    /**--------------------------------------------------------------------------------------------- 
      \def      6800_RW_n 
      \brief    pin utilizado para indicar modo lectura o escritura
                Es obligatorio escribir esta definicion en el archivo hardware_profile.h
    ----------------------------------------------------------------------------------------------*/ 
    //#define _6800_RW_P                  GPIOS_PORTC
    //#define _6800_RW_B                  7

    /**--------------------------------------------------------------------------------------------- 
      \def      6800_RS_n 
      \brief    pin itlizado para indicar si se escribe un dato o un comando
                Es obligatorio escribir esta definicion en el archivo hardware_profile.h
    ----------------------------------------------------------------------------------------------*/ 
    //#define _6800_RS_P                  GPIOS_PORTB
    //#define _6800_RS_B                  7

    
    /*-- Macros --*/
    /**--------------------------------------------------------------------------------------------- 
      \def      Define
      \brief    Write a nice description for your macros
    ----------------------------------------------------------------------------------------------*/ 
    
    
    /*-- Functions --*/
    /**---------------------------------------------------------------------------------------------
      \brief      Inicializa los pines de control que actuaran como puerto paralelo
      \param	  None
      \return     None
      \warning	  Se deben declarar los pines a usar en el archivo middleware_profile.h
    ----------------------------------------------------------------------------------------------*/
    void _6800_Init(void);

    /**---------------------------------------------------------------------------------------------
      \brief      Escribe un simple dato a través del bus (este valor puede ser de 8 o 4 bits)W/R=0,R/S=1
      \param	  u8Data.- dato a ser enviado a través del bus de datos
      \return     None
      \warning	  En el caso de la opción de 4 bits la función solo toma en cuenta los 4 bits mas altos (4-7)
    ----------------------------------------------------------------------------------------------*/
    void _6800_WriteData(_U08 u8Data);

    /**---------------------------------------------------------------------------------------------
      \brief      lee un dato del dispositivo externo conectado al bus de datos (W/R=1, R/S=1)
      \param	  None
      \return     Dato leído
      \warning	  En el caso de el modo de 4 bits solo se toma en cuenta los 4 bits mas altos (4-7)
    ----------------------------------------------------------------------------------------------*/
    _U08 _6800_u8ReadData(void);

    /**---------------------------------------------------------------------------------------------
      \brief      Esribe un comando al dispositivo externo (W/R=0 and R/S =0)
      \param	  u8Cmd.- Comando a enviar
      \return     None
      \warning	  En el caso de el modo de 4 bits solo se toma en cuenta los 4 bits mas altos (4-7)
    ----------------------------------------------------------------------------------------------*/
    void _6800_WriteCommand(_U08 u08Cmd);

    /**---------------------------------------------------------------------------------------------
      \brief      Lee una dirección del dispositivo externo (W/R=1 and R/S =0)
      \param	  None
      \return     Dirección enviada
      \warning	  En el caso de el modo de 4 bits solo se toma en cuenta los 4 bits mas altos (4-7).
                  En este tipo de dispositivos no se leen comandos pero si direcciones internas.
    ----------------------------------------------------------------------------------------------*/
    _U08 _6800_u8ReadAddr(void);

    
#endif   
 