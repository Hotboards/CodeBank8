/*--------------------------------------------------------------------------------------------------
  "THE BEER-WARE LICENSE" (Revision 42):
  <phk@FreeBSD.ORG> wrote this file. As long as you retain this notice you
  can do whatever you want with this stuff. If we meet some day, and you think
  this stuff is worth it, you can buy me a beer in return to Hotboards crew
  (Beer-ware license created by Poul-Henning Kamp)

  \file         hd44780.h
  \author       Diego
  \email        diego.perez@hotboards.org
  \ver          1.0
  \date         August 29, 2013
  \target       8-bits Generic

  \brief        Pieza de código para controlar la serie de memorias 25xx de la marca Microchip, las
                cuales poseen interfaz serial spi. El driver es capaz de controlar hasta 4 memorias
                las cuales pueden estar conectadas a un mismo modulo spi o a diferentes modulos.
                Es necesario definir esto en el archivo hardware_profile.h. las memorias de la
                serie 25 pueden venir en combinaciones de.
                Density:   1Kbit   | 2Kbit   | 4Kbit   | 8Kbit   | 16Kbit  | 32Kbit  | 64Kbit  | 128Kbit | 256Kbit | 512Kbit | 1 Mbit
                Part:      25xx010 | 25xx020 | 25xx040 | 25xx080 | 25xx160 | 25xx320 | 25xx640 | 25xx128 | 25xx256 | 25xx512 | 25xx1024
                Page/Byte: 16      | 16      | 16      | 16(32)  | 16(32)  | 32      | 32      | 64      | 64      | 128     | 256
                Addr/Bits: 7       | 8       | 9       | 16      | 16      | 16      | 16      | 16      | 16      | 16      | 24
 -------------------------------------------------------------------------------------------------*/
/*-- Includes --*/
#include "eeprom25xx/eeprom25xx.h"
#include "gpios/gpios.h"
#include "spi/spi.h"
#include "hardware_profile.h"


/*-- Private Definitions--*/
#define READ            0x03  /*Read data from memory array beginning at selected address*/
#define WRITE           0x02  /*Write data to memory array beginning at selected address*/
#define WRDI            0x04  /*Reset the write enable latch (disable write operations)*/
#define WREN            0x06  /*Set the write enable latch (enable write operations)*/
#define RDSR            0x05  /*Read STATUS register*/
#define WRSR            0x01  /*Write STATUS register*/
#define DUMMY           0x55  /*Summy byte for read operations*/


/*-- Private Macros --*/
static _U08 gu8Density;
static _U32 gau8SpiPorts[EEPROM25XX_N_MEMORIES] =
{
    #if EEPROM25XX_N_MEMORIES > 0
        EEPROM25XX_SPI0,
    #endif
    #if EEPROM25XX_N_MEMORIES > 1
        EEPROM25XX_SPI1,
    #endif
    #if EEPROM25XX_N_MEMORIES > 2
        EEPROM25XX_SPI2,
    #endif
    #if EEPROM25XX_N_MEMORIES > 3
        EEPROM25XX_SPI3,
    #endif
    #if EEPROM25XX_N_MEMORIES > 4
        #error "numero de memorias no soportado"
    #endif
};
static _U32 gau8PortSS[EEPROM25XX_N_MEMORIES] =
{
    #if EEPROM25XX_N_MEMORIES > 0
        EEPROM25XX_SS_P0,
    #endif
    #if EEPROM25XX_N_MEMORIES > 1
        EEPROM25XX_SS_P1,
    #endif
    #if EEPROM25XX_N_MEMORIES > 2
        EEPROM25XX_SS_P2,
    #endif
    #if EEPROM25XX_N_MEMORIES > 3
        EEPROM25XX_SS_P3,
    #endif
    #if EEPROM25XX_N_MEMORIES > 4
        #error "numero de memorias no soportado"
    #endif
};
static _U32 gau8PinSS[EEPROM25XX_N_MEMORIES] =
{
    #if EEPROM25XX_N_MEMORIES > 0
        EEPROM25XX_SS_B0,
    #endif
    #if EEPROM25XX_N_MEMORIES > 1
        EEPROM25XX_SS_B1,
    #endif
    #if EEPROM25XX_N_MEMORIES > 2
        EEPROM25XX_SS_B2,
    #endif
    #if EEPROM25XX_N_MEMORIES > 3
        EEPROM25XX_SS_B3,
    #endif
    #if EEPROM25XX_N_MEMORIES > 4
        #error "numero de memorias no soportado"
    #endif
};


/*-- Private functions prototypes --*/


/*-- External functions --*/
/**-----------------------------------------------------------------------------------------------*/
void Eeprom25xx_Init(const _U08 u8Memory, const _U08 u8Density)
{
    if(u8Density < EEPROM25XX_1M)
    {
        Gpios_PinDirection(gau8PortSS[u8Memory], gau8PinSS[u8Memory], GPIOS_OUTPUT);
        gu8Density = u8Density;
    }
}
/**-----------------------------------------------------------------------------------------------*/

/**-----------------------------------------------------------------------------------------------*/    
_U08 Eeprom25xx_u8ReadStatus(const _U08 u8Memory)
{
    _U08 u8Status;

    Gpios_WritePin(gau8PortSS[u8Memory], gau8PinSS[u8Memory], 0);
    (void)Spi_u8Transfer(gau8SpiPorts[u8Memory], RDSR);
    u8Status = Spi_u8Transfer(gau8SpiPorts[u8Memory], DUMMY);
    Gpios_WritePin(gau8PortSS[u8Memory], gau8PinSS[u8Memory], 1);

    return u8Status;
}
/**-----------------------------------------------------------------------------------------------*/    

/**-----------------------------------------------------------------------------------------------*/
void Eeprom25xx_Read(const _U08 u8Memory, _U08 *pu8Buffer, _U32 u32Addr, _U16 u16Size)
{
    _U16 i;

    Gpios_WritePin(gau8PortSS[u8Memory], gau8PinSS[u8Memory], 0);
    if(gu8Density == EEPROM25XX_4K)
    {
        (void)Spi_u8Transfer(gau8SpiPorts[u8Memory], (READ | ((_U08)(u32Addr>>(_U32)8))<<3));        /*mandamos comando write*/
    }
    else
    {
        (void)Spi_u8Transfer(gau8SpiPorts[u8Memory], READ);        /*mandamos comando write*/
    }
    if(gu8Density > EEPROM25XX_512K)
    {
        (void)Spi_u8Transfer(gau8SpiPorts[u8Memory], (_U08)(u32Addr>>(_U32)16));        /*parte alta de la direccion de la memoria*/
    }
    if(gu8Density > EEPROM25XX_4K)
    {
        (void)Spi_u8Transfer(gau8SpiPorts[u8Memory], (_U08)(u32Addr>>(_U32)8));        /*parte alta de la direccion de la memoria*/
    }
    (void)Spi_u8Transfer(gau8SpiPorts[u8Memory], (_U08)u32Addr);        /*parte baja de la direccion de la memoria*/
    for(i=0;i<u16Size;i++)
    {
        *pu8Buffer = Spi_u8Transfer(gau8SpiPorts[u8Memory], DUMMY);        /*escribimos un byte 0x23*/
        pu8Buffer++;
    }
    Gpios_WritePin(gau8PortSS[u8Memory], gau8PinSS[u8Memory], 1);
}
/**-----------------------------------------------------------------------------------------------*/

/**-----------------------------------------------------------------------------------------------*/
void Eeprom25xx_Write(const _U08 u8Memory, _U08 *pu8Buffer, _U32 u32Addr, _U16 u16Size)
{
    _U16 i;

    Gpios_WritePin(gau8PortSS[u8Memory], gau8PinSS[u8Memory], 0);
    (void)Spi_u8Transfer(gau8SpiPorts[u8Memory], WREN);        /*mandamos comando de write enable*/
    Gpios_WritePin(gau8PortSS[u8Memory], gau8PinSS[u8Memory], 1);

    Gpios_WritePin(gau8PortSS[u8Memory], gau8PinSS[u8Memory], 0);
    if(gu8Density == EEPROM25XX_4K)
    {
        (void)Spi_u8Transfer(gau8SpiPorts[u8Memory], (WRITE | ((_U08)(u32Addr>>(_U32)8))<<3));        /*mandamos comando write*/
    }
    else
    {
        (void)Spi_u8Transfer(gau8SpiPorts[u8Memory], WRITE);
    }
    if(gu8Density > EEPROM25XX_512K)
    {
        (void)Spi_u8Transfer(gau8SpiPorts[u8Memory], (_U08)(u32Addr>>(_U32)16));        /*parte alta de la direccion de la memoria*/
    }
    if(gu8Density > EEPROM25XX_4K)
    {
        (void)Spi_u8Transfer(gau8SpiPorts[u8Memory], (_U08)(u32Addr>>(_U32)8));        /*parte alta de la direccion de la memoria*/
    }
    (void)Spi_u8Transfer(gau8SpiPorts[u8Memory], (_U08)u32Addr);        /*parte baja de la direccion de la memoria*/

    for(i=0;i<u16Size;i++)
    {
        (void)Spi_u8Transfer(gau8SpiPorts[u8Memory], *pu8Buffer);        /*escribimos un byte 0x23*/
        pu8Buffer++;
    }
    Gpios_WritePin(gau8PortSS[u8Memory], gau8PinSS[u8Memory], 1);
}
/**-----------------------------------------------------------------------------------------------*/


/*-- Private functions --*/
/**-------------------------------------------------------------------------------------------------    
  \brief      None
  \param      None
  \return     None
  \warning    None
--------------------------------------------------------------------------------------------------*/


