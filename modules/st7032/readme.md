LCD Alfanumerico (ST7032)
---------------------------
-----------

Pieza de código para controlar un lcd alfanumérico externo con el controlador Sitronix **ST7032**. este código solo se encarga de incializar el lcd, así como mandar comandos y/o datos al lcd, es necesario inicializar adecuadamente el spi que se usara antes de usar las funciones de este driver.

Se necesita implementar funciones para escribir en la memoria de gráficos del lcd "CGRam" 

Este codigo es dependiente de los archivos **types.h**, **spi/spi.h**, **gpios/gpios.h** y **hardware_profile.h** 


####Ejemplo de uso
Envió de cadenas de datos a diferentes posiciones de un lcd de 2x16

```
#include <p18cxxx.h>
#include "vectors.h"
#include "types.h"
#include "gpios/gpios.h"
#include "spi/spi.h"
#include "st7032/st7032.h"

#pragma code
void main(void)
{
    ANCON0 = 0XFF;  /*Desactivamos las salidas analogicas*/
    ANCON1 = 0XFF;  /*Desactivamos las salidas analogicas*/

    Spi_Init(SPI_PORT1, SPI_4DIV);
    ST7032_Init(ST7032_LCD0);

    ST7032_SetCursor(ST7032_LCD0, 0, 1);
    ST7032_WriteString(ST7032_LCD0, "Hola mundo1");
    ST7032_SetCursor(ST7032_LCD0, 1, 2);
    ST7032_WriteString(ST7032_LCD0, "Hola mundo2");

    while (1)
    {

    }
}
```

####Configuración
Se debe seleccionar cuantos lcd se van a controlar con el driver. Definir en el archivo hardware_profile.h 
```
#define ST7032_N_LCDS              1 /*su valor por defaul es cero*/
```

El driver tambien controla las lineas de reset, chip select y RS, las cuales se deben indicar asi como el modulo spi que controlara el lcd
```
#define ST7032_SPI0                SPI_PORT1  /*puerto spi*/
#define ST7032_SS_P0               GPIOS_PORTE /*puerto de chip select*/
#define ST7032_SS_B0               1            /*pin de chip select*/
#define ST7032_RST_P0              GPIOS_PORTE /*puerto de reset*/
#define ST7032_RST_B0              0            /*pin de reset*/
#define ST7032_RS_P0               GPIOS_PORTA /*puerto de RS*/
#define ST7032_RS_B0               3            /*pin de RS*/
/*si se desean agregar mas lcd a controlar solo se dbee repetir*/
#define ST7032_SPIx                SPI_PORT1  /*puerto spi*/
#define ST7032_SS_P0               GPIOS_PORTB /*puerto de chip select*/
#define ST7032_SS_B0               1            /*pin de chip select*/
#define ST7032_RST_P0              GPIOS_PORTB /*puerto de reset*/
#define ST7032_RST_B0              0            /*pin de reset*/
#define ST7032_RS_P0               GPIOS_PORTA /*puerto de RS*/
#define ST7032_RS_B0               2            /*pin de RS*/
/* donde (x) es la siguiente entrada a asignar, el numero de entradas declaradas 
debe coincidir con el valor de AT45DB_N_MEMORIES, y el ultimo valor de (x) deberá 
ser ST7032_N_LCDS-1 */
```

####API 
```
	/**---------------------------------------------------------------------------------------------
      \def        _eST7032_MEMORIES
      \brief      Displays que pueden ser manejadas por esta pieza de codigo
    ----------------------------------------------------------------------------------------------*/
    typedef enum
    {
        ST7032_LCD0 = 0,
        ST7032_LCD1,
        ST7032_LCD2,
        ST7032_LCD3
    }_eST7032_MEMORIES;


    /*-- Functions --*/
    /**---------------------------------------------------------------------------------------------    
      \brief      Inicializa el controlador st7032, sin cursor, dos lineas y fuente 5x8
      \param	  None
      \return     None
      \warning	  Esta funcion usa de retardos y ciclara al uC al menos 230ms
    ----------------------------------------------------------------------------------------------*/
    void ST7032_Init(const _U08 u8Lcd);
    
    /**---------------------------------------------------------------------------------------------    
      \brief      mueve el curso a columna y fila especifica
      \param	  u8Row.- fila (0 o 1)
      \param	  u8Column.- columna (de 0 a 15)
      \return     None
      \warning	  Trava al uC por espacio de 30us
    ----------------------------------------------------------------------------------------------*/
    void ST7032_SetCursor(const _U08 u8Lcd, const _U08 u8Row, const _U08 u8Col);

    /**---------------------------------------------------------------------------------------------
      \brief      manda un comando al controlador
      \param	  u8Data.- dato a enviar
      \return     None
      \warning	  trava al uC por 30us exepto si es un clear screen, le tomara 300uS
    ----------------------------------------------------------------------------------------------*/
    void ST7032_WriteCommand(const _U08 u8Lcd, const _U08 u8Data);

    /**---------------------------------------------------------------------------------------------
      \brief      manda un simple dato a la memoria interna de datos
      \param	  u8Data.- dato a enviar
      \return     None
      \warning	  trava al uC por 30us
    ----------------------------------------------------------------------------------------------*/
    void ST7032_WriteData(const _U08 u8Lcd, const _U08 u8Data);

    /**---------------------------------------------------------------------------------------------
      \brief      Escribe una cadena terminada en cero, almacenada en flash,
      \param	  strString.- puntero a cadena terminada en cero
      \return     None
      \warning	  Trava al uC 30us por cada caracter enviado
    ----------------------------------------------------------------------------------------------*/
    void ST7032_WriteString(const _U08 u8Lcd, const rom _S08 *strString);
```

####Ejemplos

- [Ejemplo 1: Envió de mensajes en diferentes posiciones del lcd][1]


  [1]: https://github.com/Hotboards/Examples/blob/master/Microchip/st7032.X/main.c
