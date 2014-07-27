/*--------------------------------------------------------------------------------------------------
  "THE BEER-WARE LICENSE" (Revision 42):
  <phk@FreeBSD.ORG> wrote this file. As long as you retain this notice you
  can do whatever you want with this stuff. If we meet some day, and you think
  this stuff is worth it, you can buy me a beer in return to Hotboards crew
  (Beer-ware license created by Poul-Henning Kamp)

  \file         usb_cdc.c
  \author       Diego
  \email        diego.perez@hotboards.org
  \ver          2.0
  \date         July 25, 2014
  \target       PIC18F series

  \brief        Este driver maneja la operacion del periferico USB en modo CDC es decir emula una
                una comunicacion serial. Este archivo funciona como un wrapper entre el stack oficial
                de Microchip y la libraria oficial de Hotboards, simplificando en gran medida la
                implementacion anterior. Es necesario que el micro corra a una velocidad de 48MHz
 -------------------------------------------------------------------------------------------------*/
/*-- Includes --*/
#include "usb_cdc.h"
#include "usb_cdc/usb.h"
#include "usb_cdc/usb_device.h"
#include "usb_cdc/usb_device_cdc.h"

/*-- Private Definitions--*/


/*-- Global variables --*/


/*-- Private Macros --*/


/*-- Private functions prototypes --*/


/*-- External functions --*/
/**-----------------------------------------------------------------------------------------------*/
void UsbCDC_Init( _U32 u32Baud )
{
    USBDeviceInit();
    USBDeviceAttach();

    CDCSetCharacterFormat(NUM_STOP_BITS_1);
    CDCSetParity(PARITY_NONE);
    CDCSetDataSize(8);
    CDCSetBaudRate(u32Baud);
}
/**-----------------------------------------------------------------------------------------------*/

/**-----------------------------------------------------------------------------------------------*/
_BOOL UsbCDC_bEnumerated(void)
{
    if((USBGetDeviceState() < CONFIGURED_STATE) || (USBIsDeviceSuspended() == true))
    {
        return _FALSE;
    }
    else
    {
        return _TRUE;
    }
}
/**-----------------------------------------------------------------------------------------------*/

/**-----------------------------------------------------------------------------------------------*/
void UsbCDC_Task(void)
{
    USBDeviceTasks();
    CDCTxService();
}
/**-----------------------------------------------------------------------------------------------*/

/**-----------------------------------------------------------------------------------------------*/
_BOOL UsbCDC_bReady( void )
{
    return USBUSARTIsTxTrfReady();
}
/**-----------------------------------------------------------------------------------------------*/

/**-----------------------------------------------------------------------------------------------*/
void UsbCDC_SendString( char *strString )
{
    putsUSBUSART(strString);
}
/**-----------------------------------------------------------------------------------------------*/

/**-----------------------------------------------------------------------------------------------*/
void UsbCDC_TxData( _U08 *pu8Buffer, _U08 u8Lenght )
{
    putUSBUSART(pu8Buffer, u8Lenght);
}

/**-----------------------------------------------------------------------------------------------*/
_U08 UsbCDC_u8RxData( _U08 *pu8Buffer, _U08 u8Lenght )
{
    return getsUSBUSART(pu8Buffer, u8Lenght);
}
/**-----------------------------------------------------------------------------------------------*/

/**-----------------------------------------------------------------------------------------------*/
bool USER_USB_CALLBACK_EVENT_HANDLER(USB_EVENT event, void *pdata, uint16_t size)
{
    switch( (int) event )
    {
        case EVENT_TRANSFER:
            break;

        case EVENT_SOF:
            /* We are using the SOF as a timer to time the LED indicator.  Call
             * the LED update function here. */
            break;

        case EVENT_SUSPEND:
            /* Update the LED status for the suspend event. */
//            APP_LEDUpdateUSBStatus();
            break;

        case EVENT_RESUME:
            /* Update the LED status for the resume event. */
//            APP_LEDUpdateUSBStatus();
            break;

        case EVENT_CONFIGURED:
            /* When the device is configured, we can (re)initialize the
             * demo code. */
            CDCInitEP();
            break;

        case EVENT_SET_DESCRIPTOR:
            break;

        case EVENT_EP0_REQUEST:
            /* We have received a non-standard USB request.  The HID driver
             * needs to check to see if the request was for it. */
            USBCheckCDCRequest();
            break;

        case EVENT_BUS_ERROR:
            break;

        case EVENT_TRANSFER_TERMINATED:
            break;

        default:
            break;
    }
    return true;
}
/**-----------------------------------------------------------------------------------------------*/
