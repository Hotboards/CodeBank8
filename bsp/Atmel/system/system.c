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
  \date         June 14, 2014
  \target       ATmega series

  \brief        Este driver maneja operaciones internas realizadas por el CPU tales como activacion/
                desactivacion de interrupciones, reset del contador de watchdog, reset por software,
                y la activacion interna del PLL para que aumente la frecuencia de operacion a 48MHz
 -------------------------------------------------------------------------------------------------*/
/*-- Includes --*/
#include "system.h"


/*-- Private Definitions--*/


/*-- Global variables --*/


/*-- Private Macros --*/


/*-- Private functions prototypes --*/


/*-- External functions --*/
/**-----------------------------------------------------------------------------------------------*/

/**-----------------------------------------------------------------------------------------------*/

/*-- Private functions --*/
/**-------------------------------------------------------------------------------------------------    
  \brief        desbloquea los regitros de remapeado de pines
  \param        None
  \return       None
  \warning      None       
--------------------------------------------------------------------------------------------------*/

