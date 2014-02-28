Teclado matricial (keyboard)
============================


Este código permite manejar un teclado matricial de n x m, es decir la cantidad de lineas y columnas son configurables de 1 a 8, lo cual permite manejar un máximo de 64 botones en el teclado. El driver se utiliza únicamente por poleo y es necesario mandar llamar una función periódica cada x tiempo. Ademas es configurable por el usuario el tiempo de debounce y los pines a los cuales se puede conectar.

El teclado solo funciona con resistencias de pullups externas en las columnas y de momento no permite presionar dos teclas al mismo tiempo, sin embargo es posible que mediante las funciones del driver la aplicación pueda implementar algoritmos tipo "Ctrl + <tecla>"

El código es dependiente de **types.h**, **gpios/gpios.h** y **hardware_profile.h**

Ejemplo de uso
--------------

Simple poleo del teclado matricial cada 10ms con un debounce por default de 5, por lo que la activación de cada tecla se realiza a los 50ms de haberla presionado
```C
#include "vectors.h"
#include "types.h"
#include "swtimers/swtimers.h"
#include "keyboard/keyboard.h"
#include "system/system.h"

#pragma code
void main(void)
{
	_U08 u8Key;
    Keyboard_Init();                 /*inicializa las entradas a polear*/
    Timers_Init();                   /*inicializa los timers de software*/
    Timers_InterruptPriority(_HIGH); /*prioridad alta a la interrupción de los timers*/
    Timers_SetTime(0, 10/timers_ms); /*se carga el canal 0 con 10ms*/
    __ENABLE_INTERRUPTS();           /*se activan las interrupciones globales*/
    while (1)
    {
        if(Timers_u16GetTime(0) == 0)/*se pregunta si el canal 0 llego a 0*/
        {
            Timers_SetTime(0, 10/timers_ms);/*se recarga de nuevo con 10ms*/
            Keyboard_Task();                    /*se polea las entradas*/
        }

        if(Gpis_bIsPressed() == _TRUE) /*se pregunta si se activo la entrada 0*/
        {
		   	u8Key = Keyboard_u8GetKey();/*esto debe ocurrir cada 50ms pues su valor de debounce es de 5*/
            /*si se activo la tecla 2 por ejemplo, haremos cualquier cosa que queramos hacer*/
            /*como togglear un led, activar un motor, etc..*/   
        }
    }
}

#pragma interrupt YourHighPriorityISRCode
void YourHighPriorityISRCode(void)
{
    Timers_Isr();/*la función se debe llamar en este vector por ser de prioridad alta*/
}
```

Configuración
-------------

Antes de utilizar este código se debe realizar algunas configuraciones dentro del archivo **hardware_profile.h** Se debe indicar el numero de filas que la aplicación utilizara

```C
#define KEYBOARD_ROWS        2/*su valor por default es cero*/
```

Se debe indicar a que pines esta conectado a cada fila

```C
#define KEYBOARD_R0_P        GPIOS_PORTD
#define KEYBOARD_R0_B        3

#define KEYBOARD_R1_P        GPIOS_PORTD
#define KEYBOARD_R1_B        2
/*en caso de utilizarse mas entradas se debe repetir*/ 
#define KEYBOARD_Rx_P        GPIOS_PORTD //puerto
#define KEYBOARD_Rx_B        1            //pin
/* donde (x) es la siguiente entrada a asignar, el numero de entradas declaradas 
debe coincidir con el valor de KEYBOARD_ROWS, y el ultimo valor de (x) deberá 
ser KEYBOARD_ROWS */
```

De la misma forma se debe indicar el numero de columnas que la aplicación utilizara

```C
#define KEYBOARD_COLUMNS     2/*su valor por default es cero*/
```

Se debe indicar a que pines esta conectado cada entrada

```C
#define KEYBOARD_C0_P        GPIOS_PORTC
#define KEYBOARD_C0_B        2

#define KEYBOARD_C1_P        GPIOS_PORTC
#define KEYBOARD_C1_B        1
/*en caso de utilizarse mas entradas se debe repetir*/ 
#define KEYBOARD_Cx_P        GPIOS_PORTC //puerto
#define KEYBOARD_Cx_B        0            //pin
/* donde (x) es la siguiente entrada a asignar, el numero de entradas declaradas 
debe coincidir con el valor de KEYBOARD_COLUMNS, y el ultimo valor de (x) deberá 
ser KEYBOARD_COLUMNS */
```

Existen otros parámetros los cuales son opcionales, en caso de querer variar sus valores solo habrá que definirnos en **hardware_profile.h**, de no hacerse esto, el driver tomara sus valores por defaul.

```C
#define KEYBOARD_DEBOUNCE                   5/*debounce por default, la cifra mínima permitida es de 3*/
#define KEYBOARD_PULLUPS                    0/*pullups activas (1) o desactivadas (0)*/
```

API
---

```
	/*-- Functions --*/
    /**---------------------------------------------------------------------------------------------    
      \brief      Inicializa los pines conectados el teclado como entradas y salidas
      \param	  None
      \return     None
      \warning	  None   	
    ----------------------------------------------------------------------------------------------*/
    void Keyboard_Init(void);
    
    /**---------------------------------------------------------------------------------------------    
      \brief      Realiza un barrido para verificar si existe una tecla presionada
      \param	  None
      \return     None
      \warning	  Esta función se debe mandar llamar de manera periódica   	
    ----------------------------------------------------------------------------------------------*/
    void Keyboard_Task(void);
    
	/**---------------------------------------------------------------------------------------------    
      \brief      Establece un nuevo tiempo de debounce para una tecla en especifico
      \param	  u8Key.- tecla a la cual modificar su tiempo de debounce
      \param	  u8Debounce.- Nuevo valor de debounce
      \return     None
      \warning	  None  	
    ----------------------------------------------------------------------------------------------*/
    void Keyboard_SetDebounce(_U08 u8Key, _U08 u8Debounce);
    
    /**---------------------------------------------------------------------------------------------    
      \brief      Función que permite conocer si se presiono una tecla
      \param	  None
      \return     1: si se presiono una tecla (cualquiera)
      \warning	  La función limpia internamente una bandera cunado regresa una valor de uno   	
    ----------------------------------------------------------------------------------------------*/
    _BOOL Keyboard_bIsPressed(void);
    
    /**---------------------------------------------------------------------------------------------    
      \brief      Función que regresa la ultima tecla presionada
      \param	  None
      \return     Ultima tecla presionada de 0 a KEYBOARD_N_KEYS - 1 
      \warning	  Esta función se debe llamar si y solo si la función Keyboard_bIsPressed() == 1u   	
    ----------------------------------------------------------------------------------------------*/
    _U08 Keyboard_u8GetKey(void);

    /**---------------------------------------------------------------------------------------------    
      \brief      Función que regresa el estado de la tecla seleccionada
      \param	  u8Key.- Tecla a interrogar
      \return     Estado de la tecla 1: presionada 
      \warning	  Esta función no modifica el estado de la bandera de activación  	
    ----------------------------------------------------------------------------------------------*/
    _BOOL Keyboard_u8GetStatus(_U08 u8Key);
```

Ejemplos
--------

- [Ejemplo 1: Simple lectura de teclado y despliegue con leds][1]
- [Ejemplo 2: Teclas con diferente tiempo de presionado][2]


  [1]: https://github.com/Hotboards/Examples/blob/master/Microchip/keyboard1.X/main.c
  [2]: https://github.com/Hotboards/Examples/blob/master/Microchip/keyboard2.X/main.c
