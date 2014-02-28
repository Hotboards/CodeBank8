Timers por software (swtimers)
==============================


El driver swtimers permite generar períodos de tiempo sin la necesidad de ciclar el CPU haciendo nada. Mediante el uso del **TIMER0** del PIC18F se puede multiplicar el número de canales que representará un periodo de tiempo diferente. De esta manera se pueden ejecutar diferentes tareas en el microcontrolador a diferente tiempo sin  causar un overhead excesivo en el CPU.

El driver swtimers tiene dependencia directa del archivo **types.h**.

Es necesario configurar esta pieza de código antes de utilizarse. en el archivo **hardware_profile.h** se debe agregar la siguiente linea que indica la frecuencia de operación del micro:

```C
#define BSP_CLOCK           12000000 /*Varia el valor de esta definición acorde a la frecuencia de tu aplicación*/
```

Si no se define esta linea de código, la velocidad que usara el driver por defaul sera de 12MHz.

Ejemplo de uso
--------------

Generacion de un lapso de tiempo de 100ms
```C
#include "vectors.h"
#include "types.h"
#include "swtimers.h"
#include "system.h"

void main(void)
{
	Timers_Init();        /*inicializamos el driver, se generara una interrupción cada 5ms por default*/
	Timers_InterruptPriority(_HIGH);    /*colocamos la interrupción en alta prioridad*/
	Timers_SetTime(1, (100/timer_ms));  /*cargamos el canal 0 con un valor de 100ms*/
	
    __ENABLE_INTERRUPTS();  /*se debe habilitar las interrupciones globales con prioridad*/
	
	for(;;)
	{
		if(Timers_u16GetTime(1) == 0)/*preguntamos si el canal 1 llegó a cero*/
 		{
        	Timers_SetTime(1, (100/timer_ms));/*recargamos el canal 1 con el mismo tiempo */          
        	/*ejecuto cualquier cosa que se deba ejecutar cada 100ms*/
 		}
	}
}
 
#pragma interrupt YourHighPriorityISRCode
void YourHighPriorityISRCode(void) /*vector de prioridad alta*/
{
    Timers_Isr();/*la función se debe llamar en este vector por ser de prioridad alta*/
}
```

Configuración
-------------

El driver puede configurarse para establecer diferentes bases de tiempo y ajustarse a las necesidades de la aplicación, puede ser mas eficiente o mas flexible.
En el archivo **hardware_profile.h** se pueden definir las siguientes constantes:
```C
#define TIMERS_BASE_TIME        5 /*Base de tiempo, el valor deberá ser en ms (valor por default)*/
#define TIMERS_PREESCALER       8 /*Valor de preescaler para la base de tiempo del TMR0 (valor por default)*/
#define TIMERS_N_CHANNELS       9 /*Numero de canales (valor por default)*/
```
Si no se definen estas constantes se tomaran sus valores por default.
Los valores combinados se conjuntan para generar la base de tiempo interna, la cual obedece a la siguiente formula:

**Warning:** se debe evitar que el resultado de la formula sea 0 o de un valor negativo
```
/*--------------------------------------------------------------------------------------
                             //            CLOCK / 4         \                    \
timer_mod_register = 65535 - ||------------------------------| * TIMERS_BASE_TIME |
                             \\ BSP_TIMERS_PREESCALER * 1000 /                    /
--------------------------------------------------------------------------------------*/
```  
 
API
---

```C
/*-- Functions --*/
/**---------------------------------------------------------------------------------------------    
  \brief      Inicializa el timer interno TMR0 para que genere una interrupción cada TIMERS_BASE_TIME
  \param	  None
  \return     None
  \warning	  None   	
----------------------------------------------------------------------------------------------*/
void Timers_Init(void);
	
/**---------------------------------------------------------------------------------------------
  \brief      Asigna prioridad a la interrupción que generara el driver
  \param	  bPriority.- valor de la prioridad _HIGH o _LOW
  \return     None
  \warning	  Por default al llamarse la función Timers_Init la interrupción tiene un valor _LOW
----------------------------------------------------------------------------------------------*/
void Timers_InterruptPriority(const _BOOL bPriority);

/**---------------------------------------------------------------------------------------------
  \brief      Regresa el valor actual del timer seleccionado
  \param	  u8Timer.- Canal a regresar su valor de tiempo, valores de 0 a TIMERS_CHANNELS-1
  \return     Valor de la cuenta actual del canal
  \warning	  None   	
----------------------------------------------------------------------------------------------*/
_U16 Timers_u16GetTime(const _U08 u8Timer);
	
/**---------------------------------------------------------------------------------------------    
  \brief      Asigna un nuevo periodo de tiempo a decrementar al canal asignado
  \param	  u8Timer.- Canal a recargar, valores de 0 a TIMERS_CHANNELS-1 
  \param	  u16Time.-  Cantidad e tiempo a decrementar, solo valores múltiplos de TIMERS_BASE_TIME
  \return     None
  \warning	  se puede convertir un valor constante a ms si se usa la constante timers_ms, siempre 
              y cuando el valor sea múltiplo de TIMERS_BASE_TIME    	
----------------------------------------------------------------------------------------------*/
void Timers_SetTime(const _U08 u8Timer, const _U16 u16Time);

/**---------------------------------------------------------------------------------------------
  \brief      Función de interrupción la cual decrementa en uno los canales activos.
  \param	  None
  \return     None
  \warning	  Esta función se debe mandar llamar en uno de los dos vectores de interrupción
----------------------------------------------------------------------------------------------*/
void Timers_Isr(void);
```

Ejemplos
--------

- [Ejemplo 1: Parpadeo de un led cada 100ms][1]
- [Ejemplo 2: Parpadeo de de 3 leds a diferentes tiempos][2]
- [Ejemplo 3: Rotación de leds en un puerto cada 500ms y parpadeo de un led en RA1 cada 100ms][3]
- [Ejemplo 4: Parpadeo de led con prioridad alta, CPU a 48MHz y base de tiempo a 10ms][4]


  [1]: https://github.com/Hotboards/Examples/blob/master/Microchip/swtimers1.X/main.c
  [2]: https://github.com/Hotboards/Examples/blob/master/Microchip/swtimers2.X/main.c
  [3]: https://github.com/Hotboards/Examples/blob/master/Microchip/swtimers3.X/main.c
  [4]: https://github.com/Hotboards/Examples/blob/master/Microchip/swtimers4.X/main.c

