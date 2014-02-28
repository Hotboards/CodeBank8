Serial Peripheral Interface (spi)
=================================


Driver para controlar la comunicación serial sincrona de tipo spi usando el periférico **MSSP** del uC. El periférico **MSSP** de los **PIC18FJ** puede tener dos funciones, como spi o i2c, en este caso este driver lo configura para ser usado únicamente como spi en modo maestro. Aparte de solo implementarse como maestro, no se implementan interrupciones pues solo se usa una simple función bloqueante para tx y rx un byte por vez.

En futuras ediciones de este driver se implementara el modo esclavo basado en interrupciones. Es deber del usuario asignar de manera manual los pines que actuaran como selector de esclavos, pues esta pieza de código no lo realiza por default, dejando esta tarea a la aplicación, la cual puede ser realizada mediante el driver de **gpios**. 


Ejemplo de uso
--------------

Simple envió y recepción de un byte
```C
#include <p18cxxx.h>
#include "vectors.h"
#include "types.h"
#include "spi/spi1.h"
#include "gpios/gpios.h"

#define DUMMY  0xaa

#pragma code
void main(void)
{
    _U08 byte;

    ANCON0 = 0XFF;  /*Desactivamos las entradas analógicas*/
    ANCON1 = 0XFF;  /*Desactivamos las entradas analógicas*/

    Gpios_PinDirection(GPIOS_PORTD, 3, GPIOS_OUTPUT); /*pin RD3 como salida para actuar como SS*/
    Gpios_WritePin(GPIOS_PORTD, 3, 1);                /*inhabilitamos esclavo*/
    Gpios_PinDirection(GPIOS_PORTC, 7, GPIOS_OUTPUT); /*pin RD7 como salida para SDO*/
    Gpios_PinDirection(GPIOS_PORTB, 4, GPIOS_OUTPUT); /*pin RB4 como salida para SCK*/
    Gpios_PinDirection(GPIOS_PORTB, 5, GPIOS_INPUT);  /*pin RB4 como salida para SDI*/
    
    Spi1_Init(SPI_PORT1, SPI_4DIV);                   /*inicializamos spi a 12MHz/4 = 3MHz*/
    
    while (1)
    {
        Gpios_WritePin(GPIOS_PORTD, 3, 0);            /*habilitamos dispositivo esclavo*/
        (void)Spi1_u8Transfer(SPI_PORT1, 0x9F);       /*escribimos un byte*/
        Gpios_WritePin(GPIOS_PORTD, 3, 1);            /*des habilitamos dispositivo esclavo*/

		Gpios_WritePin(GPIOS_PORTD, 3, 0);            /*habilitamos dispositivo esclavo*/
        byte = Spi1_u8Transfer(SPI_PORT1, DUMMY);     /*leemos un byte*/
        Gpios_WritePin(GPIOS_PORTD, 3, 1);            /*des habilitamos dispositivo esclavo*/
    }
}

```

Configuración
-------------

Esta pieza de código no requiere configuración alguna, ni siquiera es necesario indicarle la frecuencia de operación de uC

API
---

```C
/**--------------------------------------------------------------------------------------------- 
  \def      SPI_DIV
  \brief    Valores que dividen la frecuencia de operación del uC (valores 4, 16, 64)
----------------------------------------------------------------------------------------------*/ 
#define SPI_4DIV                        0
#define SPI_16DIV                       1
#define SPI_64DIV                       2
    
/*----------------------------------------------------------------------------------------------
|                            SPI_BUSCLK                                                        |
|   Baud rate generation  = ------------                                                       |
|                           (SPI_DIV)                                                          |
----------------------------------------------------------------------------------------------*/
    
    
/*-- Functions --*/
/**---------------------------------------------------------------------------------------------    
  \brief      Inicializa el periférico MSSP como SPI en modo maestro
  \param	  u8Divisor.- divide la frecuencia de operación del cpu, SPI_4DIV, SPI_16DIV, SPI_64DIV
  \param      u8Spi.- Puerto spi que sera inicializado
  \return     None
  \warning	  El periférico se inicia con modo de transmicion 0
----------------------------------------------------------------------------------------------*/
void Spi1_Init(_U08 u8Spi, _U08 u8Divisor);
    
/**---------------------------------------------------------------------------------------------
  \brief      Establece el modo de transmicion SPI a usar
  \param	  u8Mode.- mode  clk pha         descripción
                        0      0,  0   (clk idle=0, data sample=rising)
                        1      0,  1   (clk idle=0, data sample=fall)
                        2      1,  0   (clk idle=1, data sample=fall)
                        3      1,  1   (clk idle=1, data sample=rise)
  \param      u8Spi.- Puerto spi que se seteara su modo de trasnmision
  \return     None
  \warning	  None
----------------------------------------------------------------------------------------------*/
void Spi1_SetMode(_U08 u8Spi, _U08 u8Mode);
    
/**---------------------------------------------------------------------------------------------
  \brief      Transmite un simple byte, ademas de regresar un byte de llegada (si lo hay)
  \param	  u8TxByte.- byte a transmitir
  \param      u8Spi.- Puerto spi que sera usado para transferir un dato
  \return     byte recibido
  \warning	  Esta función traba el cpu hasta que se termine de transmitir y/o recibir un byte
----------------------------------------------------------------------------------------------*/
_U08 Spi1_u8Transfer(_U08 u8Spi, _U08 u8TxByte);

```

Ejemplos
--------

- [Ejemplo 1: Transmicion y recepción de datos][1]
- [Ejemplo 2: Transmicion y recepción de datos con spi2][2]


  [1]: https://github.com/Hotboards/Examples/blob/master/Microchip/spi1.X/main.c
  [2]: https://github.com/Hotboards/Examples/blob/master/Microchip/spi2.X/main.c

