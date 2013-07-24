/*--------------------------------------------------------------------------------------------------
  "THE BEER-WARE LICENSE" (Revision 42):
  <phk@FreeBSD.ORG> wrote this file. As long as you retain this notice you
  can do whatever you want with this stuff. If we meet some day, and you think
  this stuff is worth it, you can buy me a beer in return to Hotboards crew 
  (Beer-ware license created by Poul-Henning Kamp)

  \file         system.c
  \author       Diego
  \email        diego.perez@hotboards.org
  \ver          1.0
  \date         July 16, 2013
  \target       PIC18F series

  \brief        Este driver maneja operaciones internas realizadas por el CPU tales como activacion/
                desactivacion de interrupciones, reset del contador de watchdog, reset por software,
                y la activacion interna del PLL para que aumente la frecuencia de operacion a 48MHz
 -------------------------------------------------------------------------------------------------*/
/*-- Includes --*/
#include "system.h"
#include <p18cxxx.h>


/*-- Private Definitions--*/


/*-- Global variables --*/


/*-- Private Macros --*/


/*-- Private functions prototypes --*/


/*-- External functions --*/
void System_EnablePLL(void)
{
    _U16 pll_startup_counter  = 600;
    SET_8BIT(OSCTUNE, 6); /*Enable the PLL and wait 2+ms until the PLL locks before enabling USB module*/
    while(pll_startup_counter--);
}

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


/*-- Private functions --*/
/**-------------------------------------------------------------------------------------------------    
  \brief        None
  \param        None
  \return       None
  \warning      None       
--------------------------------------------------------------------------------------------------*/
