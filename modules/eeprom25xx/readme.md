Memoria eeprom microchip (25xx)
================================


Esta pieza de codigo controla Pieza de código para controlar la serie de memorias 25xx de la marca Microchip, las cuales poseen interfaz serial spi. El driver es capaz de controlar hasta 4 memorias las cuales pueden estar conectadas a un mismo modulo spi o a diferentes modulos.
                
Es necesario definir esto en el archivo hardware_profile.h. las memorias de la serie 25 pueden venir en combinaciones de.

| Density:   | 1Kbit   | 2Kbit   | 4Kbit   | 8Kbit   | 16Kbit  | 32Kbit  | 64Kbit  | 128Kbit | 256Kbit | 512Kbit | 1 Mbit |
|---------|---|--|
| Part: | 25xx010 | 25xx020 | 25xx040 | 25xx080 | 25xx160 | 25xx320 | 25xx640 | 25xx128 | 25xx256 | 25xx512 | 25xx1024 |
| Page/Byte: | 16      | 16      | 16      | 16(32)  | 16(32)  | 32      | 32      | 64      | 64      | 128     | 256 |
| Addr/Bits: | 7       | 8       | 9       | 16      | 16      | 16      | 16      | 16      | 16      | 16      | 2 |

El código no es especifico de ningun microcontrolador, solo se requiere que sea de 8bits.  Es dependiente de, **gpios/gpios.h**, **spi/spi.h** y se configura atravez del archivo **hardware_profile.h**

Este tipo de memorias estan divididas en paginas dependiendo la densidad de la memoria sera el tamaño de su paginas. No se puede escribir entre limites de la pagina. Ejemplo se tiene la memoria 25xx320 que posee paginas de 32 bytes, si vas a escribir 10 bytes apartir de la direccion 27 tendras que hacerlo en dos partes, 5 bytes apartir de la direccion 27 y los otros 5 apartir de la direccion 32. Recuerda que cada que mandes escribir un dato en la memoria tendras que esperar 5ms a que se graben.


**Ejemplo de uso**

Simple codigo que escribe una pagina entera en la memoria si y solo si esta desocupada y libre para escritura

```C
#include "types.h"
#include "system/system.h"
#include "spi/spi.h"
#include "eeprom25xx/eeprom25xx.h"

_U08 buffer[32];

void main(void)
{
    Spi_Init(SPI_PORT1, SPI_4DIV);
    Eeprom25xx_Init(EEPROM25XX_MEMORY0, EEPROM25XX_32K);
    
    while (1)
    {
        if((Eeprom25xx_u8ReadStatus(EEPROM25XX_MEMORY0) & EEPROM25XX_BUSY) == 0)
        {
            Eeprom25xx_Write(EEPROM25XX_MEMORY0, &gau8BufferOut[0], 0, 32);
        }
    }
}
```


**Configuracion**

Antes de utilizar este código se debe realizar algunas configuraciones dentro del archivo hardware_profile.h
Se debe indicar el numero de memorias que la aplicación utilizara

```C
#define EEPROM25XX_N_MEMORIES          1 /*su valor por defaul es cero*/
```

Se debe indicar que puerto spi comunicara a cada memoria en uso, ademas del pin que controlara su activacion (pin SS)

```C
#define EEPROM25XX_SPI0                SPI_PORT2  /*puerto spi*/

#define EEPROM25XX_SS_P0               GPIOS_PORTE /*puerto*/
#define EEPROM25XX_SS_B0               2            /*pin*/
/*en caso de utilizarse mas entradas se debe repetir */
#define EEPROM25XX_SPIx	               SPI_PORT0 

#define EEPROM25XX_SS_Px_P             GPIOS_PORT2 //puerto
#define EEPROM25XX_SS_Bx_B             3			//pin
/* donde (x) es la siguiente entrada a asignar, el numero de entradas declaradas 
debe coincidir con el valor de AT45DB_N_MEMORIES, y el ultimo valor de (x) deberá 
ser EEPROM25XX_N_MEMORIES-1 */
```


**Interfaz**

```C
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

        
    /*-- Functions --*/
    /**---------------------------------------------------------------------------------------------    
      \brief      Inicializa el pin de CS que seleccionara la memoria e indica a las demas funciones
                  la debsidad de la memoria
      \param	  u8Memory.- Memoria a incializar (solo usar enums _eEEPROM25XX_MEMORIES)
      \param	  u8Density.- Densidad de la memoria (solo usar enums _eEEPROM25XX_DENSITY)
      \return     None
      \warning	  Esta funcion es necesaria antes de mandar llamar cualquier otra funcion del driver
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
      \param	  pu8Buffer.- Puntero a la localidad de memoria donde se almacenara los bytes leidos
      \param	  u32Addr.- Direccion detro de la memoria a leer
      \param	  u16Size.- Cantidad de bytes a leer
      \return     None
      \warning	  se recomienda preguntar si la memoria no esta ocupada antes de usar esta funcion
    ----------------------------------------------------------------------------------------------*/
    void Eeprom25xx_Read(const _U08 u8Memory, _U08 *pu8Buffer, _U32 u32Addr, _U16 u16Size);

    /**---------------------------------------------------------------------------------------------    
      \brief      Inicializa el controlador hd44780, sin cursor, dos lineas y fuente 5x8
      \param	  u8Memory.- Memoria a incializar (solo usar enums _eEEPROM25XX_MEMORIES)
      \param	  pu8Buffer.- Puntero a la localidad de memoria donde se almacenara los bytes leidos
      \param	  u32Addr.- Direccion dentro de la memoria a escribir
      \param	  u16Size.- Cantidad de bytes a leer
      \return     None
      \warning	  se recomienda preguntar si la memoria no esta ocupada antes de usar esta funcion
                  Esta funcion solo manda los datos al buffer interno de la memoria y da la orden de
                  escritura pero no espera a que la memoria termine de escibir los datos
    ----------------------------------------------------------------------------------------------*/
    void Eeprom25xx_Write(const _U08 u8Memory, _U08 *pu8Buffer, _U32 u32Addr, _U16 u16Size);

    
```

**Ejemplos**

- [Ejemplo 1: Escribir y leer una pagina entera en la memoria][1]

[1]: https://github.com/Hotboards/Examples/blob/master/Microchip/eprom25xx1.X/main.c