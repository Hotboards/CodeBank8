/*--------------------------------------------------------------------------------------------------
  "THE BEER-WARE LICENSE" (Revision 42):
  <phk@FreeBSD.ORG> wrote this file. As long as you retain this notice you
  can do whatever you want with this stuff. If we meet some day, and you think
  this stuff is worth it, you can buy me a beer in return to Hotboards crew
  (Beer-ware license created by Poul-Henning Kamp)

  \file         tc74ax.c
  \author       Diego
  \email        diego.perez@hotboards.org
  \ver          1.0
  \date         March 24, 2014
  \target       8-bits Generic

  \brief        Pieza de código para controlar el sensor digital de temperaura ambiental Microchip
                TC74. El sensor se controla a travez de una interfaz I2C, por lo que es necesario
                utilizar el driver de i2c del banco de codigo. Es posible leer toda la serie de
                sensores pertenecientes a esta familia los cuales posee diferentes direcciones.
                El codigo permite poner al sensor en modo de bajo consumo y despertarlo.
 -------------------------------------------------------------------------------------------------*/
/*-- Includes --*/
#include "tc74ax/tc74ax.h"
#include "i2c/i2c.h"
#include "hardware_profile.h"


/*-- Private Definitions--*/
#ifndef TC74AX_PORT
    #define I2C_PORT                 I2C_PORT2
#else
    #define I2C_PORT                 TC74AX_PORT
#endif

#define CONFIG              0x01  /*Read data from memory array beginning at selected address*/
#define TEMP                0x00  /*Write data to memory array beginning at selected address*/


/*-- Private Macros --*/


/*-- Private functions prototypes --*/


/*-- External functions --*/
/**-----------------------------------------------------------------------------------------------*/
_BOOL Tc74ax_bReadyFlag(const _U08 u8Sensor)
{
    _U08 u8Config;

    I2c_Start(I2C_PORT);                                   /*generamos condicion start*/
    (void)I2c_bTxByte(I2C_PORT, ADDR_WRITE(u8Sensor));    /*madamos direccion del sensor en modo escritura*/
    (void)I2c_bTxByte(I2C_PORT, CONFIG);                     /*mandamos direccion a leer*/
    I2c_RepeatedStart(I2C_PORT);                           /*repetimos señal start*/
    (void)I2c_bTxByte(I2C_PORT, ADDR_READ(u8Sensor));     /*madmaos direccion del sensor en modo lectura*/
    u8Config = I2c_u8RxByte(I2C_PORT, I2C_NACK);           /*leemos dato leido y contestamos NACK*/
    I2c_Stop(I2C_PORT);                                    /*indicamos fin de comunicacion*/

    return QUERY_8BIT(u8Config, 6);
}
/**-----------------------------------------------------------------------------------------------*/

/**-----------------------------------------------------------------------------------------------*/
_S08 Tc74ax_ReadTemp(const _U08 u8Sensor)
{
    _S08 i8Temp;

    I2c_Start(I2C_PORT);                                   /*generamos condicion start*/
    (void)I2c_bTxByte(I2C_PORT, ADDR_WRITE(u8Sensor));    /*mandamos direccion del sensor en modo escritura*/
    (void)I2c_bTxByte(I2C_PORT, TEMP);                     /*mandamos direccion de la lectura d ela temperatura*/
    I2c_RepeatedStart(I2C_PORT);                           /*repetimos señal start*/
    (void)I2c_bTxByte(I2C_PORT, ADDR_READ(u8Sensor));     /*madamos direccion del sensor en modo lectura*/
    i8Temp = (_S08)I2c_u8RxByte(I2C_PORT, I2C_NACK);     /*leemos dato (temperatura) y contestamos NACK*/
    I2c_Stop(I2C_PORT);                                    /*indicamos fin de comunicacion*/
    
    return i8Temp;
}
/**-----------------------------------------------------------------------------------------------*/

/**-----------------------------------------------------------------------------------------------*/
void Tc74ax_Standby(const _U08 u8Sensor, _BOOL bStatus)
{
    bStatus &= 0x01;
    I2c_Start(I2C_PORT);                                   /*generamos condicion start*/
    (void)I2c_bTxByte(I2C_PORT, ADDR_WRITE(u8Sensor));    /*madamos direccion del sensor en modo escritura*/
    (void)I2c_bTxByte(I2C_PORT, CONFIG);                     /*mandamos direccion a leer*/
    (void)I2c_bTxByte(I2C_PORT, bStatus);     /*madmaos direccion del sensor en modo lectura*/
    I2c_Stop(I2C_PORT);                                    /*indicamos fin de comunicacion*/
}
/**-----------------------------------------------------------------------------------------------*/


/*-- Private functions --*/
/**-------------------------------------------------------------------------------------------------    
  \brief      None
  \param      None
  \return     None
  \warning    None
--------------------------------------------------------------------------------------------------*/
