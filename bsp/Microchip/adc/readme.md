Convertidor análogo digital (adc)
---------------------------------
----------

Pieza de código que maneja el convertidor análogo digital interno de el uC. mediante la función de inicializacion se puede elegir resoluciones de 8 y 10 bits, ademas de escoger el canal que se leerá cada que se efectué una operación de lectura. Debido a la cantidad tan pequeña de tiempo (en el orden de los micro segundos), no se utilizan interrupciones.

El tiempo de adquisición del modulo adc se ajusta automáticamente dependiendo de la frecuencia de operación del CPU, y el tiempo de conversión se fija a 12 veces el tiempo de adquisición.  

El driver gpios tiene dependencia directa del archivo **types.h**.

Es necesario configurar esta pieza de código antes de utilizarse. en el archivo **bsp_profile.h** se debe agregar la siguiente linea que indica la frecuencia de operación del micro:

```
#define BSP_CLOCK			12000000 /*Varia el valor de esta definición acorde a la frecuencia de tu aplicación*/
```
Si no se define esta linea de código, la velocidad que usara el driver por defaul sera de 12MHz.

####Ejemplo de uso
Simple sampleo de un canal de adc a 10 bits de resolución
```
#include <p18cxxx.h>
#include "vectors.h"
#include "types.h"
#include "adc/adc.h"

#pragma code
void main(void)
{
    _U16 valor_adc; /*variable que almacenara el resultado del adc (canal 3)*/

    ANCON0 = 0XFF;  /*Desativamos las salidas analogicas*/
    ANCON1 = 0XFF;  /*Desativamos las salidas analogicas*/

    Adc_Init(ADC_10BITS);           /*se incializa el adc con resolución de 10bits*/

    while (1)
    {
        valor_adc = Adc_u16Read(3);       /*Se lee la lectura del canal 3 en el adc*/
		/*se tiene la medición del adc en la variable "valor_adc"*/
    }
}

```

####Configuración
No se requiere configurar este modulo mas a allá de indicar la frecuencia de operación del CPU, el tiempo de adquisición se adapta ala frecuencia y el tiempo de conversión esta fijo a 12TAD.

####API
```
	/*-- Defines --*/
	/**---------------------------------------------------------------------------------------------
      \def        ADC_8BITS
      \brief      Configura el modulo adc para conversiones de 8 bits
    ----------------------------------------------------------------------------------------------*/
    #define ADC_8BITS               1
    
    /**---------------------------------------------------------------------------------------------
      \def        ADC_10BITS
      \brief      Configura el modulo adc para conversiones de 10 bits
    ----------------------------------------------------------------------------------------------*/
    #define ADC_10BITS              2


    /*-- Functions --*/
    /**---------------------------------------------------------------------------------------------    
      \brief      Enciende el modulo ADC a la resolución indicada 
      \param      u8Flags.- Selecciona resolución entre ADC_8BITS y ADC_10BITS
      \return     None 
      \warning    None    
    ----------------------------------------------------------------------------------------------*/
    void Adc_Init(_U08 u8Flags);
    
	/**---------------------------------------------------------------------------------------------    
      \brief      Lee un canal de adc 
      \param      u8Channel.- Canal de adc a leer
      \return     Lectura del adc  
      \warning    Esta función traba al cpu hasta que se termina de samplear y convertir una lectura    
    ----------------------------------------------------------------------------------------------*/
    _U16 Adc_u16Read(_U08 u8Channel);
```

####Ejemplos
Descomprime estos ejemplos en el mismo directorio donde tengas tu banco de código.

- [Ejemplo 1: lectura simple de una canal de adc][1]
- [Ejemplo 2: lectura múltiple de diferentes canales de adc][2]


  [1]: http://www.hotboards.org/images/codigo/8bits/examples/adc1.zip
  [2]: http://www.hotboards.org/images/codigo/8bits/examples/adc2.zip