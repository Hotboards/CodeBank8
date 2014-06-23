Interfaz Paralela 6800 (_6800)
==============================


Esta pieza de código emula la interfaz paralela 6800 mediante simples pines del uC, las tres señales de control (E, RS y RW) utilizadas en esta interfaz se generan mediante el driver **gpios**. La amplitud del bus solo puede ser de 4 u 8 bits y para ello se utiliza un puerto completo del uC, en caso de 4 bits solo se usan los bits mas significativos.

La señal enable la cual tipicamene es un pulso se establece de manera fija a 1 micro segundo. Este tiempo es mas que suficiente pues tipicamnete es acorde a los 300ns

El driver es dependiente de ***types.h**, **delays/delays**, **gpios/gpios.h** y **hardware_profile.h**

Ejemplo de uso
--------------

Simple escritura en modo de 8 bits
```C
#include "types.h"
#include "6800/_6800.h"

int main(void)
{
    _6800_Init();   /*incializamos la interfaz paralela*/
	_6800_WriteData('a'); /*escribimos un simple dato*/
	while (1)
    {

    }
}
```

Simple escritura en modo de 4 bits
```C
#include "types.h"
#include "6800/_6800.h"

int main(void)
{
    _6800_Init();   /*incializamos la interfaz paralela*/
	_6800_WriteData('a'); /*escribimos el nibble mas significativo*/
	_6800_WriteData('a' << 4u); /*escribimos el nibble menos significativo*/
	
	while (1)
    {

    }
}

```

Configuración
-------------

En el archivo **hardware_profile** se debe indicar de manera obligatorio los pines que actuaran en la interfaz 6800, de la siguiente manera.
```C
#define _6800_BUSLENGHT             8   /*numero de bits enviados por vez (valores de 4 o 8)*/
#define _6800_DATAPORT              GPIOS_PORTD /*puerto que actúa como bus de datos*/
/*en caso de bus de 4 bits solo se usan los 4 bits mas significativos del puerto*/

/*pin de enable*/
#define _6800_E_P               	GPIOS_PORTC  /*puerto*/
#define _6800_E_B               	6			 /*pin*/	
/*pin de lectura/escritura*/
#define _6800_RW_P                  GPIOS_PORTC  /*puerto*/
#define _6800_RW_B                  7			 /*pin*/
/*pin de datos/comandos*/
#define _6800_RS_P                  GPIOS_PORTB  /*puerto*/
#define _6800_RS_B                  7			 /*pin*/
```

API
---

```C
	/*-- Functions --*/
    /**---------------------------------------------------------------------------------------------
      \brief      Inicializa los pines de control que actuaran como puerto paralelo
      \param	  None
      \return     None
      \warning	  Se deben declarar los pines a usar en el archivo middleware_profile.h
    ----------------------------------------------------------------------------------------------*/
    void _6800_Init(void);

    /**---------------------------------------------------------------------------------------------
      \brief      Escribe un simple dato a través del bus (este valor puede ser de 8 o 4 bits)W/R=0,R/S=1
      \param	  u8Data.- dato a ser enviado a través del bus de datos
      \return     None
      \warning	  En el caso de la opción de 4 bits la función solo toma en cuenta los 4 bits mas altos (4-7)
    ----------------------------------------------------------------------------------------------*/
    void _6800_WriteData(_U08 u8Data);

    /**---------------------------------------------------------------------------------------------
      \brief      lee un dato del dispositivo externo conectado al bus de datos (W/R=1, R/S=1)
      \param	  None
      \return     Dato leído
      \warning	  En el caso de el modo de 4 bits solo se toma en cuenta los 4 bits mas altos (4-7)
    ----------------------------------------------------------------------------------------------*/
    _U08 _6800_u8ReadData(void);

    /**---------------------------------------------------------------------------------------------
      \brief      Esribe un comando al dispositivo externo (W/R=0 and R/S =0)
      \param	  u8Cmd.- Comando a enviar
      \return     None
      \warning	  En el caso de el modo de 4 bits solo se toma en cuenta los 4 bits mas altos (4-7)
    ----------------------------------------------------------------------------------------------*/
    void _6800_WriteCommand(_U08 u08Cmd);

    /**---------------------------------------------------------------------------------------------
      \brief      Lee una dirección del dispositivo externo (W/R=1 and R/S =0)
      \param	  None
      \return     Dirección enviada
      \warning	  En el caso de el modo de 4 bits solo se toma en cuenta los 4 bits mas altos (4-7).
                  En este tipo de dispositivos no se leen comandos pero si direcciones internas.
    ----------------------------------------------------------------------------------------------*/
    _U08 _6800_u8ReadAddr(void);

```

Ejemplos
--------

- [Ejemplo 1: Inicilizacion de un controlador hd44780][1]
- [Ejemplo 2: Escritura de un dato a un controlador hd44780][2]
- [Ejemplo 3: Escritura de un dato a un controlador hd44780 en modo 4 bits a 48MHz][2]

  [1]: https://github.com/Hotboards/Examples/blob/master/Microchip/68001.X/main.c
  [2]: https://github.com/Hotboards/Examples/blob/master/Microchip/68002.X/main.c
  [3]: https://github.com/Hotboards/Examples/blob/master/Microchip/68003.X/main.c
