/*--------------------------------------------------------------------------------------------------
  "THE BEER-WARE LICENSE" (Revision 42):
  <phk@FreeBSD.ORG> wrote this file. As long as you retain this notice you
  can do whatever you want with this stuff. If we meet some day, and you think
  this stuff is worth it, you can buy me a beer in return to Hotboards crew
  (Beer-ware license created by Poul-Henning Kamp)

  \file         spi2.h
  \author       Diego
  \email        diego.perez@hotboards.org
  \ver          1.0
  \date         August 20, 2013
  \target       PIC18FJ

  \brief        Driver para controlar la comunicación serial sincrona de tipo spi usando el periférico
                MSSP del uC. El periférico MSSP de los PIC18FJ puede tener dos funciones, como spi o
                i2c, en este caso este driver lo configura para ser usado únicamente como spi en modo
                maestro. Aparte de solo implementarse como maestro, no se implementan interrupciones
                pues solo se usa una simple función bloqueante para tx y rx un byte por vez.
--------------------------------------------------------------------------------------------------*/
#ifndef __SPI2_H__
#define __SPI2_H__


    /*-- Includes --*/
    #include "types.h"
     

    /*-- Defines --*/
    /**---------------------------------------------------------------------------------------------
      \def      SPI_DIV
      \brief    Valores que dividen la frecuencia de operación del uC (valores 4, 16, 64)
    ----------------------------------------------------------------------------------------------*/
    #define SPI2_4DIV                        0
    #define SPI2_16DIV                       1
    #define SPI2_64DIV                       2

    /*----------------------------------------------------------------------------------------------
    |                            SPI_BUSCLK                                                        |
    |   Baud rate generation  = ------------                                                       |
    |                           (SPI_DIV)                                                          |
    ----------------------------------------------------------------------------------------------*/

    
    /*-- Macros --*/
    
    
    /*-- Functions --*/
    /**---------------------------------------------------------------------------------------------
      \brief      Inicializa el periférico MSSP como SPI en modo maestro
      \param	  u8Divisor.- divide la frecuencia de operación del cpu, SPI2_4DIV, SPI2_16DIV, SPI2_64DIV
      \return     None
      \warning	  El periférico se inicia con modo de transmicion 0
    ----------------------------------------------------------------------------------------------*/
    void Spi2_Init(_U08 u8Divisor);

    /**---------------------------------------------------------------------------------------------
      \brief      Establece el modo de transmicion SPI a usar
      \param	  u8Mode.- mode  clk pha         descripción
                            0      0,  0   (clk idle=0, data sample=rising)
                            1      0,  1   (clk idle=0, data sample=fall)
                            2      1,  0   (clk idle=1, data sample=fall)
                            3      1,  1   (clk idle=1, data sample=rise)
      \return     None
      \warning	  None
    ----------------------------------------------------------------------------------------------*/
    void Spi2_SetMode(_U08 u8Mode);

    /**---------------------------------------------------------------------------------------------
      \brief      Transmite un simple byte, ademas de regresar un byte de llegada (si lo hay)
      \param	  u8TxByte.- byte a transmitir
      \return     byte recibido
      \warning	  Esta función traba el cpu hasta que se termine de transmitir y/o recibir un byte
    ----------------------------------------------------------------------------------------------*/
    _U08 Spi2_u8Transfer(_U08 u8TxByte);


#endif   
 