Pulse Width Modulation (PWM)
=================================


Driver para controlar la generación de señales PWM por hardware, las señales son generadas usando los módulos ECCP del microcontrolador (el cual posee dos). Además de estos módulos, se depende de dos timer del micro, el Timer2 lo usa el módulo ECCP1 y el Timer4 lo usa el módulo ECCP2.

Las siguientes limitaciones del modulo deben tomarse en cuenta:
- Apesar de poseer 4 canales por modulo PWM solo se puede tener una señal de ciclo util en un solo canal por vez o tener la misma señal en dos, tres o cuatro canales.
- Se debe asignar los pines a los que estaran conectados los canales de PWM mediante la funcion `System_PeripheralPinSelect` del driver system.
- Se deben setear los gpios a los que son asignados los canales como salidas mediante la funcion `Gpios_PinDirection`
- Cuando se incializa el modulo es necesario tomar en cuenta que la fecuencia estara limitada por la siguiente formula

```C
                             (BSP_CLOCK / 4)
                   PR = ------------------------  - 1   /*El valor de PR debe estar en el rango de 0 a 255*/
                          u32Frec * PWM_DIVISOR
```

Es necesario configurar esta pieza de código antes de utilizarse. en el archivo **hardware_profile.h** se debe agregar la siguiente linea que indica la frecuencia de operación del micro:

```C
#define BSP_CLOCK           12000000 /*Varia el valor de esta definición acorde a la frecuencia de tu aplicación*/
```

Si no se define esta linea de código, la velocidad que usara el driver por defaul sera de 12MHz.

Ejemplo de uso
--------------

Pulso al 80% en el canal A con una frecuencia de 1KHz
```C
#include <p18cxxx.h>
#include "vectors.h"
#include "types.h"
#include "gpios/gpios.h"
#include "pwm/pwm.h"


#pragma code
void main(void)
{
    _U08 byte;

    ANCON0 = 0XFF;  /*Desactivamos las entradas analógicas*/
    ANCON1 = 0XFF;  /*Desactivamos las entradas analógicas*/

    System_PeripheralPinSelect(CompareOrPWMOutputChannel1A, 2);   /*PWM1_A en el pin RP2 (A5)*/
    Gpios_PinDirection(GPIOS_PORTA, 5, GPIOS_OUTPUT);  /*pin RA5 como salida para PWM canal A*/

    Pwm_Init(PWM_PORT1, 1000);
    Pwm_DutyCycle(PWM_PORT1, PWM_CHANNEL_A, 80);

    while (1)
    {

    }
}

```

Configuración
-------------

El driver puede configurarse para que pueda generar las señales a la frecuncia indicada en la funcion `Pwm_Init`. En el archivo **hardware_profile.h** se pueden definir las siguientes constantes:
```C
#define BSP_CLOCK                12000000 /*Freuencia en Hz a la que corre el micro (valor por default)*/
#define PWM_DIVISOR              16 /*Valor de preescaler para la base de tiempo del PWM (valor por default)*/
```
Si no se definen estas constantes se tomaran sus valores por default.
Los valores combinados se conjuntan para generar la base de tiempo interna, la cual obedece a la siguiente formula:

API
---

```C
	/**---------------------------------------------------------------------------------------------
      \def      _ePWM
      \brief    Moulos pwm que pueden ser usados en el driver
    ----------------------------------------------------------------------------------------------*/
    typedef enum
    {
        PWM_PORT1 = 1,
        PWM_PORT2
    }_ePWM;

    /**---------------------------------------------------------------------------------------------
      \def      _ePWM_CHANNELS
      \brief    Canales pwm que pueden ser usados en ambos PWM, se pueden usar uno por vez o demanera
                simultanea
    ----------------------------------------------------------------------------------------------*/
    typedef enum
    {
        PWM_CHANNEL_A = (1<<0),
        PWM_CHANNEL_B = (1<<1),
        PWM_CHANNEL_C = (1<<2),
        PWM_CHANNEL_D = (1<<3)
    }_ePWM_CHANNELS;


    /*-- Functions --*/
    /**---------------------------------------------------------------------------------------------
      \brief      Inicializa el periférico ECCPx en modo pwm para generar una señal con la frecuencia
                  indicada
      \param	  u8Pwm.- Modulo PWM a ser inicializado
      \param      u32Frec.- Frecuencia a la cual se generara el pulso del PWM
      \return     None
      \warning	  Debes tomar en cuenta que el valor de la frecuencia se calcula apartir de la
                  siguiente formula y que PR solo puede tener valores de 0 a 255:
                             (BSP_CLOCK / 4)
                   PR = ------------------------  - 1
                          u32Frec * PWM_DIVISOR
    ----------------------------------------------------------------------------------------------*/
    void Pwm_Init(_U08 u8Pwm, _U32 u32Frec);

    /**---------------------------------------------------------------------------------------------
      \brief      Establece el duty cycle de la señal en el canal seleccionado
      \param	  u8Pwm.- Modulo PWM con el canal a configurar
      \param      u8Channel.- Canal a setear su duty cycle (solo usar los valores en _ePWM_CHANNELS)
      \param      u8Duty.- Porcentaje de la señal en nivl alto, valores de 0 a 100 unicamente
      \return     None
      \warning	  En el mismo modulo pwm no se puede setear canales con duty cycle diferente
    ----------------------------------------------------------------------------------------------*/
    void Pwm_DutyCycle(_U08 u8Pwm, _U08 u8Channel, _U08 u8Duty);
```

Ejemplos
--------

- [Ejemplo 1: Señal a 1KHz y 80% en el canal A del modulo PWM 1][1]
- [Ejemplo 2: Señales a 2KHz al 30% en los canales A, B y C del modulo PWM 1][2]
- [Ejemplo 3: Señales a una frecuencia de 1KHz con diferente duty cycle usando PWM1 y PWM2][3]
- [Ejemplo 4: Simple dimmer de 1% a 100% en el PWM 1 canal A, 750Hz de Frecuencia][4]


  [1]: https://github.com/Hotboards/Examples/blob/master/Microchip/pwm1.X/main.c
  [2]: https://github.com/Hotboards/Examples/blob/master/Microchip/pwm2.X/main.c
  [3]: https://github.com/Hotboards/Examples/blob/master/Microchip/pwm3.X/main.c
  [4]: https://github.com/Hotboards/Examples/blob/master/Microchip/pwm4.X/main.c

