/*--------------------------------------------------------------------------------------------------
  "THE BEER-WARE LICENSE" (Revision 42):
  <phk@FreeBSD.ORG> wrote this file. As long as you retain this notice you
  can do whatever you want with this stuff. If we meet some day, and you think
  this stuff is worth it, you can buy me a beer in return to Microchip Technology
  (Beer-ware license created by Poul-Henning Kamp)

  \file         delays.c
  \author       Atmel
  \email        
  \ver          1.0
  \date         June 13, 2013
  \target       ATmega series
  
  \brief        Código para generar retardos por software sin utilizar ningún periférico del 
                microcontrolador. Los retardos los realiza mediante decrementos acorde a la frecuencia 
                de operación del CPU. Se debe tomar en cuenta que el uso de interrupciones puede 
                afectar la duración de los retardos.
 -------------------------------------------------------------------------------------------------*/
/*-- Includes --*/
#include "delays.h"


/*-- Private Definitions--*/

    


/*-- Global variables --*/


/*-- Private Macros --*/


/*-- Private functions prototypes --*/


/*-- External functions --*/
/**-----------------------------------------------------------------------------------------------*/
void Delays_ms( _U16 ms )
{
    _U16 i;
    
    for(i=0;i<ms;i++)
    {
        _delay_ms(1);
    }
}
/**-----------------------------------------------------------------------------------------------*/


/*-- Private functions --*/
/**-------------------------------------------------------------------------------------------------    
  \brief        None
  \param        None
  \return       None
  \warning      None       
--------------------------------------------------------------------------------------------------*/
