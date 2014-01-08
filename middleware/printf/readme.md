Salidas y entradas formateadas (printf)
------------------------------------
------------

Las funciones printf contenidas en este archivo son ideales para uC con poca memoria y que en general no tienen los recursos necesarios que demanda una función printf típica de la librería estándar de C. El uso recomendable de estas funciones es para propósitos de depuración y pruebas de código. Debido a la cantidad de overhead que le demandan al uC y a su característica bloqueante no se recomienda usar en aplicaciones finales.

El driver no depende de ninguna salida en especifico, es deber del usuario configurar una función de salida acorde a lo que su aplicación tomara como sistema de despliegue, (puerto serial, display, file system, etc..).

**NOTA:** Estas funciones están desarrolladas por [ChaN][5], y modificadas para que cumplieran los estándares de codificación de **Hotboards**. Se recomienda visitar la pagina del autor para mayor información.


####Ejemplos de uso
Distintas formas de cadenas formateadas con salida a través del puerto uart
```
#include <p18cxxx.h>
#include "vectors.h"
#include "types.h"
#include "gpios/gpios.h"
#include "uart/uart1.h"
#include "printf/printf.h"

#pragma code
void main(void)
{
	_U08 buffer[20] = "hola";
    ANCON0 = 0XFF;  /*Desativamos las salidas analogicas*/
    ANCON1 = 0XFF;  /*Desativamos las salidas analogicas*/

    Gpios_PinDirection(GPIOS_PORTC, 6, GPIOS_OUTPUT); /*puerto de tx como salida*/

    (void)Uart1_Init(115200); /*velocidad a 115200 bauds*/
    xdev_out(Uart1_PutChar);  /*funcion Uart1_PutChar como salida*/
    
    xprintf("%d\n", (_S16)1234);                    //"1234"
    xprintf("%6d,%3d%%\n", (_S16)-200, (_S16)5);    //"  -200,  5%"
    xprintf("%-6u\n", (_S16)100);                   //"100   "
    xprintf("%ld\n", 12345678L);                    //"12345678"
    xprintf("%04x\n", (_U16)0xA3);                  //"00a3"
    xprintf("%08LX\n", (_U32)0x123ABC);             //"00123ABC"
    xprintf("%016b\n", (_U16)0x550F);               //"0101010100001111"
    xprintf("%s\n", "String");                      //"String"
    xprintf("%c\n", (_U16)'a');                     //"a"
	xprintf("%s\n", buffer);                        //No soportado
	xprintf("flotante %f", 3.14)					//No soportado

    while (1)
    {
    }
}
```


####Configuración
La única constante a configurar en el archivo **hardware_profile.h** es la definición

```
#define _CR_CRLF            1 /*Convert \n ==> \r\n in the output char*/
```
Esta definición solo inserta \r\n cuando se manda \n

Otra configuración necesaria antes de mandar datos es la de decirle al driver que salida es la que usara, esto se realiza mediante la función
```
xdev_out(Uart1_PutChar);  /*funcion Uart1_PutChar como salida*/
```
Esta función acepta otra función como parámetro, la cual se encargara de mandar un solo carácter por vez en el dispositivo de salida

####API
```
	/*-- Functions --*/
    /**---------------------------------------------------------------------------------------------    
      \brief      Establece el sistema de salida que usaran el resto de las funciones por default
      \param      func.- puntero a función que se encarga de mandar un carácter por vez a la salida
      \return     None 
      \warning    Tipicamente usas una función "putc"    
    ----------------------------------------------------------------------------------------------*/
    void xdev_out( void(*func)(const unsigned char) );

    /**---------------------------------------------------------------------------------------------    
      \brief      Manda un carácter por pantalla
      \param      c.- carácter a mandar a la salida
      \return     None 
      \warning    None    
    ----------------------------------------------------------------------------------------------*/
    void xputc(char c);
    
	/**---------------------------------------------------------------------------------------------    
      \brief      Manda una cadena de caracteres terminada en cero por pantalla
      \param      str.- puntero a cadena de caracteres
      \return     None 
      \warning    Esta función solo trabaja con cadenas almacenadas en flash    
    ----------------------------------------------------------------------------------------------*/
    void xputs(const rom char *str );
    
	/**---------------------------------------------------------------------------------------------    
      \brief      Manda una cadena de caracteres terminada en cero a través de una salida especifica
      \param      func.- salida especifica para mandar los caracteres
      \param      str.- puntero a cadena de caracteres
      \return     None 
      \warning    Esta función solo trabaja con cadenas almacenadas en flash    
    ----------------------------------------------------------------------------------------------*/
    void xfputs(void(*func)(const unsigned char), const rom char *str);
    
	/**---------------------------------------------------------------------------------------------    
      \brief      Emulación de función printf
      \param      fmt.- puntero a cadena de caracteres
	  \param      ...	parámetros opcionales
      \return     None 
      \warning    Esta función solo trabaja con cadenas almacenadas en flash    
    ----------------------------------------------------------------------------------------------*/
    void xprintf(const rom char *fmt, ...);
    
	/**---------------------------------------------------------------------------------------------    
      \brief      Almacena la cadena de salida formateada en un buffer de memoria
      \param      buff.- puntero a buffer donde se almacenara la cadena de caracteres
	  \param      fmt.- puntero a cadena de caracteres
	  \param      ...	parámetros opcionales
      \return     None 
      \warning    Esta función solo trabaja con cadenas almacenadas en flash    
    ----------------------------------------------------------------------------------------------*/
    void xsprintf(char* buff, const rom char* fmt, ...);
    
	/**---------------------------------------------------------------------------------------------    
      \brief      Similar a la función printf, solo que aquí se indica la opción de salida de caracteres
      \param      func.- función que se usara para desplegar los caracteres
	  \param      fmt.- puntero a cadena de caracteres
	  \param      ...	parámetros opcionales
      \return     None 
      \warning    Esta función solo trabaja con cadenas almacenadas en flash    
    ----------------------------------------------------------------------------------------------*/
    void xfprintf(void (*func)(const unsigned char), const rom char *fmt, ...);
    
    /**---------------------------------------------------------------------------------------------    
      \brief      Despliega el contenido de memoria apuntado
      \param      buff.- puntero al buffer que contiene los datos a desplegar
	  \param      addr.- dirección de inicio de los datos
	  \param      len.-	cantidad de datos a desplegar
      \param      width.- ancho de la palabra de los datos (bytes)
      \return     None 
      \warning    para el ultimo parámetros utilizar sizeof(char),sizeof(short) o sizeof(long)     
    ----------------------------------------------------------------------------------------------*/
    void put_dump(const void *buff, unsigned long addr, int len, int width);

```
####Ejemplos

- [Ejemplo 1: Trasnmicion de cadenas formateadas con la función xprintf][1]
- [Ejemplo 2: Transmicion de una cadena formateada con xsprintf por interrupciones][2]
- [Ejemplo 3: Printf como herramienta de depuración][3]


  [1]: https://github.com/Hotboards/Examples/blob/master/Microchip/printf1.X/main.c
  [2]: https://github.com/Hotboards/Examples/blob/master/Microchip/printf2.X/main.c
  [3]: https://github.com/Hotboards/Examples/blob/master/Microchip/printf3.X/main.c

  [5]: http://elm-chan.org/ 

 