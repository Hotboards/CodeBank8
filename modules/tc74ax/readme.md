Sensor de Temperatura Microchip (TC74Ax)
====================================


Pieza de código para controlar el sensor digital de temperaura ambiental Microchip TC74. El sensor se controla a travez de una interfaz I2C, por lo que es necesario utilizar el driver de i2c del banco de codigo. Es posible leer toda la serie de sensores pertenecientes a esta familia los cuales poseen diferentes direcciones. El codigo permite poner al sensor en modo de bajo consumo y despertarlo.

El código no es especifico de ningún microcontrolador, solo se requiere que sea de 8bits.  Es dependiente de, **i2c/i2c.h** y se configura a través del archivo **hardware_profile.h**

La serie TC74 cosiste en 8 numeros de parte diferentes con funcionalidad exacta, la unica variacion es la direccion que poseen, lo cual permite conectar mas de un sensor en el mismo bus.


Ejemplo de uso
--------------

Simple lectura de la temperatura cuando el sensor esta listo

```C
#include "types.h"
#include "i2c/i2c.h"
#include "gpios/gpios.h"
#include "tc74ax/tc74ax.h"

void main(void)
{
    _S08 i8Temp;

    ANCON0 = 0XFF;  /*Desactivamos las entradas analógicas*/
    ANCON1 = 0XFF;  /*Desactivamos las entradas analógicas*/

    Gpios_PinDirection(GPIOS_PORTD, 0, GPIOS_INPUT);  /*SCL2*/
    Gpios_PinDirection(GPIOS_PORTD, 1, GPIOS_INPUT);  /*SDA2*/

    I2c_Init(I2C_PORT2, 100000);           /*puerto I2C 2 a 100KHz de velocidad*/

    while (1)
    {
    	if(Tc74ax_bReadyFlag(TC74A5)) == _TRUE)
        {
            i8Temp = Tc74ax_ReadTemp(TC74A5);
        }
    }
}
```


Configuración
-------------

Antes de utilizar este código se debe realizar algunas configuraciones dentro del archivo hardware_profile.h
Se debe indicar el puerto I2C que el codigo utilizara para comunicarse con el sensor

```C
#define TC74AX_PORT 					I2C_PORT2 /*puerto a usar I2C_PORT1 o I2C_PORT2*/
```
Si no se declara esta linea de codigo el driver tomara por default elpuerto I2c numero 2

Interfaz
--------

```C
	/**---------------------------------------------------------------------------------------------
      \def        _eTC74AX_ADDR
      \brief      Direcciones de los sensores de temperatura de la serie TC74 de Microchip
    ----------------------------------------------------------------------------------------------*/
    typedef enum
    {
        TC74A0 = 0b1001000,
        TC74A1 = 0b1001001,
        TC74A2 = 0b1001010,
        TC74A3 = 0b1001011,
        TC74A4 = 0b1001100,
        TC74A5 = 0b1001101,
        TC74A6 = 0b1001110,
        TC74A7 = 0b1001111
    }_eTC74AX_ADDR;

    /**---------------------------------------------------------------------------------------------
      \def        _eTC74AX_STANDBY
      \brief      Estados en los que puede estar el sensor de tmeperatura
    ----------------------------------------------------------------------------------------------*/
    typedef enum
    {
        TC74AX_WAKEUP = 0,
        TC74AX_STANDBY
    }_eTC74AX_STANDBY;


    /*-- Functions --*/
    /**---------------------------------------------------------------------------------------------
      \brief      Lee la bandera de status del sensor
      \param	  u8Sensor.- Sensor a leer (solo usar enums _eTC74AX_ADDR)
      \return     1 si el sensor esta listo, 0 si no lo esta
      \warning	  La funcion invade el bus i2c
    ----------------------------------------------------------------------------------------------*/
    _BOOL Tc74ax_bReadyFlag(const _U08 u8Sensor);

    /**---------------------------------------------------------------------------------------------
      \brief      Lee la temperatura ya digital
      \param	  u8Sensor.- Sensor a leer (solo usar enums _eTC74AX_ADDR)
      \return     Temperatura medida, cantidad entera con signo
      \warning	  se recomienda preguntar el estado del sensor nates de usar la funcion, en especial
                  si el sensor se acaba de encender
    ----------------------------------------------------------------------------------------------*/
    _S08 Tc74ax_ReadTemp(const _U08 u8Sensor);

    /**---------------------------------------------------------------------------------------------
      \brief      Coloca al sensor en modo de bajo consumo o lo despierta
      \param	  u8Sensor.- Sensor a apagar/encender (solo usar enums _eTC74AX_ADDR)
      \param	  bStatus.- Estado del sensor (usar valores de enum _eTC74AX_STANDBY)
      \return     None
      \warning	  None
    ----------------------------------------------------------------------------------------------*/
    void Tc74ax_Standby(const _U08 u8Sensor, _BOOL bStatus);
```

Ejemplos
--------

- [Ejemplo 1: Lectura de la temperatura][1]

  [1]: https://github.com/Hotboards/Examples/blob/master/Microchip/TC74Ax1.X/main.c