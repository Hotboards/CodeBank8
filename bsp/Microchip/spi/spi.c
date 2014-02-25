/*--------------------------------------------------------------------------------------------------
  "THE BEER-WARE LICENSE" (Revision 42):
  <phk@FreeBSD.ORG> wrote this file. As long as you retain this notice you
  can do whatever you want with this stuff. If we meet some day, and you think
  this stuff is worth it, you can buy me a beer in return to Hotboards crew
  (Beer-ware license created by Poul-Henning Kamp)

  \file         spi.c
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
#include "spi.h"
#include <p18cxxx.h>
#include <string.h>


/*-- Private Definitions--*/


/*-- Global variables --*/
static volatile near _U08 *gau8SSPCON1[3] = {NULL, &SSP1CON1, &SSP2CON1};
static volatile near _U08 *gau8SSPSTAT[3] = {NULL, &SSP1STAT, &SSP2STAT};
static volatile near _U08 *gau8SSPBUF[3]  = {NULL, &SSP1BUF, &SSP2BUF};
static const rom _U08 gau8ModeCKP[4] = {0x00,0x00,0x10,0x10};
static const rom _U08 gau8ModeCKE[4] = {0x40,0x00,0x40,0x00};


/*-- Private Macros --*/


/*-- Private functions prototypes --*/


/*-- External functions --*/
/**-----------------------------------------------------------------------------------------------*/
void Spi_Init(_U08 u8Spi, _U08 u8Divisor)
{
    if((u8Divisor==SPI_4DIV) || (u8Divisor==SPI_16DIV) || (u8Divisor==SPI_64DIV))
    {
        *gau8SSPCON1[u8Spi] = u8Divisor;
        Spi_SetMode(u8Spi, 0);
        SET_8BIT(*gau8SSPCON1[u8Spi], 5);
    }
}
/**-----------------------------------------------------------------------------------------------*/

/**-----------------------------------------------------------------------------------------------*/
void Spi_SetMode(_U08 u8Spi, _U08 u8Mode)
{
    u8Mode &= (_U08)3;
    *gau8SSPCON1[u8Spi] |= gau8ModeCKP[u8Mode];
    *gau8SSPSTAT[u8Spi] |= gau8ModeCKE[u8Mode];
}
/**-----------------------------------------------------------------------------------------------*/

/**-----------------------------------------------------------------------------------------------*/
_U08 Spi_u8Transfer(_U08 u8Spi, _U08 u8TxByte)
{
    *gau8SSPBUF[u8Spi] = u8TxByte;
    while(QUERY_8BIT(*gau8SSPSTAT[u8Spi], 0) == 0)
    {/* wait to receive single byte */
        /*MISRA*/
    }
    return (_U08)*gau8SSPBUF[u8Spi];
}
/**-----------------------------------------------------------------------------------------------*/


/*-- Private functions --*/
/**---------------------------------------------------------------------------------------------    
  \brief    None
  \param    None
  \return   None
  \warning  None   	
----------------------------------------------------------------------------------------------*/
