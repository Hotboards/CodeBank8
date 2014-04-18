Display de 7 Segmentos (_7segments)
==============================


Esta pieza de código controla el multiplexado de display de 7 segmentos mediante un bus de datos común.El driver puede controlar de uno hasta cuatro display en configuración ánodo común o cátodo común. Requiere de un puerto completo para el bus de datos y un pin por cada display. Es necesario mandar llamar la función `_7segments_Task()` periódica al menos cada 5ms para cuatro displays.

Una simple función se encarga de setear los valores que se desplegaran en cada display, la que permite desplegar cualquier carácter que se desee combinado los 7 segmentos (mas un punto), donde los segmentos a = bit0, b = bit1, ..., g = bit6 y . = bit7

El driver es dependiente de ***types.h**, **gpios/gpios.h** y **hardware_profile.h**

Ejemplo de uso
--------------

Simple despliegue de un numero de 4 cifras
```C
#include <p18cxxx.h>
#include "vectors.h"
#include "swtimers/swtimers.h"
#include "system/system.h"
#include "7segments/_7segments.h"

#pragma code
void main(void)
{
    ANCON0 = 0xff;
    ANCON1 = 0xff;

    Timers_Init();                      /*inicializamos el driver para genere una interrupcion cada 5ms*/
    _7segments_Init();                  /*configuramos los pines como salidas*/
    _7segments_SetNumbers(1234);
  
    __ENABLE_INTERRUPTS();   /*se habilitan las interrupciones globales con prioridad*/

    while (1)
    {
        if(Timers_u16GetTime(0) == 0)/*preguntamos si la interrupcion decremento hasta llegar a 0 el canal 0*/
        {
            Timers_SetTime(0, 5/timers_ms);/*se cumplen los 5ms asi que volvemos a recargar el mismo canal */
            _7segments_Task();              /*actualiza el valor en el display y multiplexa al siguiente display*/
        }
    }
}

#pragma interrupt YourHighPriorityISRCode
void YourHighPriorityISRCode(void)
{
    /*coloca aquí el código que llevará tu interrupción en caso de usarla*/
}

#pragma interruptlow YourLowPriorityISRCode
void YourLowPriorityISRCode(void)
{
    Timers_Isr();
}
```

Configuración
-------------

En el archivo **hardware_profile** se debe indicar de manera obligatorio los pines que se usaran para controlar el diplay de 7 segmentos, de la siguiente manera, 
```C
#define _7SEGMENTS_DIGI_N                    2       /*2 digitos*/
#define _7SEGMENTS_MODE                     1       /*anodo comun*/
#define _7SEGMENTS_PORT                     GPIOS_PORTB /*puerto b como bus de datos*/
/*pines que controlan los comunes*/
#define _7SEGMENTS_DIGI0_P                  GPIOS_PORTD /*primer display*/
#define _7SEGMENTS_DIGI0_B                  7

#define _7SEGMENTS_DIGI1_P                  GPIOS_PORTD /*segundo display*/
#define _7SEGMENTS_DIGI1_B                  6
```

API
---

```C
    /**---------------------------------------------------------------------------------------------
      \def      _e7SEGMENTS_NUMBERS
      \brief    Valores que representan los numeros decimales desplegados en los displays
    ----------------------------------------------------------------------------------------------*/
    typedef enum
    {
        _0 = 0b00111111,
        _1 = 0b00000110,
        _2 = 0b01011011,
        _3 = 0b01001111,
        _4 = 0b01100110,
        _5 = 0b01101101,
        _6 = 0b01111101,
        _7 = 0b00000111,
        _8 = 0b01111111,
        _9 = 0b01101111
    }_e7SEGMENTS_NUMBERS;

    
    /*-- Functions --*/
    /**---------------------------------------------------------------------------------------------
      \brief      Inicializa como salidas los pines que se usaran con el display
      \param    None
      \return     None
      \warning    Se deben declarar los pines a usar en el archivo middleware_profile.h
    ----------------------------------------------------------------------------------------------*/
    void _7segments_Init(void);

    /**---------------------------------------------------------------------------------------------
      \brief      Carga el valor con los segmentos a encender en cada display
      \param    u8Display.- numero de display que desplegara el valor seleccionado (de 0 a _7SEGMENTS_DIGI_N-1)
      \param    u8Value.- valor con los segmentos que se deberán encender
      \return     None
      \warning    Un bit en uno representa un segmento encendido. La función invertirá el valor si el
                  el driver esta configurado como ánodo común.
    ----------------------------------------------------------------------------------------------*/
    void _7segments_SetDisplay(const _U08 u8Display, const _U08 u8Value);

    /**---------------------------------------------------------------------------------------------
      \brief      Regresa el valor con los segmentos a encendidos en el display
      \param    u8Display.- numero de display que se le obtendrá su valor (de 0 a _7SEGMENTS_DIGI_N-1)
      \return     Valor en binario representando cada segmento encendido
      \warning    Un bit en uno representa un segmento encendido.
    ----------------------------------------------------------------------------------------------*/
    _U08 _7segments_u8GetDisplay(const _U08 u8Display);

    /**---------------------------------------------------------------------------------------------
      \brief      Despliega un numero decimal de hasta 4 dígitos en los displays
      \param    u16Value.- numero a desplegar en los displays
      \return     None
      \warning    La función toma en cuenta el numero de displays que se ha configurado. No se toma
                  en cuenta el punto
    ----------------------------------------------------------------------------------------------*/
    void _7segments_SetNumber(const _U16 u16Value);

    /**---------------------------------------------------------------------------------------------
      \brief      Actualiza el valor en los display y multiplexa al siguiente display
      \param    None
      \return     None
      \warning    Esta función se debe mandar llamar de manera periódica al menos cada 5ms
    ----------------------------------------------------------------------------------------------*/
    void _7segments_Task(void);
```

Ejemplos
--------

- [Ejemplo 1: Simple despliegue de 4 numeros ](http://github.com/Hotboards/Examples/blob/master/Microchip/7segments_1.X/main.c)
- [Ejemplo 2: Contador de 0 a 255 con tres display](http://github.com/Hotboards/Examples/blob/master/Microchip/7segments_2.X/main.c)
- [Ejemplo 3: Despliegue de voltaje (0 a 3.3) con punto decimal](http://github.com/Hotboards/Examples/blob/master/Microchip/7segments_3.X/main.c)
