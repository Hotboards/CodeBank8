/*--------------------------------------------------------------------------------------------------
  "THE BEER-WARE LICENSE" (Revision 42):
  <phk@FreeBSD.ORG> wrote this file. As long as you retain this notice you
  can do whatever you want with this stuff. If we meet some day, and you think
  this stuff is worth it, you can buy me a beer in return to Hotboards crew
  (Beer-ware license created by Poul-Henning Kamp)

  \file         eeprom_25xx.h
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
#ifndef __EEPROM25XX_H__
#define __EEPROM25XX_H__


    /*-- Includes --*/
    #include "types.h"
     

    /*-- Defines --*/
    /**---------------------------------------------------------------------------------------------
      \def        _eEEPROM25XX_DENSITY
      \brief      Densidad en las que se pueden encontrar las memorias 25xx
    ----------------------------------------------------------------------------------------------*/
    typedef enum
    {
        EEPROM25XX_1K = 0,
        EEPROM25XX_2K,
        EEPROM25XX_4K,
        EEPROM25XX_8K,
        EEPROM25XX_16K,
        EEPROM25XX_32K,
        EEPROM25XX_64K,
        EEPROM25XX_128K,
        EEPROM25XX_256K,
        EEPROM25XX_512K,
        EEPROM25XX_1M
    }_eEEPROM25XX_DENSITY;

    /**---------------------------------------------------------------------------------------------
      \def        _eEEPROM25XX_MEMORIES
      \brief      Memorias que pueden ser manejadas por esta pieza de codigo
    ----------------------------------------------------------------------------------------------*/
    typedef enum
    {
        EEPROM25XX_MEMORY0 = 0,
        EEPROM25XX_MEMORY1,
        EEPROM25XX_MEMORY2,
        EEPROM25XX_MEMORY3
    }_eEEPROM25XX_MEMORIES;

    /**---------------------------------------------------------------------------------------------
      \def        _eEEPROM25XX_STATUS
      \brief      Mascaras que permiten leer los valores del registro de estatus interno de la memoria
    ----------------------------------------------------------------------------------------------*/
    typedef enum
    {
        EEPROM25XX_BUSY     = 0x01,
        EEPROM25XX_WREN     = 0x02
    }_eEEPROM25XX_STATUS;


    /*-- Macros --*/
    /**--------------------------------------------------------------------------------------------- 
      \def      Define
      \brief    Write a nice descripción for your macros
    ----------------------------------------------------------------------------------------------*/ 
    
    
    /*-- Functions --*/
    /**---------------------------------------------------------------------------------------------    
      \brief      Inicializa el pin de CS que seleccionara la memoria e indica a las demás funciones
                  la densidad de la memoria
      \param	  u8Memory.- Memoria a incializar (solo usar enums _eEEPROM25XX_MEMORIES)
      \param	  u8Density.- Densidad de la memoria (solo usar enums _eEEPROM25XX_DENSITY)
      \return     None
      \warning	  Esta función es necesaria antes de mandar llamar cualquier otra función del driver
    ----------------------------------------------------------------------------------------------*/
    void Eeprom25xx_Init(const _U08 u8Memory, const _U08 u8Density);

    /**---------------------------------------------------------------------------------------------
      \brief      lee el registro de estatus de la memoria
      \param	  u8Memory.- Memoria a incializar (solo usar enums _eEEPROM25XX_MEMORIES)
      \return     Valor del registro de estatus
      \warning	  Con esta funcion se puede interrogar si al memoria esta ocupada
    ----------------------------------------------------------------------------------------------*/
    _U08 Eeprom25xx_u8ReadStatus(const _U08 u8Memory);

    /**---------------------------------------------------------------------------------------------    
      \brief      Lee una cantidad de bytes dada de una direccion de la memoria
      \param	  u8Memory.- Memoria a incializar (solo usar enums _eEEPROM25XX_MEMORIES)
      \param	  pu8Buffer.- Puntero a la localidad de memoria donde se almacenara los bytes leídos
      \param	  u32Addr.- Dirección dentro de la memoria a leer
      \param	  u16Size.- Cantidad de bytes a leer
      \return     None
      \warning	  se recomienda preguntar si la memoria no esta ocupada antes de usar esta funcion
    ----------------------------------------------------------------------------------------------*/
    void Eeprom25xx_Read(const _U08 u8Memory, _U08 *pu8Buffer, _U32 u32Addr, _U16 u16Size);

    /**---------------------------------------------------------------------------------------------    
      \brief      Inicializa el controlador hd44780, sin cursor, dos lineas y fuente 5x8
      \param	  u8Memory.- Memoria a incializar (solo usar enums _eEEPROM25XX_MEMORIES)
      \param	  pu8Buffer.- Puntero a la localidad de memoria donde se almacenara los bytes leídos
      \param	  u32Addr.- Dirección dentro de la memoria a escribir
      \param	  u16Size.- Cantidad de bytes a leer
      \return     None
      \warning	  se recomienda preguntar si la memoria no esta ocupada antes de usar esta función
                  Esta función solo manda los datos al buffer interno de la memoria y da la orden de
                  escritura pero no espera a que la memoria termine de escribir los datos
    ----------------------------------------------------------------------------------------------*/
    void Eeprom25xx_Write(const _U08 u8Memory, _U08 *pu8Buffer, _U32 u32Addr, _U16 u16Size);


#endif   
 