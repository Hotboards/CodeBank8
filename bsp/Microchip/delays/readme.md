Retardos por software (Delays)
------------------------------
-----------
Código para generar retardos por software sin utilizar ningún periférico del microcontrolador. Los retardos los realiza mediante decrementos acorde a la frecuencia de operación del CPU. Se debe tomar en cuenta que el uso de interrupciones puede afectar la duración de los retardos.

El driver gpios tiene dependencia directa del archivo **types.h**.

Es necesario configurar esta pieza de código antes de utilizarse. en el archivo **bsp_profile.h** se debe agregar la siguiente linea que indica la frecuencia de operación del micro:

```
#define BSP_CLOCK			12000000 /*Varia el valor de esta definición acorde a la frecuencia de tu aplicación*/
```

Si no se define esta linea de código, la velocidad que usara el driver por defaul sera de 12MHz.

####Ejemplos de uso:
> Parpadeando un led cada 100ms, combinado con el driver gpios
```
Gpios_PinDirection(GPIOS_PORTA, 1, GPIOS_OUTPUT); /*Pin RA1 como salida*/
for(;;)
{
    Gpios_TogglePin(GPIOS_PORTA, 1); /*invierto el estado del pin RA1*/
    Delays_ms(100);                  /*ciclo al CPU por 100ms*/
}
```

> Rotando un led a la izquierda cada 100ms, combinado con el driver gpios y el archivo types.h
```
_U08 var = 1;                           /*declaro variable que contendrá el valor a rotar*/
Gpios_WriteTris(GPIOS_PORTB, 0x00);     /*declaro puerto B como salida*/
for(;;)
{
    Gpios_WritePort(GPIOS_PORTB, var);  /*escribo el valor de var en el puerto B*/
    Delays_ms(100);                     /*ciclo al CPU por 100ms*/
    LEFT_8SHIFT(var,1);                 /*recorro un bit a la izquierda el valor de var*/
    if(var == 0)var = 1;Delays_ms(100); /*si el valor de var es cero vuelve a empezar*/
}
``` 
 