Salidas de propósito general (gpos)
----------------------------------
---------

El driver gpos permite tener en los pines digitales del micro en estados de salidas de propósito general, que pueden configurarse de manera que oscilen continuamente, se mantengan encendidas o apagadas por cierto tiempo o totalmente apagadas o encendidas sin cambiar nunca, este driver no utiliza ningún hardware del uC y solo es eficiente con periodos de tiempo mayores a 1ms.

Este codigo es dependiente de **types.h**, **gpios/gpios.h** y **hardware_profile.h**

####Ejemplos de uso:

Programa que mantiene oscilando cada segundo una salida seleccionada en nuestro uC, para seleccionar el pin que invertirá su estado ver la sección **Configuración**

```
#include "vectors.h"
#include "types.h"
#include "swtimers/swtimers.h"
#include "gpos/gpos.h"

void main(void)
{
	Gpos_Init()  						  /*inicializo mis salidas*/
	Timers_Init(); 						  /*inicializo timers*/
	Timers_SetTime(0, 10/timers_ms); 	  /*cargo timer para poleo de salidas*/
	Gpos_SetMode(1, GPOS_OSC_MODE,  100); /*canal uno oscilara cada 1s*/
	__INTERRUPT_ENABLE(); 				  /*activo interrupciones*/
	while(1)
	{
   		if(Timers_u16GetTime(0) == 0)	  /* llego a 0 el primer canal*/
   		{
        	Timers_SetTime(0, 10/timers_ms);/*se cumplen los 10ms volvemos a recargar el*/
        	Gpos_Task();            		/*actualizamos el estado de los pines*/
		}
   	}
}

#pragma interrupt YourHighPriorityISRCode
void YourHighPriorityISRCode(void) /*vector de prioridad alta*/
{
    Timers_Isr();/*la función se debe llamar en este vector por ser de prioridad alta*/
}
```

####Configuración
En el archivo **hardware_profile.h** se debe indicar el numero de salidas a usar

```
#define GPOS_N_PINS					1 /*numero de salidas a usarse*/
```

Una vez indicadas el numero de salidas a usar, se debe definir los pines a los que estarán asignadas esas salidas

```
#define GPOS_P0_P					GPIOS_PORTA /* port */
#define GPOS_P0_B                  	1           /* pin */
/*en caso de utilizarse mas entradas se debe repetir */ 
#define GPOS_Px_P            	    GPIOS_PORTD //puerto
#define GPOS_Px_B               	3			//pin
/* donde (x) es la siguiente salida a asignar, el numero de salidas declaradas 
debe coincidir con el valor de GPOS_N_PINS, y el ultimo valor de (x) deberá 
ser GPOS_N_PINS-1 */
``` 

####API
```
	/**---------------------------------------------------------------------------------------------
      \def      _eGPOS_MODES
      \brief    Estos son los diferentes modos en los que pueden operar las salidas
    ----------------------------------------------------------------------------------------------*/
    typedef enum
    {
        GPOS_OSC_MODE = 0u,             /*|-|_|-|_|-|_|-|_|  siempre oscilando*/
        GPOS_HIGH_MODE,                 /*-----------------  siempre en alto*/
        GPOS_LOW_MODE,                  /*_________________  siempre en bajo*/
        GPOS_DELAYHIGH_MODE,            /*________|--------  un momento en alto*/
        GPOS_DELAYLOW_MODE              /*--------|________  un momento en bajo*/
    }_eGPOS_MODES;
    
    
    /*-- Functions --*/
    /**---------------------------------------------------------------------------------------------
      \brief      Inicializa las salidas seleccionadas
      \param	  None
      \return     None
      \warning	  None
    ----------------------------------------------------------------------------------------------*/
    void Gpos_Init(void);

    /**---------------------------------------------------------------------------------------------
      \brief      Actualiza el estado de las salidas con cada ejecución
      \param	  None
      \return     None
      \warning	  Esta función debe ser ejecutada de manera periódica, se recomienda cada 10ms
    ----------------------------------------------------------------------------------------------*/
    void Gpos_Task(void);

    /**---------------------------------------------------------------------------------------------
      \brief      Establece el modo de operación la salida seleccionada 
      \param	  u8Gpo.- Pin seleccionado
      \param	  u8Status.- Modo de operación de la salida
      \param	  u8Counter.- Numero de ejecuciones de Gpos_Task en los que permanecera un estado el pin
      \return     None
      \warning	  None
    ----------------------------------------------------------------------------------------------*/
    void Gpos_SetMode(_U08 u8Gpo, _eGPOS_MODES eModes, _U08 u8Counter);
```
####Ejemplos 


- [Ejemplo 1: Invertir el estado de un led cada 200ms][1]
- [Ejemplo 2: Encender un led por 300ms cada que se presione un botón][2]
- [Ejemplo 3: Parpadeo un led a 50ms por un lapso de 1s][3]
- [Ejemplo 4: Parpadeo de 4 leds a diferentes timepos][4]


  [1]: https://github.com/Hotboards/Examples/blob/master/Microchip/gpos1.X/main.c
  [2]: https://github.com/Hotboards/Examples/blob/master/Microchip/gpos2.X/main.c
  [3]: https://github.com/Hotboards/Examples/blob/master/Microchip/gpos3.X/main.c
  [4]: https://github.com/Hotboards/Examples/blob/master/Microchip/gpos4.X/main.c
 