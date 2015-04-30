#include "usb_keyboard/usb.h"
#include "usb_keyboard/usb_device_hid.h"
#include "usb_keyboard/usb_keyboard.h"


const struct{uint8_t report[HID_RPT01_SIZE];}hid_rpt01 =
{
    {
        0x05, 0x01
    }
};

void UsbKeyboard_Init(void)
{
    USBDeviceInit();
    USBDeviceAttach();
}

void UsbKeyboard_Task(void)
{
    USBTasks();
}

bool USER_USB_CALLBACK_EVENT_HANDLER(USB_EVENT event, void *pdata, uint16_t size)
{
    switch((int)event)
    {
        case EVENT_TRANSFER:
            break;

        case EVENT_SOF:
            /* We are using the SOF as a timer to time the LED indicator.  Call
             * the LED update function here. */
            break;

        case EVENT_SUSPEND:
            /* Update the LED status for the suspend event. */
            break;

        case EVENT_RESUME:
            /* Update the LED status for the resume event. */
            break;

        case EVENT_CONFIGURED:
            /* When the device is configured, we can (re)initialize the keyboard
             * demo code. */
            Keyboard_Init();
            break;

        case EVENT_SET_DESCRIPTOR:
            break;

        case EVENT_EP0_REQUEST:
            /* We have received a non-standard USB request.  The HID driver
             * needs to check to see if the request was for it. */
            USBCheckHIDRequest();
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

