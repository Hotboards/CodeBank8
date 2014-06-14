Retardos por software (Delays)
==============================

Código para generar retardos por software sin utilizar ningún periférico del microcontrolador. Los retardos los realiza mediante decrementos acorde a la frecuencia de operación del CPU. Se debe tomar en cuenta que el uso de interrupciones puede afectar la duración de los retardos.

El driver gpios tiene dependencia directa del archivo **types.h**.

Es necesario configurar esta pieza de código antes de utilizarse. en el archivo **hardware_profile.h** se debe agregar la siguiente linea que indica la frecuencia de operación del micro:

```C
#define BSP_CLOCK           8000000 /*Varia el valor de esta definición acorde a la frecuencia de tu aplicación*/
```

Si no se define esta linea de código, la velocidad que usara el driver por default sera de 12MHz.

Ejemplos de uso
----------------

Parpadeando un led cada 100ms

```C
#include "types.h"
#include "gpios.h"
#include "delays.h"

void main(void)
{
	Gpios_PinDirection(GPIOS_PORTA, 1, GPIOS_OUTPUT); /*Pin RA1 como salida*/
	for(;;)
	{
    	Gpios_TogglePin(GPIOS_PORTA, 1); /*invierto el estado del pin RA1*/
    	Delays_ms(100);                  /*ciclo al CPU por 100ms*/
	}
}
```

Rotando un led a la izquierda cada 100ms
```C
#include "types.h"
#include "gpios.h"
#include "delays.h"

void main(void)
{
	_U08 var = 1;                           /*declaro variable que contendrá el valor a rotar*/
	Gpios_WriteTris(GPIOS_PORTB, 0xFF);     /*declaro puerto B como salida*/
	for(;;)
	{
    	Gpios_WritePort(GPIOS_PORTB, var);  /*escribo el valor de var en el puerto B*/
    	Delays_ms(100);                     /*ciclo al CPU por 100ms*/
    	LEFT_8SHIFT(var,1);                 /*recorro un bit a la izquierda el valor de var*/
    	if(var == 0)var = 1; 				/*si el valor de var es cero vuelve a empezar*/
	}
}
``` 

Configuración
-------------

Para que los retardos se deban calcular correctamente es necesario indicar la velocidad del microcontrolador en el archivo **hardware_profile**, la cifra esta en hertz y de colocarse se toma 8000000 por default

```C
#define BSP_CLOCK               8000000 /*Varia el valor de esta definición acorde a la frecuencia de tu aplicación*/
```


API
---

```C
/*-- Functions --*/
/**---------------------------------------------------------------------------------------------    
  \brief      Esta función realiza retardos en lapsos de micro segundos
  \param      us.- numero de micro segundos a retardar.
  \return     None
  \warning    Esta función se debe usar con las interrupciones desactivadas
----------------------------------------------------------------------------------------------*/
#define Delays_us(us)                   _delay_us((us))
    
/**---------------------------------------------------------------------------------------------    
  \brief      Esta función realiza retardos en múltiplos de 10 micro segundos
  \param      us.- numero de decenas de micro segundos a retardar.
  \return     None
  \warning    Esta función se debe usar con las interrupciones desactivadas
----------------------------------------------------------------------------------------------*/
#define Delays_10us(us)                   _delay_us((us)*10)

/**---------------------------------------------------------------------------------------------
  \brief      Esta función realiza retardos en lapsos de milisegundos
  \param      ms.- numero de milisegundos a retardar
  \return     None
  \warning    Esta función se debe usar con las interrupciones desactivadas
----------------------------------------------------------------------------------------------*/
void Delays_ms(_U16 ms);
```

Ejemplos
--------

- [Ejemplo 1: Parpadeo de un led cada 100ms][1]
- [Ejemplo 2: Parpadeo de un  puerto completo a 500ms][2]
- [Ejemplo 3: Rotación de leds en un puerto cada 200ms][3]


  [1]: https://github.com/Hotboards/Examples/blob/master/Atmel/delays1/delays1/main.c
  [2]: https://github.com/Hotboards/Examples/blob/master/Atmel/delays2/delays2/main.c
  [3]: https://github.com/Hotboards/Examples/blob/master/Atmel/delays3/delays3/main.c

 