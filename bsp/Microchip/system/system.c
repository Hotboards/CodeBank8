/*--------------------------------------------------------------------------------------------------
  "THE BEER-WARE LICENSE" (Revision 42):
  <phk@FreeBSD.ORG> wrote this file. As long as you retain this notice you
  can do whatever you want with this stuff. If we meet some day, and you think
  this stuff is worth it, you can buy me a beer in return to Hotboards crew 
  (Beer-ware license created by Poul-Henning Kamp)

  \file         system.c
  \author       Diego
  \email        diego.perez@hotboards.org
  \ver          2.0
  \date         May 30, 2014
  \target       PIC18F series

  \brief        Este driver maneja operaciones internas realizadas por el CPU tales como activacion/
                desactivacion de interrupciones, reset del contador de watchdog, reset por software,
                y la activacion interna del PLL para que aumente la frecuencia de operacion a 48MHz
 -------------------------------------------------------------------------------------------------*/
/*-- Includes --*/
#include "system.h"
#include <xc.h>
#include <string.h>


/*-- Private Definitions--*/


/*-- Global variables --*/
static volatile _U08 *const guap8Inputs[] =
{
    &RPINR1,
    &RPINR2,
    &RPINR3,
    &RPINR4,
    &RPINR6,
    &RPINR7,
    &RPINR8,
    &RPINR12,
    &RPINR13,
    &RPINR16,
    &RPINR17,
    &RPINR21,
    &RPINR22,
    &RPINR23,
    &RPINR24
};

static volatile _U08 *const guap8Outputs[] =
{
    &RPOR0,
    &RPOR1,
    &RPOR2,
    &RPOR3,
    &RPOR4,
    &RPOR5,
    &RPOR6,
    &RPOR7,
    &RPOR8,
    &RPOR9,
    &RPOR10,
    &RPOR11,
    &RPOR12,
    &RPOR13,
    NULL,
    NULL,
    NULL,
    &RPOR17,
    &RPOR18,
    &RPOR19,
    &RPOR20,
    &RPOR21,
    &RPOR22,
    &RPOR23,
    &RPOR24
};


/*-- Private Macros --*/


/*-- Private functions prototypes --*/
static void unlock(void);
static void lock(void);


/*-- External functions --*/
/**-----------------------------------------------------------------------------------------------*/
void System_EnablePLL(void)
{
    _U16 pll_startup_counter  = 600;
    SET_8BIT(OSCTUNE, 6); /*Enable the PLL and wait 2+ms until the PLL locks before enabling USB module*/
    while(pll_startup_counter--);
}
/**-----------------------------------------------------------------------------------------------*/

/**-----------------------------------------------------------------------------------------------*/
void System_EnableInterrupts(void)
{
    SET_8BIT(INTCON, 7);    /*enable high priority interrupts*/
    SET_8BIT(INTCON, 6);    /*enable low priority interrupts*/
    SET_8BIT(RCON, 7);      /*enable priority levels*/
}
/**-----------------------------------------------------------------------------------------------*/

/**-----------------------------------------------------------------------------------------------*/
void System_DisableInterrupts(void)
{
    CLEAR_8BIT(INTCON, 7);    /*disable high priority interrupts*/
    CLEAR_8BIT(INTCON, 6);    /*disable low priority interrupts*/
    CLEAR_8BIT(RCON, 7);      /*disable priority levels*/
}
/**-----------------------------------------------------------------------------------------------*/

/**-----------------------------------------------------------------------------------------------*/
void System_PeripheralPinSelect(_ePPS ePeripheral, _U08 u8Pin)
{
    unlock();
    if((_U08)ePeripheral <= (_U08)PWMFaultInput)
    {
        *guap8Inputs[(_U08)ePeripheral] = u8Pin;
    }
    else if(((_U08)ePeripheral >= (_U08)Comparator1Output) && ((_U08)ePeripheral <= (_U08)EnhancedPWMOutputChannel2D))
    {
        *guap8Outputs[u8Pin] = (_U08)ePeripheral % (_U08)100;
    }
    lock();
}
/**-----------------------------------------------------------------------------------------------*/

/*-- Private functions --*/
/**-------------------------------------------------------------------------------------------------    
  \brief        desbloquea los regitros de remapeado de pines
  \param        None
  \return       None
  \warning      None       
--------------------------------------------------------------------------------------------------*/
static void unlock(void)
{
    asm ("MOVLW 0x55");
    asm ("MOVWF EECON2, 0");
    asm ("MOVLW 0xAA");
    asm ("MOVWF EECON2, 0");
    //Turn off PPS Write Protect
    PPSCON = 0;//_asm BCF PPSCON, IOLOCK, BANKED _endasm;
}

/**-------------------------------------------------------------------------------------------------
  \brief        bloquea los registros de remapeo de pines
  \param        None
  \return       None
  \warning      None
--------------------------------------------------------------------------------------------------*/
static void lock(void)
{
    asm ("MOVLW 0x55");
    asm ("MOVWF EECON2, 0");
    asm ("MOVLW 0xAA");
    asm ("MOVWF EECON2, 0");
    // Write Protect PPS (if desired)
    PPSCON = 1;//_asm BSF PPSCON, IOLOCK, BANKED _endasm;
}

