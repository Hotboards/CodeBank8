Manejo de puertos de propósito general (gpios)
----------------------------------------------

El driver de bajo nivel gpios agrupa una colección de funciones que permiten manejar los puertos de una manera más manejable y portable. Mediante las funciones descritas en el archivo **gpios.h**, podrás establecer la dirección, escribir y/o leer, ya sea el puerto entero o pin a pin.

El driver gpios tiene dependencia directa del archivo **types.h.**

####Ejemplos de uso:

Inicio de dirección de un puerto como salida y escritura del mismo.

```
Gpios_PinDirection(GPIOS_PORTA, 2, GPIOS_OUTPUT); 	/*pin RA2 como salida*/
Gpios_WritePin(GPIOS_PORTA, 2, 1); 					/*enciendo pin RA2*/
Gpios_WritePin(GPIOS_PORTA, 2, 0); 					/*apago pin RA2*/
```

Lectura de un pin de entrada y toggleo de un pin de salida.

```
Gpios_PinDirection(GPIOS_PORTA, 5, GPIOS_OUTPUT); 	/*pin RA5 como salida*/
Gpios_PinDirection(GPIOS_PORTA, 3, GPIOS_INTPUT); 	/*pin RA3 como entrada*/
 
if(Gpios_bReadPin(GPIOS_PORTA, 3)==1)				/*si se lee un 1 en el pin RA3*/
{
    Gpios_TogglePin(GPIOS_PORTA, 5); 				/*invierto el estado del pin RA5*/
}
```

Lectura de un puerto completo.

```
_U08 valor;
Gpios_WriteTris(GPIOS_PORTA, 0xFF); 	/*puerto A como entrada*/
valor = Gpios_u8ReadPort(GPIOS_PORTA); 	/*asignamos el valor del puerto a la variable valor*/
```

Lectura de un puerto y escritura en otro puerto

```
_U08 valor;
Gpios_WriteTris(GPIOS_PORTD, 0xFF); 	/*puerto D como entrada*/
Gpios_WriteTris(GPIOS_PORTB, 0x00); 	/*puerto B como salida*/

valor = Gpios_u8ReadPort(GPIOS_PORTD); 	/*asignamos el valor del puerto a la variable valor*/
Gpios_WritePort(GPIOS_PORTB, valor);	/*escribimos el valor de valor en el puerto B*/
``` 

####API
```
	/*----------------------------------------------------------------------------------------------
  	\def		GPIOS_INPUT/OUTPUT
 	\brief	constantes para definir la dirección en la que operaran los pines de los puertos 
	----------------------------------------------------------------------------------------------*/
	#define GPIOS_OUTPUT		0
	#define GPIOS_INTPUT		1

	/**--------------------------------------------------------------------------------------------- 
      \typedef  _eGPIOS_PORT
      \brief    Enumeraciones usadas para identificar el puerto que se quiere manipular                        
    ----------------------------------------------------------------------------------------------*/     
    typedef enum 
    {
        GPIOS_PORTA = 0, 
        GPIOS_PORTB, 
        GPIOS_PORTC,
        GPIOS_PORTD,
        GPIOS_PORTE,
        GPIOS_PORTF,
        GPIOS_PORTG
    }_eGPIOS_PORT;
    
    
    /*-- Functions --*/
    /**---------------------------------------------------------------------------------------------    
      \brief      Iniaciliza un pin del uC a la vez 
      \param      ePort: Puerto donde se encuentra el pin (GPIOS_PORTx)
      \param      u8Pin:  Numero de pin (de 0 to 7)
      \param      bDirection: Modo entrada (GPIOS_INPUT) o salida (GPIOS_OUTPUT)
      \return     None
      \warning    None    
    ----------------------------------------------------------------------------------------------*/
    void Gpios_PinDirection(_eGPIOS_PORT ePort, _U08 u8Pin, _BOOL bDirection);

    /**---------------------------------------------------------------------------------------------    
      \brief      funcion usada para escribir valores en un pin del uC  
      \param      ePort: Puerto en el que se encuentra el pin (GPIOS_PORTx)
      \param      u8Pin: Numero de pin (de 0 a 7)    
      \param      bValue: valor de salida (0 o 1)    
      \return     None
      \warning    Se debe inicializar el pin como salida antes de usar esta función      
    ----------------------------------------------------------------------------------------------*/
    void Gpios_WritePin(_eGPIOS_PORT ePort, _U08 u8Pin, _BOOL bValue);
 
    /**---------------------------------------------------------------------------------------------    
      \brief      Funcion usada para activar las pullups internas del uC 
      \param      ePort: Puerto en el que se encuentra el pin (GPIOS_PORTx)
      \param      u8Pin: Numero de pin (de 0 a 7)    
      \param      bValue: '1' to activar pullup 0 para desactivar    
      \return     None
      \warning    Esta opción no existe en los PIC18F y solo existe por compatibilidad     
    ----------------------------------------------------------------------------------------------*/
    void Gpios_PullupPin(_eGPIOS_PORT ePort, _U08 u8Pin, _BOOL bValue);
 
    /**---------------------------------------------------------------------------------------------    
      \brief      Función usada para invertir el estado de un solo Pin.
      \param      ePort: Puerto en el que se encuentra el pin (GPIOS_PORTx)
      \param      u8Pin: Numero de pin (de 0 a 7)    
      \return     None
      \warning    Se debe inicializar el pin como salida antes de usar esta función   
    ----------------------------------------------------------------------------------------------*/  
    void Gpios_TogglePin(_eGPIOS_PORT ePort, _U08 u8Pin);

    /**---------------------------------------------------------------------------------------------
      \brief      Función usada para leer el valor en un pin.
      \param      ePort: Puerto en el que se encuentra el pin (GPIOS_PORTx)
      \param      u8Pin: Numero de pin (de 0 a 7)   
      \return     la función solo regresa valores de 1 o 0 
      \warning    Se debe inicializar el pin como entrada antes de usar esta función        
    ----------------------------------------------------------------------------------------------*/  
    _BOOL Gpios_bReadPin(_eGPIOS_PORT ePort, _U08 u8Pin);
    
    /**---------------------------------------------------------------------------------------------
      \brief      Función usada para escribir completamente en un puerto
      \param      ePort: puerto a escribir (GPIOS_PORTx)
      \param      u8Value: valor a escribir (de 0 a 255)      
      \return     None
      \warning    se debe inicializar el puerto completo como salida
    ----------------------------------------------------------------------------------------------*/
    void Gpios_WritePort(_eGPIOS_PORT ePort, _U08 u8Value);

    /**---------------------------------------------------------------------------------------------
      \brief      Función usada para leer un puerto completo
      \param      ePort: puerto a leer (GPIOS_PORTx) 
      \return     Valor leído del puerto 
      \warning    None
    ----------------------------------------------------------------------------------------------*/
    _U08 Gpios_u8ReadPort(_eGPIOS_PORT ePort);
    
    /**---------------------------------------------------------------------------------------------    
      \brief      Función usada para escribir la dirección de los pines del puerto
      \param      ePort: Nombre del puerto a configurar 
      \param      u8Value: Valor a cargar al registro TRISx
      \return     None
      \warning    None      
    ----------------------------------------------------------------------------------------------*/
    void Gpios_WriteTris(_eGPIOS_PORT ePort, _U08 u8Value);

    /**---------------------------------------------------------------------------------------------
      \brief      Funcion usada para leer la configuración de un puerto completo
      \param      ePort: Nombre del puerto
      \return     Valor interno del registro                   
      \warning    None
    ----------------------------------------------------------------------------------------------*/
    _U08 Gpios_u8ReadTris(_eGPIOS_PORT ePort);
    
    /**---------------------------------------------------------------------------------------------
      \brief      Funcion usada para activar las resistencias de pullup de un puerto completo
      \param      ePort: Nombre del puerto
      \return     None                    
      \warning    Esta opción no existe en los PIC18F y solo existe por compatibilidad 
    ----------------------------------------------------------------------------------------------*/
    void Gpios_SetPullups(_eGPIOS_PORT ePort, _U08 u8Value);
    
    /**---------------------------------------------------------------------------------------------
      \brief      Funcion usada para leer las resistencias pullup activas de un puerto
      \param      ePort: Nombre del puerto 
      \return     Resistencias e pullup activas                     
      \warning    Esta opción no existe en los PIC18F y solo existe por compatibilidad 
    ----------------------------------------------------------------------------------------------*/
    _U08 Gpios_u8ReadPullups(_eGPIOS_PORT ePort);

```
####Ejemplos
Descomprime estos ejemplos en el mismo directorio donde tengas tu banco de código.

- [Ejemplo 1: Parpadeo de un led][1]
- [Ejemplo 2: Escritura y lectura de un puerto][2]
- [Ejemplo 3: Rotación de leds en un puerto][3]


  [1]: https://github.com/Hotboards/Examples/blob/master/Microchip/gpios1.X/main.c
  [2]: https://github.com/Hotboards/Examples/blob/master/Microchip/gpios2.X/main.c
  [3]: https://github.com/Hotboards/Examples/blob/master/Microchip/gpios3.X/main.c

