Puerto serial asíncrono (uart)
------------------------------
-----

El driver de puerto serial maneja el periférico EUART para que realize transmisiones de 8 y 9 bits, son configurables por el usuario las opciones de paridad y numero de bits de stop.

El driver fue creado principalmente para que este funcione mediante interrupciones y no sea necesario ciclar al uC de manera innecesaria, sin embargo se crearon un par de funciones de tx de caracteres y cadenas, para cuando se requiera implementar una prueba rápida o si la aplicación no requiere demasiado procesamiento multitarea.

La función de recepción de caracteres esta implementada totalmente por interrupciones y solo se contempla una función vacía en la cual se pasa por parámetro el carácter llegado, se deja al usuario la libertad de implementar el protocolo de recepción de datos acorde a su aplicación. para realizar esto es necesario se declare un función callback.

Algunas cosas que se agregaran a futuro son:

- Modo de tx/rx de 9 bits
- Autobauding
- Detección de errores

Es necesario configurar esta pieza de código antes de utilizarse. en el archivo **hardware_profile.h** se debe agregar la siguiente linea que indica la frecuencia de operación del micro:

```
#define BSP_CLOCK           12000000/*Varia el valor de esta definición acorde a la frecuencia de tu aplicación*/
```
Si no se define esta linea de código, la velocidad que usara el driver por defaul sera de 12MHz.

####Ejemplo de uso:

Trasmitir de manera sencilla una cadena de caracteres terminada en cero y almacenada en memoria flash

```
#include <p18cxxx.h>
#include "vectors.h"
#include "types.h"
#include "uart/uart1.h"
#include "delays/delays.h"
#include "gpios/gpios.h"

#pragma code
void main(void)
{
    _U32 baudrate;
    
    ANCON0 = 0XFF;  /*Desativamos las salidas analogicas*/
    ANCON1 = 0XFF;  /*Desativamos las salidas analogicas*/

    Gpios_PinDirection(GPIOS_PORTC, 6, GPIOS_OUTPUT); /*pin de tx como salida*/
    baudrate = Uart1_Init(115200); /*se iniclaiza el puerto serial a 115200 baudios*/
    
    while (1)
    {
        Uart1_PutString("Hola mundo\n\r");   /*se manda mensaje por puerto serial*/
        Delays_ms(1000);                    /*se cicla por 1 seg*/
    }
}

```

Trasmitir una cadena de caracteres usando interrupciones

```
#include <p18cxxx.h>
#include "vectors.h"
#include "types.h"
#include "uart/uart1.h"
#include "system/system.h"
#include "gpios/gpios.h"

#pragma code
void main(void)
{
    _U32 baudrate;

    ANCON0 = 0XFF;  /*Desativamos las salidas analogicas*/
    ANCON1 = 0XFF;  /*Desativamos las salidas analogicas*/

    Gpios_PinDirection(GPIOS_PORTC, 6, GPIOS_OUTPUT); /*pin de tx como salida*/
    baudrate = Uart1_Init(115200);   /*se iniciliza el puerto serial a 115200 baudios*/
    __ENABLE_INTERRUPTS();          /*habilitamos interrupciones globales*/

    while (1)
    {
        if(Uart1_TxBusy() == 0)              /*preguntamos si esta libre para transmitir*/
        {
            /*transmitimos una cadena de datos almacenada en flash*/
            Uart1_TxFlashBuffer((rom _U08*)"Hola mundo\n\r", sizeof("Hola mundo\n\r")-1);
        }        
    }
}


#pragma interruptlow YourLowPriorityISRCode
void YourLowPriorityISRCode(void)
{
    Uart1_TxIsr();/*se transmite los datos seriales en esta interrupcion*/
    /*coloca aquí el código que llevará tu interrupción de baja prioridad en caso de usarla*/
}
```

Simple recepción y transmicion de un eco por interrupciones en **hardware_profile.h** se debe escribir: #define UART1_ENABLE_RX      1

```
#include <p18cxxx.h>
#include "vectors.h"
#include "types.h"
#include "uart/uart1.h"
#include "system/system.h"
#include "gpios/gpios.h"

static _U08 gu8RxData;
static _BOOL gbFlag = 0;

#pragma code
void main(void)
{
    _U32 baudrate;
    ANCON0 = 0XFF;  /*Desativamos las salidas analogicas*/
    ANCON1 = 0XFF;  /*Desativamos las salidas analogicas*/
    Gpios_PinDirection(GPIOS_PORTC, 6, GPIOS_OUTPUT); /*pin de tx como salida*/
    Gpios_PinDirection(GPIOS_PORTC, 7, GPIOS_INPUT); /*pin de rx como entrada*/
    baudrate = Uart1_Init(9600);   /*se iniclaiza el puerto serial a 9600 baudios*/
    __ENABLE_INTERRUPTS();          /*habilitamos interrupciones globales*/

    while (1)
    {
        if(gbFlag == 1) /*llego un caracter por teclado*/
        {
            gbFlag = 0; /*limpiamos la bandera*/
            Uart1_PutChar(gu8RxData);/*lo enviamos de regreso para tener feedback visual*/
        }
    }
}

#pragma interruptlow YourLowPriorityISRCode
void YourLowPriorityISRCode(void)
{
    Uart1_RxIsr();/*en esta función se capturan los datos de llegada*/
}

/*esta función es llamada dentro de la función Uart_RxIsr() y se encarga de pasar a 
la aplicación los datos llegados por puerto serial*/
void Uart1_CallbackRx(_U08 u8Data)
{
    gu8RxData = u8Data;/*se respalda el dato llegado en una variable global*/
    gbFlag = 1;     /*se activa una bandera para indicar a la aplicación que se tiene un dato*/
}
```

####Configuración
El driver puede configurarse con 2 parámetros extras, uno de ellos indica la cantidad máxima de datos a transmitir por interrupciones y otro mas indica si se usara recepción. En el archivo **hardware_profile.h** se pueden definir las siguientes constantes:

```
#define UART1_ENABLE_RX          0  /*Definición para habilitar la recepción por interrupciones, 
                                    setear a 1 (no hay por poleo)*/
#define UART1_TX_BUFFER          30 /*Se indica la cantidad máxima de datos que se pueden trasmitir 
                                    por interrupciones */
```
Si no se definen estas constantes se tomaran sus valores por default.

####API
```
    /*-- Functions --*/
    /**---------------------------------------------------------------------------------------------
      \brief      Inicializa el periférico del puerto serial uart a la velocidad seleccionada
      \param      u32BaudRate.- velocidad de tx (9600, 57600, 115200)
      \return     El valor actual que se estableció en el registro de baudrate
      \warning    Por default la configuración es tx de 8 bits, sin paridad y 1 bit de stop (_8N1)
    ----------------------------------------------------------------------------------------------*/
    _U32 Uart1_Init(const _U32 u32BaudRate);

    /**---------------------------------------------------------------------------------------------
      \brief      Cambia la prioridad de interrupción de Tx.
      \param      bPriority.- prioridad _HIGH o _LOW
      \return     None
      \warning    Por default la función Uart_Init  pone la prioridad en _LOW
    ----------------------------------------------------------------------------------------------*/
    void Uart1_TxInterruptProprity(const _BOOL bPriority);

    /**---------------------------------------------------------------------------------------------
      \brief      Cambia la prioridad de interrupción de Rx.
      \param      bPriority.- prioridad _HIGH o _LOW
      \return     None
      \warning    Por default la función Uart_Init  pone la prioridad en _LOW
    ----------------------------------------------------------------------------------------------*/
    void Uart1_RxInterruptProprity(const _BOOL bPriority);

    /**---------------------------------------------------------------------------------------------
      \brief      Transmite un solo carácter de 8 bits por puerto serial
      \param      u8Char.- Carácter a transmitir
      \return     None
      \warning    La función se espera a que el buffer de trasmisión este libre para mandar el carácter
    ----------------------------------------------------------------------------------------------*/
    void Uart1_PutChar(const _S08 u8Char);

    /**---------------------------------------------------------------------------------------------
      \brief      Transmite una cadena de caracteres terminada en cero
      \param      strString.- Cadena de caracteres a transmitir
      \return     None
      \warning    la función traba al procesador hasta que la cadena es enviada por completo y solo
				  funciona con cadenas almacenadas en memoria flash
    ----------------------------------------------------------------------------------------------*/
    void Uart1_PutString(const rom _S08 *strString);

    /**---------------------------------------------------------------------------------------------
      \brief      Transmite un arreglo de datos mediante interrupciones
      \param      pu8Char.- puntero al arreglo de datos
      \param      pu8Lenght.- numero de elementos a transmitir
      \return     None
      \warning    la Función activa interrupción por tx y deja que la interrupción transmita los datos
                  la función Uart_TxBusy debe regresar cero antes de llamar esta función
    ----------------------------------------------------------------------------------------------*/
    void Uart1_TxBuffer(const _U08 *pu8Char, const _U08 u8Lenght);

    /**---------------------------------------------------------------------------------------------
      \brief      Transmite un arreglo de datos almacenados en memoria flash mediante interrupciones
      \param      pu8Char.- puntero al arreglo de datos
      \param      pu8Lenght.- numero de elementos a transmitir
      \return     None
      \warning    la Función activa interrupción por tx y deja que la interrupción transmita los datos
                  la función Uart_TxBusy debe regresar cero antes de llamar esta función
    ----------------------------------------------------------------------------------------------*/

    void Uart1_TxFlashBuffer(const rom _U08 *pu8Char, const _U08 u8Lenght);
    /**---------------------------------------------------------------------------------------------
      \brief      Revisa si el puerto serial esta ocupado trasmitiendo
      \param      None
      \return     Regresa un uno si esta ocupado y un cero si esta libre para tx
      \warning    None
    ----------------------------------------------------------------------------------------------*/
    _BOOL Uart1_TxBusy(void);
    
    /**---------------------------------------------------------------------------------------------
      \brief      Función de interrupción por transmisión de datos en puerto serial, esta función es
				  complemento de las función Uart1_TxBuffer y Uart1_TxFlashBuffer  
      \param      None
      \return     None
      \warning    Indispensable mandar llamar esta función en alguno de los vectores de interrupción
    ----------------------------------------------------------------------------------------------*/
    void Uart1_TxIsr(void);

    /**---------------------------------------------------------------------------------------------
      \brief      Función de interrupción por recepción en puerto serial, lo único que se realiza aquí 
                  es mandar llamar la función Uart1_CallbackRx y pasarle el dato llegado
      \param      None
      \return     None
      \warning    Indispensable mandar llamar esta función en alguno de los vectores de interrupción
    ----------------------------------------------------------------------------------------------*/
    void Uart1_RxIsr(void);

    /**---------------------------------------------------------------------------------------------
      \brief      Función callback mandada llamar por la función Uart1_RxIsr, es necesario que la 
				  aplicación defina esta función y establezca que hacer con dad parámetro que llegue
      \param      u8Data.- dato de 8 bits llegado por puerto serial
      \return     None
      \warning    Es importante recordar que esta función se ejecuta dentro de la interrupción por rx
 				  y debe ser lo mas eficiente posible
    ----------------------------------------------------------------------------------------------*/
    extern void Uart1_CallbackRx(_U08 u8Data);

    /**---------------------------------------------------------------------------------------------
      \brief      Regresa un carácter llegado del puerto serial
      \param      None
      \return     Carácter llegado del puerto serial
      \warning    Esta función traba al procesador hasta recibir un carácter por serial. Es
				  conveniente solo utilizarla para propósitos de depuración y pruebas.
    ----------------------------------------------------------------------------------------------*/
	_U08 Uart1_u8GetChar(void);
```

####Ejemplos

- [Ejemplo 1: Transmicion de una cadena de caracteres a 115200][1]
- [Ejemplo 2: Transmicion de una cadena de caracteres con interrupciones a 115200][2]
- [Ejemplo 3: Transmicion de una cadena cada que se presiona un botón a 9600][3]
- [Ejemplo 4: Simple eco de un caracter escrito en una terminal a 9600][4]
- [Ejemplo 5: Encendido y apagado de leds con recepción de cadenas por puerto serial a 9600][5]
- [Ejemplo 6: Transmicion de una cadena de caracteres a 115200 usando uart2][6]


  [1]: http://www.hotboards.org/images/codigo/8bits/examples/uart1.zip
  [2]: http://www.hotboards.org/images/codigo/8bits/examples/uart2.zip
  [3]: http://www.hotboards.org/images/codigo/8bits/examples/uart3.zip
  [4]: http://www.hotboards.org/images/codigo/8bits/examples/uart4.zip
  [5]: http://www.hotboards.org/images/codigo/8bits/examples/uart5.zip
  [6]: http://www.hotboards.org/images/codigo/8bits/examples/uart6.zip
