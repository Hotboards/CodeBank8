/*--------------------------------------------------------------------------------------------------
  "THE BEER-WARE LICENSE" (Revision 42):
  <phk@FreeBSD.ORG> wrote this file. As long as you retain this notice you
  can do whatever you want with this stuff. If we meet some day, and you think
  this stuff is worth it, you can buy me a beer in return to Hotboards crew
  (Beer-ware license created by Poul-Henning Kamp)

  \file         spi2.c
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
/*-- Includes --*/
#include "spi2.h"
#include <p18cxxx.h>


/*-- Private Definitions--*/


/*-- Global variables --*/
static const rom _U08 gau8ModeCKP[4] = {0x00,0x00,0x10,0x10};
static const rom _U08 gau8ModeCKE[4] = {0x40,0x00,0x40,0x00};


/*-- Private Macros --*/
#define SET_BAUDRATE(rate)      (SSP2CON1 |= (rate))
#define SET_MODE(mode)          (SSP2CON1 |= gau8ModeCKP[(mode)]);(SSP2STAT |= gau8ModeCKE[(mode)])
#define MODULE_ON()             (SET_8BIT(SSP2CON1, 5))
#define GET_RX_FLAG()           (QUERY_8BIT(SSP2STAT, 0))
#define TX_BUFFER(data)         (SSP2BUF = (data))
#define RX_BUFFER()             (SSP2BUF)


/*-- Private functions prototypes --*/


/*-- External functions --*/
/**-----------------------------------------------------------------------------------------------*/
void Spi2_Init(_U08 u8Divisor)
{
    if((u8Divisor==SPI2_4DIV) || (u8Divisor==SPI2_16DIV) || (u8Divisor==SPI2_64DIV))
    {
        SET_BAUDRATE(u8Divisor);
        Spi2_SetMode(0);
        MODULE_ON();
    }
}
/**-----------------------------------------------------------------------------------------------*/

/**-----------------------------------------------------------------------------------------------*/
void Spi2_SetMode(_U08 u8Mode)
{
    u8Mode &= (_U08)3;
    SET_MODE(u8Mode);
}
/**-----------------------------------------------------------------------------------------------*/

/**-----------------------------------------------------------------------------------------------*/
_U08 Spi2_u8Transfer(_U08 u8TxByte)
{
    TX_BUFFER(u8TxByte);
    while(GET_RX_FLAG() == 0u)
    {/* wait to receive single byte */
        /*MISRA*/
    }
    return (_U08)RX_BUFFER();
}
/**-----------------------------------------------------------------------------------------------*/


/*-- Private functions --*/
/**---------------------------------------------------------------------------------------------    
  \brief    None
  \param    None
  \return   None
  \warning  None   	
----------------------------------------------------------------------------------------------*/
