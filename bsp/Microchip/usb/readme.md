Universal Serial Bus (USB)
--------------------------
----------
El Universal Serial Bus (USB) es un estándar industrial que define los cables, conectores y protocolos usados en un bus para conectar, comunicar y proveer de alimentación eléctrica entre ordenadores y periféricos y dispositivos electrónicos.

Los sistemas embebidos no son la excepción en donde podrás encontrar estos dispositivos, actualmente la serie **PIC18FJ** tiene la posibilidad de implementar este tipo de comunicación, limitada a actuar solo en modo device. Desafortunadamente al protocolo usb puede presentar un alto grado de complejidad para muchos desarrolladores y el echo de construir un driver desde cero (con todo y sus clases) puede representar un reto demasiado extenso.

En esta sección encontraras una distribución del stack oficial de **Microchip**, con ligeras modificaciones. Este stack es una pieza de código muy completa que te permitirá manejar las diferentes clases de comunicación que especifica el protocolo USB.

Las modificaciones realizadas por **Hotboards** tienen como objetivo presentar esta pieza de código de una manera mas sencilla y compacta. Los cambios solo se basan en la reubicacion de archivos en un árbol de directorios especifico para los **pics18f** y la eliminación del archivo **HardwareProfile.h**, así como de todo aquello no referente a los uC de la serie 18f. En caso de requerir la versión completa y sin modificar de este stack se puede descargar la [Microchip Application Library](http://www.microchip.com/stellent/idcplg?IdcService=SS_GET_PAGE&nodeId=2680&dDocName=en547784).  

####Ejemplos de uso
Esta es la implementacion mas básica del stack de usb, por si solo no hase nada, es necesario implementar las funciones de las diferentes clases que brinda microchip como CDC o HID.
```
#include <p18cxxx.h>
#include "vectors.h"
#include "types.h"
#include "system/system.h"
#include "usb/usb.h"

#pragma code
void main(void)
{
    ANCON0 = 0XFF;  /*Desactivamos las salidas analogicas*/
    ANCON1 = 0XFF;  /*Desactivamos las salidas analogicas*/

    System_EnablePLL(); /*CPU a 48MHz, obligatorio para USB*/
    USBDeviceInit();    /*inicializamos el stack de USB*/

    while (1)
    {
        USBDeviceTasks();     /*poleamos de manera continua el estado del bus USB*/
    }
}

_BOOL USER_USB_CALLBACK_EVENT_HANDLER(int event, void *pdata, _U16 size)
{
	/*Esta función callback es obligatoria y debe ser implementada por el usuario
    acorde a su aplicación, para mayor información revisar la pagina 215 del manual de la libreria
    se puede tomar un ejemplo de su implementacion descargando los ejemplos de cada clase*/
}
```

####Configuración
Para configurar esta pieza de código en particular se usan un par de archivos, **usb_config.h** en el cual configuras la librería con distintos parámetros tales como usar o no interrupciones, endpoints, etc. El otro archivo es el **usb_descriptors.c** y en el se encuentran las configuraciones de los descriptores (para mayor información acerca de que es un descriptor revisar el estandar usb).
Se recomienda copiarlos directamente de algunos ejemplos de que se pueden descargar en cada clase y modificarlos acorde a su aplicación 

####API
La lista completa de las funciones usb device las puedes encontrar en al pagina 211 del [manual][2] de la libreria Microchip USB

####Clases

- [Communications Device Class (CDC)][3]
- Human Interface Device (HID)
- Mass Storage Device (MSD)
- Personal Healthcare Device Class (PHDC)
- Audio Function Driver
- Vendor Class (Generic)

  [2]: http://www.hotboards.org/images/files/MCHPFSUSB.pdf
  [3]: https://github.com/Hotboards/CodeBank8/tree/master/bsp/Microchip/usb/cdc