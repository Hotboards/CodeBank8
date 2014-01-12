Communication Device Class (CDC)
--------------------------------
-------

Esta es una de las implementaciones del protocolo USB y es usado en mayor medida para realizar emulaciones del antiguo puerto serial sobre el mas nuevo puerto USB. Esta clase puede ser implementada por interrupciones o por poleo (valor por default), es necesario que en cada proyecto se incluyan los archivos **usb_config.h** y **usb_descriptors.c** en donde es posible configurar parámetros del stack.

Siendo una clase no muy sencilla de utilizar, la mejor forma de comenzar un proyecto es descargando uno de los ejemplos que aquí te ofrecemos, estos ejemplos son lo suficiente básicos y sencillos para permitirte empezar a trabajar con esta clase.

Sera necesario instalar un pequeño driver en tu PC para que tu computadora pueda reconocer tu aplicación CDC. [Descargar Driver][1]

####Ejemplo de uso
Simple transmisión de datos por USB usando emulación serial
```
#include <p18cxxx.h>
#include "vectors.h"
#include "types.h"
#include "system/system.h"
#include "usb/usb.h"
#include "usb/cdc/usb_function_cdc.h"

#pragma code
void main(void)
{
    ANCON0 = 0XFF;  /*Desactivamos las salidas analógicas*/
    ANCON1 = 0XFF;  /*Desactivamos las salidas analógicas*/

    System_EnablePLL(); /*CPU a 48MHz, obligatorio para USB*/
    USBDeviceInit();    /*inicializamos el stack de USB*/

    while (1)
    {
        USBTasks();     /*poleamos de manera continua el estado del bus USB*/
        if((USBDeviceState >= CONFIGURED_STATE) || (USBSuspendControl != 1))
        {/*preguntamos si la PC ya enumero nuestro uC*/
            if(USBUSARTIsTxTrfReady())
            {/*preguntamos si el puerto USB esta libre para transmitir*/
                putrsUSBUSART("Hola mundo soy USB!!\r\n");  /*transmitimos una simple cadena*/
            }
            CDCTxService();      /*damos servicio a la clase CDC*/
        }
    }
}

_BOOL USER_USB_CALLBACK_EVENT_HANDLER(int event, void *pdata, _U16 size)
{
    /*Esta función callback es obligatoria y debe ser implementada por el usuario
    acorde a su aplicación, para mayor información revisar la pagina 215 del manual de la libreria
    se puede tomar un ejemplo de su implementacion descargando los ejemplos de cada clase*/
}
```

Simple transmision de datos por USB usando emulación serial, se utilizan interrupciones en el usb
```
#include <p18cxxx.h>
#include "vectors.h"
#include "types.h"
#include "system/system.h"
#include "usb/usb.h"
#include "usb/cdc/usb_function_cdc.h"

#pragma code
void main(void)
{
    ANCON0 = 0XFF;  /*Desativamos las salidas analogicas*/
    ANCON1 = 0XFF;  /*Desativamos las salidas analogicas*/

    System_EnablePLL(); /*CPU a 48MHz, obligatorio para USB*/
    USBDeviceInit();    /*inicializamos el stack de USB*/
    __ENABLE_INTERRUPTS(); //habilitamos interrupciones

    while (1)
    {
        if(USBGetDeviceState() == DETACHED_STATE)
        {//si el dispositivo esta des-enumerado, se enumera
            USBDeviceAttach();
        }
        if((USBDeviceState >= CONFIGURED_STATE) || (USBSuspendControl != 1))
        {/*preguntamos si la PC ya enumero nuestro uC*/
            if(USBUSARTIsTxTrfReady())
            {/*preguntamos si el puerto USB esta libre para transmitir*/
                putrsUSBUSART("Hola mundo soy USB!!\r\n");  /*transmitimos una simple cadena*/
            }
            CDCTxService();      /*damos servicio al periferico USB*/
        }
    }
}

#pragma interrupt YourHighPriorityISRCode
void YourHighPriorityISRCode(void)
{
    USBTasks();     /*poleamos de manera continua el estado del bus USB*/
}
```

####Configuración
Para configurar el funcionamiento de la case CDC se utilizan dos archivos **usb_config.h** y **usb_descriptors.c** en estos archivos se establece el funcionamiento de la librería USB en modo CDC. Estos dos archivos pueden ser extraídos de alguno de los ejemplos.

Una de las opciones mas importantes en usb_config.h es definir si el stack se usara con interrupciones o por poleo, para hacer eso se debe comentar una opcion y descomentar la otra (nunca usar las dos al mismo tiempo)
```
#define USB_POLLING  /*opción por default*/
//#define USB_INTERRUPT
```

####API
```
    /*-- Functions --*/
    /**---------------------------------------------------------------------------------------------
      \brief      Esta función (macro) es usada para setear un valor de baudrate
      \param      baudRate.- baudrate deseado
      \return     None
      \warning    Esta función es opcional pues hay equipos CDC que no modifican su velocidad de baudrate
    ----------------------------------------------------------------------------------------------*/
	void CDCSetBaudRate(DWORD baudRate);

	/**---------------------------------------------------------------------------------------------
      \brief      Establece el numero de bits de stop usados en la transmision
      \param      charFormat - numero de bits de stop (NUM_STOP_BITS_1, NUM_STOP_BITS_1_5, NUM_STOP_BITS_2)
      \return     None
      \warning    Esta función es opcional pues hay equipos CDC que no modifican su formato de tx
    ----------------------------------------------------------------------------------------------*/
	void CDCSetCharacterFormat(BYTE charFormat);

	/**---------------------------------------------------------------------------------------------
      \brief      Establece el formato de paridad a usar en la transmision
      \param      parityType - tipo de paridad (PARITY_NONE, PARITY_ODD, PARITY_EVEN, PARITY_MARK, PARITY_SPACE)
      \return     None
      \warning    Esta función es opcional pues hay equipos CDC que no modifican su formato de paridad
    ----------------------------------------------------------------------------------------------*/
	void CDCSetParity(BYTE parityType);

	/**---------------------------------------------------------------------------------------------
      \brief      Establece el numero de bits a transmitir 
      \param      dataBits - numero de bits (5, 6, 7, 8, or 16).
      \return     None
      \warning    None
    ----------------------------------------------------------------------------------------------*/
	void CDCSetDataSize(BYTE dataBits);	

	/**---------------------------------------------------------------------------------------------
      \brief      Indica si la clase CDC esta lista para mandar mas datos 
      \param      None
      \return     '1' si esta listo para enviar datos
      \warning    El valor de retorno de esta función es valido si el dispositivo ya ha sido enumerado,
                  la función USBDeviceGetState() debió regresar el valor CONFIGURED_STATE
    ----------------------------------------------------------------------------------------------*/
	BOOL USBUSARTIsTxTrfReady(void);

	/**---------------------------------------------------------------------------------------------
      \brief      Esta función revisa si el paquete llegado del host corresponde a la clase CDC, de
                  ser así la función se encarga de mandar la respuesta apropiada      
      \param      None
      \return     None
      \warning    Esta función debe ser llamada solo después de la llegada de una transferencia de 
                  control, un buen lugar es la callback function void USBCBCheckOtherReq(void)
    ----------------------------------------------------------------------------------------------*/
	void USBCheckCDCRequest(void);

    /**---------------------------------------------------------------------------------------------
      \brief      Esta función inicializa la clase CDC, ademas de establecer los valores por default
                  de transmicion (baudrate, paridad, stop y numero de bits). La función habilita
                  los endpoints asignados y los prepara para la transmicion del host     
      \param      None
      \return     None
      \warning    Esta función debe ser llamada hasta después de configurarse el USB. el mejor lugar
                  es dentro dela función callbak void USBCBInitEP(void)
    ----------------------------------------------------------------------------------------------*/
	void CDCInitEP(void);

    /**---------------------------------------------------------------------------------------------
      \brief      Lee determinado numero de bytes llegados a traves del USB en el endpoint Bulk OUT, 
                  los bytes recibidos los almacena en un buffer 
      \param      buffer.- puntero a arreglo donde almacenara los bytes llegados
      \param      len.- numero de bytes esperados (este valor no deberá exceder el ancho del enpoint)            
      \return     Numero de bytes que actualmente se recibieron
      \warning    La función no bloquea hasta que llegan datos, si no existen datos llegados, regresa
                  un valor cero
    ----------------------------------------------------------------------------------------------*/
	BYTE getsUSBUSART(char *buffer, BYTE len);

    /**---------------------------------------------------------------------------------------------
      \brief      Manda un arreglo de datos al host
      \param      data.- puntero donde se encuentran los datos a mandar (solo ram)
      \param      Length.- numero de bytes a mandar            
      \return     None
      \warning    La función se debe usar si el USB no esta ocupado, USBUSARTIsTxTrfReady() regrese '1')
    ----------------------------------------------------------------------------------------------*/
	void putUSBUSART(char *data, BYTE Length);

    /**---------------------------------------------------------------------------------------------
      \brief      Manda una cadena terminada en null '\0' almacenada en ram
      \param      data.- puntero donde se encuentra la cadena a mandar (solo ram)
      \return     None
      \warning    La función se debe usar si el USB no esta ocupado, USBUSARTIsTxTrfReady() regrese '1')
    ----------------------------------------------------------------------------------------------*/
    void putsUSBUSART(char *data);

    /**---------------------------------------------------------------------------------------------
      \brief      Manda una cadena terminada en null '\0' almacenada en flash
      \param      data.- cadena de datos a enviar
      \return     None
      \warning    La función se debe usar si el USB no esta ocupado, USBUSARTIsTxTrfReady() regrese '1')
    ----------------------------------------------------------------------------------------------*/
    void putrsUSBUSART(const ROM char *data);

    /**---------------------------------------------------------------------------------------------
      \brief      Función periódica encargada de dar servicio a las transmiciones y recepciones que
                  se realizan en el usb con la clase CDC
      \param      None
      \return     None
      \warning    La función se debe se llamada de manera periódica a partir de que el dispositivo
                  este enumerado (USBGetDeviceState() >= CONFIGURED_STATE)
    ----------------------------------------------------------------------------------------------*/
    void CDCTxService(void);
```
Para mayor información de las funciones revisar la pagina 254 del [manual][2] de la librería de USB

####Ejemplos

- [Ejemplo 1: Transmicion continua de una cadena de caracteres][3]
- [Ejemplo 2: Simple eco de caracteres][4]
- [Ejemplo 3: Transmicion continua de una cadena de caracteres usando interrupciones][5]


  [1]: http://www.hotboards.org/images/files/mchpinf.zip
  [2]: http://www.hotboards.org/images/files/MCHPFUSB.pdf
  [3]: http://github.com/Hotboards/Examples/blob/master/Microchip/usb_cdc1.X/main.c
  [4]: http://github.com/Hotboards/Examples/blob/master/Microchip/usb_cdc2.X/main.c
  [5]: http://github.com/Hotboards/Examples/blob/master/Microchip/usb_cdc3.X/main.c