/*--------------------------------------------------------------------------------------------------
  "THE BEER-WARE LICENSE" (Revision 42):
  <phk@FreeBSD.ORG> wrote this file. As long as you retain this notice you
  can do whatever you want with this stuff. If we meet some day, and you think
  this stuff is worth it, you can buy me a beer in return to Hotboards crew
  (Beer-ware license created by Poul-Henning Kamp)

  \file         i2c.c
  \author       Diego
  \email        diego.perez@hotboards.org
  \ver          1.0
  \date         Marzo 19, 2014
  \target       PIC18FJ

  \brief        Driver para controlar la generación de señales necesarias para implementar el
                protocolo de comunicaciones sincrona I2C. El driver hace uso del periférico MSSP
                por lo cual una vez seteado no se puede usar como spi, además sólo opera en modo
                maestro.
--------------------------------------------------------------------------------------------------*/
/*-- Includes --*/
#include "i2c.h"
#include <p18cxxx.h>
#include <string.h>
#include "hardware_profile.h"


/*-- Private Definitions--*/
#ifndef BSP_CLOCK
    #define I2C_CLOCK                 12000000
#else
    #define I2C_CLOCK                 BSP_CLOCK
#endif

#define SEN_BIT                 0
#define RSEN_BIT                1
#define PEN_BIT                 2
#define RCEN_BIT                3
#define ACKEN_BIT               4
#define ACKDT_BIT               5


/*-- Global variables --*/
static volatile near _U08 *gau8SSPCON2[3] = {NULL, &SSP1CON2, &SSP2CON2};
static volatile near _U08 *gau8SSPBUF[3] = {NULL, &SSP1BUF, &SSP2BUF};
static volatile near _U08 *gau8SSPIF[3]  = {NULL, &PIR1, &PIR3};
static volatile near _U08 *gau8SSPCON1[3]  = {NULL, &SSP1CON1, &SSP2CON1};
static volatile near _U08 *gau8SSPADD[3]  = {NULL, &SSP1ADD, &SSP2ADD};
static const _U08 gau8SSPIFBIT[3]  = {0x00, 3, 7};


/*-- Private Macros --*/


/*-- Private functions prototypes --*/


/*-- External functions --*/
/**-----------------------------------------------------------------------------------------------*/
void I2c_Init(_U08 u8I2c, _U32 u32Frec)
{
    _U08 u8Brg;
    u8Brg = (_U08)(((_U32)I2C_CLOCK / (_U32)4) / u32Frec);
    *gau8SSPCON1[u8I2c] = 0b00101000; /*Module enable, Master mode*/
    *gau8SSPADD[u8I2c] = u8Brg - 1;
}
/**-----------------------------------------------------------------------------------------------*/

/**-----------------------------------------------------------------------------------------------*/
void I2c_Start(_U08 u8I2c)
{
    SET_8BIT(*gau8SSPCON2[u8I2c], SEN_BIT);                         /*se genera condicion de start*/
    while(QUERY_8BIT(*gau8SSPIF[u8I2c], gau8SSPIFBIT[u8I2c]) == 0); /*esperamos a que termine la condicion*/
    CLEAR_8BIT(*gau8SSPIF[u8I2c], gau8SSPIFBIT[u8I2c]);             /*limpiamos bandera*/
}
/**-----------------------------------------------------------------------------------------------*/

/**-----------------------------------------------------------------------------------------------*/
_BOOL I2c_bTxByte(_U08 u8I2c, _U08 u8Byte)
{
    *gau8SSPBUF[u8I2c] = u8Byte;                                    /*cargamos byte a transmitir*/
    while(QUERY_8BIT(*gau8SSPIF[u8I2c], gau8SSPIFBIT[u8I2c]) == 0); /*esperamos a que envie el byte*/
    CLEAR_8BIT(*gau8SSPIF[u8I2c], gau8SSPIFBIT[u8I2c]);             /*limpiamos bandera*/
    
    return (*gau8SSPCON2[u8I2c] >> 6) & 0x01;                       /*regresamos condicion de ACK*/
}
/**-----------------------------------------------------------------------------------------------*/

/**-----------------------------------------------------------------------------------------------*/
_U08 I2c_u8RxByte(_U08 u8I2c, _BOOL bAck)
{
    _U08 u8Byte;
    bAck &= 0x1;

    SET_8BIT(*gau8SSPCON2[u8I2c], RCEN_BIT);                        /*preparamos la recepcion*/
    while(QUERY_8BIT(*gau8SSPIF[u8I2c], gau8SSPIFBIT[u8I2c]) == 0); /*espermaos a recivir un byte*/
    CLEAR_8BIT(*gau8SSPIF[u8I2c], gau8SSPIFBIT[u8I2c]);             /*limpiamos bandera*/
    u8Byte = *gau8SSPBUF[u8I2c];                                    /*respaldamos el byte recivido*/
    *gau8SSPCON2[u8I2c] |= (bAck << ACKDT_BIT);                     /*seteamos pulso de ACK*/
    SET_8BIT(*gau8SSPCON2[u8I2c], ACKEN_BIT);                       /*mandamos pulso de ACK*/
    CLEAR_8BIT(*gau8SSPIF[u8I2c], gau8SSPIFBIT[u8I2c]);             /*limpiamos bandera*/
    
    return u8Byte;
}
/**-----------------------------------------------------------------------------------------------*/

/**-----------------------------------------------------------------------------------------------*/
void I2c_RepeatedStart(_U08 u8I2c)
{
    SET_8BIT(*gau8SSPCON2[u8I2c], RSEN_BIT);                        /*se genera la condicion de repeated start*/
    while(QUERY_8BIT(*gau8SSPIF[u8I2c], gau8SSPIFBIT[u8I2c]) == 0); /*esperamos a que envie el byte*/
    CLEAR_8BIT(*gau8SSPIF[u8I2c], gau8SSPIFBIT[u8I2c]);             /*limpiamos bandera*/
}
/**-----------------------------------------------------------------------------------------------*/

/**-----------------------------------------------------------------------------------------------*/
void I2c_Stop(_U08 u8I2c)
{
    SET_8BIT(*gau8SSPCON2[u8I2c], PEN_BIT);                         /*se genera la condicion de stop*/
    while(QUERY_8BIT(*gau8SSPIF[u8I2c], gau8SSPIFBIT[u8I2c]) == 0); /*esperamos a que envie el byte*/
    CLEAR_8BIT(*gau8SSPIF[u8I2c], gau8SSPIFBIT[u8I2c]);             /*limpiamos bandera*/
}
/**-----------------------------------------------------------------------------------------------*/


/*-- Private functions --*/
/**---------------------------------------------------------------------------------------------    
  \brief    None
  \param    None
  \return   None
  \warning  None   	
----------------------------------------------------------------------------------------------*/
