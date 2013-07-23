Manejo de puertos de propósito general (gpios)
----------------------------------------------

El driver de bajo nivel gpios agrupa una colección de funciones que permiten manejar los puertos de una manera más manejable y portable. Mediante las funciones descritas en el archivo **gpios.h**, podrás establecer la dirección, escribir y/o leer, ya sea el puerto entero o pin a pin.

El driver gpios tiene dependencia directa del archivo **types.h.**

####Ejemplos de uso:

> Inicio de dirección de un puerto como salida y escritura del mismo.

```
Gpios_PinDirection(GPIOS_PORTA, 2, GPIOS_OUTPUT); 	/*pin RA2 como salida*/
Gpios_WritePin(GPIOS_PORTA, 2, 1); 					/*enciendo pin RA2*/
Gpios_WritePin(GPIOS_PORTA, 2, 0); 					/*apago pin RA2*/
```

> Lectura de un pin de entrada y toggleo de un pin de salida.

```
Gpios_PinDirection(GPIOS_PORTA, 5, GPIOS_OUTPUT); 	/*pin RA5 como salida*/
Gpios_PinDirection(GPIOS_PORTA, 3, GPIOS_INTPUT); 	/*pin RA3 como entrada*/
 
if(Gpios_bReadPin(GPIOS_PORTA, 3)==1)				/*si se lee un 1 en el pin RA3*/
{
    Gpios_TogglePin(GPIOS_PORTA, 5); 				/*invierto el estado del pin RA5*/
}
```

> Lectura de un puerto completo.

```
_U08 valor;
Gpios_WriteTris(GPIOS_PORTA, 0xFF); 	/*puerto A como entrada*/
valor = Gpios_u8ReadPort(GPIOS_PORTA); 	/*asignamos el valor del puerto a la variable valor*/
```

> Lectura de un puerto y escritura en otro puerto

```
_U08 valor;
Gpios_WriteTris(GPIOS_PORTD, 0xFF); 	/*puerto D como entrada*/
Gpios_WriteTris(GPIOS_PORTB, 0x00); 	/*puerto B como salida*/

valor = Gpios_u8ReadPort(GPIOS_PORTD); 	/*asignamos el valor del puerto a la variable valor*/
Gpios_WritePort(GPIOS_PORTB, valor);	/*escrivimos el valor de valor en el puerto B*/
``` 