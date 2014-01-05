Entradas de propósito general (gpis)
-----------------------------------
--------

Esta pieza de código maneja la complejidad detrás de la activación de interruptores mecánicos, toma en cuenta la eliminación de rebotes, detección de flancos de subida, tiempo de presionado y el estado actual de cada botón.

La detección de activaciones se realiza mediante poleo y puede configurarse para que detecte activaciones en niveles ato (1) o bajo (0). El driver puede activar resistencias internas de pullup en los uC que disponen de ellas.

El código es dependiente de **types.h**, **gpios/gpios.h** y **hardware_profile.h**


####Ejemplo de uso:
El siguiente código muestra como se polea las entradas cada 10ms y se espera que se active un botón a los 50ms, pues el valor de debounce por defaul es de 5 y la función Gpis_Task se polea cada 10ms. Se utiliza el canal 0 de los swtimers para determinar el tiempo exacto que se debe polear las entradas. 
```
#include "vectors.h"
#include "types.h"
#include "swtimers/swtimers.h"
#include "gpis/gpis.h"

#pragma code
void main(void)
{
    Gpis_Init();  	                 /*inicializa las entradas a polear*/
    Timers_Init();					 /*inicializa los timers de software*/
    Timers_InterruptPriority(_HIGH); /*prioridad alta a la interrupción de los timers*/
    Timers_SetTime(0, 10/timers_ms); /*se carga el canal 0 con 10ms*/
    __ENABLE_INTERRUPTS();           /*se activan las interrupciones globales*/
    while (1)
    {
        if(Timers_u16GetTime(0) == 0)/*se pregunta si el canal 0 llego a 0*/
        {
            Timers_SetTime(0, 10/timers_ms);/*se recarga de nuevo con 10ms*/
            Gpis_Task();					/*se polea las entradas*/
        }

        if(Gpis_bGetInput(0) == _TRUE) /*se pregunta si se activo la entrada 0*/
        {	/*esto debe ocurrir cada 50ms pues su valor de debounce es de 5*/
			/*se activo la entrada 0, haremos cualquier cosa que queramos hacer*/
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


####Configuracion
Antes de utilizar este código se debe realizar algunas configuraciones dentro del archivo **hardware_profile.h**. Se debe indicar el numero de entradas que la aplicación utilizara

```
#define GPIS_N_INPUTS           1 /*su valor por defaul es cero*/
```

Se debe indicar a que pines esta conectado cada entrada

```
#define GPIS_B0_P               GPIOS_PORTA /*puerto*/
#define GPIS_B0_B               0			/*pin*/
/*en caso de utilizarse mas entradas se debe repetir */
#define GPIS_Bx_P               GPIOS_PORTD //puerto
#define GPIS_Bx_B               3			//pin
/* donde (x) es la siguiente entrada a asignar, el numero de entradas declaradas 
debe coincidir con el valor de GPIS_N_INPUTS, y el ultimo valor de (x) deberá 
ser GPIS_N_INPUTS-1 */
```

Existen otros parámetros los cuales son opcionales, en caso de querer variar sus valores solo habrá que definirnos en **hardware_profile.h**, de no hacerse esto, el driver tomara sus valores por defaul.

```
#define GPIS_ON                         0/*nivel de activación bajo (0) o alto(1)*/
#define GPIS_DEBOUNCE                   5/*debounce por defaul, la cifra mínima permitida es de 3*/
#define GPIS_PULLUPS                    0/*pullups activas (1) o desactivadas (0)*/
```


####API
```
    /*-- Functions --*/
    /**---------------------------------------------------------------------------------------------
      \brief      Inicializa los pines de entradas en los que estarán conectados los interruptores
      \param	  None
      \return     None
      \warning	  None
    ----------------------------------------------------------------------------------------------*/
    void Gpis_Init(void);

    /**---------------------------------------------------------------------------------------------
      \brief      Lee el estado de los pines de entrada, los procesa para eliminar rebotes y detecta
				  flancos de subida (momentos de activación) 
      \param	  None
      \return     None
      \warning	  Esta función debe ser llamada de manera periódica, el tiempo recomendad es de 10ms
    ----------------------------------------------------------------------------------------------*/
    void Gpis_Task(void);

    /**---------------------------------------------------------------------------------------------
      \brief      Habilita o deshabilita una entrada mecánica (botón, interruptor, etc.)
      \param	  u8Gpi.- Entrada seleccionada (de 0 hasta GPIS_N_INPUTS-1)
      \param      bStatus.- 1 para activar 0 para desactivar
      \return     None
      \warning	  Por defaul la función Gpis_Init habilita todas las entradas
    ----------------------------------------------------------------------------------------------*/
    void Gpis_Enable(_U08 u8Gpi, _BOOL bStatus);
    
    /**---------------------------------------------------------------------------------------------
      \brief      Establece un nuevo valor de debounce
      \param	  u8Gpi.- Entrada seleccionada (de 0 hasta GPIS_N_INPUTS-1)
      \param      u8Debounce.- El numero de ejecuciones de Gpis_Task necesarias para dar una activación
      \return     None
      \warning	  El valor mínimo de debounce es de 3
    ----------------------------------------------------------------------------------------------*/
    void Gpis_Debounce(_U08 u8Gpi, _U08 u8Debounce);

    /**---------------------------------------------------------------------------------------------
      \brief      Pregunta si una entrada a tenido una activación
      \param	  u8Gpi.- Entrada seleccionada (de 0 hasta GPIS_N_INPUTS-1)
      \return     '1' si la entrada ha sido activada
      \warning	  La función limpia internamente la bandera de activación cuando esta regresa un '1'
    ----------------------------------------------------------------------------------------------*/
    _BOOL Gpis_bGetInput(_U08 u8Gpi);

    /**---------------------------------------------------------------------------------------------
      \brief      Regresa el estado actual en el que se encuentra la entrada
      \param	  u8Gpi.- Entrada seleccionada (de 0 hasta GPIS_N_INPUTS-1)
      \return     '1' si la entrada esta activa ("presionado") o '0' si esta en estado no activo
      \warning	  None
    ----------------------------------------------------------------------------------------------*/
    _BOOL Gpis_bGetStatus(_U08 u8Gpi);

```

####Ejemplos 

- [Ejemplo 1: Invertir el estado de un led cada que se presiona un botón][1]
- [Ejemplo 2: Aumentar el tiempo que se debe presionar un botón][2]
- [Ejemplo 3: Apagar y prender un led según el estado de un botón/interruptor][3]
- [Ejemplo 4: Invertir el estado de 3 leds acorde a la activación de 3 botones][4]


  [1]: http://www.hotboards.org/images/codigo/8bits/examples/gpis1.zip
  [2]: http://www.hotboards.org/images/codigo/8bits/examples/gpis2.zip
  [3]: http://www.hotboards.org/images/codigo/8bits/examples/gpis3.zip
  [4]: http://www.hotboards.org/images/codigo/8bits/examples/gpis4.zip

