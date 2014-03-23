Inter-Integrated Circuit (I2C)
===========================


Driver para controlar la generación de señales necesarias para implementar el protocolo de comunicaciones sincrona I2C. El driver hace uso del periférico MSSP por lo cual una vez seteado no se puede usar como spi, además sólo opera en modo maestro.

Es necesario configurar esta pieza de código antes de utilizarse. en el archivo **hardware_profile.h** se debe agregar la siguiente linea que indica la frecuencia de operación del micro:

```C
#define BSP_CLOCK           12000000 /*Varia el valor de esta definición acorde a la frecuencia de tu aplicación*/
```

Si no se define esta linea de código, la velocidad que usara el driver por defaul sera de 12MHz.

Ejemplo de uso
--------------

Escritura de un byte
```C
#include <p18cxxx.h>
#include "vectors.h"
#include "types.h"
#include "gpios/gpios.h"
#include "i2c/i2c.h"

#pragma code
void main(void)
{
    Gpios_PinDirection(GPIOS_PORTD, 0, GPIOS_INPUT);  /*SCL2*/
    Gpios_PinDirection(GPIOS_PORTD, 1, GPIOS_INPUT);  /*SDA2*/
    
    I2c_Init(I2C_PORT2, 100000);           /*puerto I2C 2 a 100KHz de velocidad*/

    I2c_Start(I2C_PORT2);                                   /*generamos condicion start*/
    (void)I2c_bTxByte(I2C_PORT2, ADDR_WRITE(0b1001101));    /*madamos direccion del sensor en modo escritura*/
    (void)I2c_bTxByte(I2C_PORT2, 0x01);                     /*mandamos un dato a escribir en el bus*/
    I2c_Stop(I2C_PORT2);         							/*terminamos la comunicacion*/

    while (1)
    {

    }
}
```

Lectura de dos bytes
```C
#include <p18cxxx.h>
#include "vectors.h"
#include "types.h"
#include "gpios/gpios.h"
#include "i2c/i2c.h"

#pragma code
void main(void)
{
    _U08 u8Byte;
    
    Gpios_PinDirection(GPIOS_PORTD, 0, GPIOS_INPUT);  /*SCL2*/
    Gpios_PinDirection(GPIOS_PORTD, 1, GPIOS_INPUT);  /*SDA2*/
    
    I2c_Init(I2C_PORT2, 100000);           /*puerto I2C 2 a 100KHz de velocidad*/

    I2c_Start(I2C_PORT2);                                   /*generamos condicion start*/
    (void)I2c_bTxByte(I2C_PORT2, ADDR_READ(0b1001101));    /*madamos direccion del sensor en modo escritura*/
    u8Byte = 2c_bTxByte(I2C_PORT2, ACK);                     /*mandamos un dato a escribir en el bus*/
    u8Byte = 2c_bTxByte(I2C_PORT2, NACK);                     /*mandamos un dato a escribir en el bus*/
    I2c_Stop(I2C_PORT2);         							/*terminamos la comunicacion*/

    while (1)
    {

    }
}
```

Configuración
-------------

No es necesario configurar esta pieza de codigo, mas alla de indicar la freceuncia al que esta el microcontrolador

API
---

```C
	/**---------------------------------------------------------------------------------------------
      \def      _eI2C
      \brief    Moulos i2c que pueden ser usados en el driver
    ----------------------------------------------------------------------------------------------*/
    typedef enum
    {
        I2C_PORT1 = 1,
        I2C_PORT2
    }_eI2C;

    /**---------------------------------------------------------------------------------------------
      \def      _eACK
      \brief    Pulsos de Acknowledge y Not Acknowledge a mandar o recivir
    ----------------------------------------------------------------------------------------------*/
    typedef enum
    {
        I2C_ACK = 0,
        I2C_NACK
    }_eACK;

    
    /*-- Macros --*/
    /**---------------------------------------------------------------------------------------------
      \def        ADDR_x
      \brief      Macros para indicar una transmision de lectura o escritura en el bus
    ----------------------------------------------------------------------------------------------*/
    #define ADDR_WRITE(add)          (((add) << 1) & (0xfe))
    #define ADDR_READ(add)           (((add) << 1) | (0x01))


    /*-- Functions --*/
    /**---------------------------------------------------------------------------------------------
      \brief      Inicializa el periférico MSSPx en modo I2C en mdod maestro y con la frceuncia
                  indicada
      \param	  u8I2c.- Modulo I2C a ser inicializado
      \param      u32Frec.- Frecuencia a la cual se comunicara el modulo (solo valroes estandar)
      \return     None
      \warning	  Debes tomar en cuenta por estandar se manejan velocidades de 100KHz, 400KHz y 1MHz
    ----------------------------------------------------------------------------------------------*/
    void I2c_Init(_U08 u8I2c, _U32 u32Frec);

    /**---------------------------------------------------------------------------------------------
      \brief      Genera la condicion de start en las lineas de DATA y CLK en el modulo indicado
      \param	  u8I2c.- Modulo I2C en el cuale se generara la condicion de start
      \return     None
      \warning	  None
    ----------------------------------------------------------------------------------------------*/
    void I2c_Start(_U08 u8I2c);

    /**---------------------------------------------------------------------------------------------
      \brief      Transmite un byte por I2C 
      \param	  u8I2c.- Modulo I2C en el cuale se mnadara el byte
      \param	  u8Byte.- byte a transmitir
      \return     El valor de aknowlege que regresa el dispositivo esclavo (valores ACK y NACK)
      \warning	  None
    ----------------------------------------------------------------------------------------------*/
    _BOOL I2c_bTxByte(_U08 u8I2c, _U08 u8Byte);
    
    /**---------------------------------------------------------------------------------------------
      \brief      Recive un byte por I2C 
      \param	  u8I2c.- Modulo I2C en el cuale se recivira el byte
      \param	  bAck.- ACK o NACK a mandar al dispositivo esclavo
      \return     El byte que mando el dispositivo esclavo 
      \warning	  None
    ----------------------------------------------------------------------------------------------*/
    _U08 I2c_u8RxByte(_U08 u8I2c, _BOOL bAck);

    /**---------------------------------------------------------------------------------------------
      \brief      Genera la condicion de repeated start en las lineas de DATA y CLK en el modulo indicado
      \param	  u8I2c.- Modulo I2C en el cuale se generara la condicion
      \return     None
      \warning	  None
    ----------------------------------------------------------------------------------------------*/
    void I2c_RepeatedStart(_U08 u8I2c);

    /**---------------------------------------------------------------------------------------------
      \brief      Genera la condicion de stop en las lineas de DATA y CLK en el modulo indicado
      \param	  u8I2c.- Modulo I2C en el cuale se generara la condicion
      \return     None
      \warning	  None
    ----------------------------------------------------------------------------------------------*/
    void I2c_Stop(_U08 u8I2c);
```

Ejemplos
--------

- [Ejemplo 1: Lectura de un registro, (registro del sensor TC47)][1]
- [Ejemplo 2: Lectura continua de un registro, (lectura de la temperatura)][2]


  [1]: https://github.com/Hotboards/Examples/blob/master/Microchip/i2c1.X/main.c
  [2]: https://github.com/Hotboards/Examples/blob/master/Microchip/i2c2.X/main.c
  
