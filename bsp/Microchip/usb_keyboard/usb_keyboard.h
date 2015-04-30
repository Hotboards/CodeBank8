/* 
 * File:   usb_keyboard.h
 * Author: diego
 *
 * Created on 29 de julio de 2014, 07:09 PM
 */

#ifndef USB_KEYBOARD_H
#define	USB_KEYBOARD_H

#ifdef	__cplusplus
extern "C" {
#endif

void UsbKeyboard_Init(void);
void UsbKeyboard_Task(void);

#ifdef	__cplusplus
}
#endif

#endif	/* USB_KEYBOARD_H */

