/*--------------------------------------------------------------------------------------------------
  "THE BEER-WARE LICENSE" (Revision 42):
  <phk@FreeBSD.ORG> wrote this file. As long as you retain this notice you
  can do whatever you want with this stuff. If we meet some day, and you think
  this stuff is worth it, you can buy me a beer in return to Microchip Technology
  (Beer-ware license created by Poul-Henning Kamp)

  \file         delays.c
  \author       Microchip Technology, Inc
  \email        
  \ver          1.0
  \date         July 15, 2013
  \target       PIC18F series
  
  \brief        Código para generar retardos por software sin utilizar ningún periférico del 
                microcontrolador. Los retardos los realiza mediante decrementos acorde a la frecuencia 
                de operación del CPU. Se debe tomar en cuenta que el uso de interrupciones puede 
                afectar la duración de los retardos.
 -------------------------------------------------------------------------------------------------*/
/*-- Includes --*/
#include "delays.h"
#include <p18cxxx.h>
#include "bsp_profile.h"


/*-- Private Definitions--*/
#ifndef BSP_CLOCK
    #define DELAYS_CLOCK                 12000000
#else
    #define DELAYS_CLOCK                 BSP_CLOCK
#endif
    


/*-- Global variables --*/


/*-- Private Macros --*/
#define FREQUENCY()                   (DELAYS_CLOCK/4)

/*-- Private functions prototypes --*/


/*-- External functions --*/
/**-----------------------------------------------------------------------------------------------*/
void Delays_10us( _U32 us )
{
    volatile _S32 s32Cycles;

    if(FREQUENCY() <= 2500000) /*for all FCY speeds under 2MHz (FOSC <= 10MHz)*/
    {
        /*26 cycles burned through this path (includes return to caller).
        For FOSC == 1MHZ, it takes 104us.
        For FOSC == 4MHZ, it takes 26us
        For FOSC == 8MHZ, it takes 13us.
        For FOSC == 10MHZ, it takes 10.5us.*/
    }
    else
    {
        /*14 cycles burned to this point.
        We want to pre-calculate number of cycles required to delay 10us * us using a 1 cycle granule.*/
        s32Cycles = (_S32)(FREQUENCY()/100000) * us;
        /*We subtract all the cycles used up until we reach the while loop below, where each loop cycle count is subtracted.
        Also we subtract the 22 cycle function return.*/
        s32Cycles -= (153 + 22);

        if (s32Cycles <= 45)
        {
            /*If we have exceeded the cycle count already, bail! Best compromise between FOSC == 12MHz and FOSC == 24MHz.*/
        }
        else
        {
            /*Try as you may, you can't get out of this heavier-duty case under 30us.*/
            while (s32Cycles>0) /*153 cycles used to this point*/
            {
                {_asm nop _endasm}; /*Delay one instruction cycle at a time, not absolutely necessary*/
                s32Cycles -= 42; /*Subtract cycles burned while doing each delay stage, 42 in this case*/
            }
        }
    }
}
/**-----------------------------------------------------------------------------------------------*/

/**-----------------------------------------------------------------------------------------------*/
void Delays_ms( _U16 ms )
{
    _S32 s32Cycles;

    /*We want to pre-calculate number of cycles required to delay 1ms, using a 1 cycle granule*/
    s32Cycles = (_S32)(FREQUENCY()/1000) * ms;
    /*We subtract all the cycles used up until we reach the while loop below, where each loop cycle count is subtracted.
    Also we subtract the 22 cycle function return.*/
    s32Cycles -= (148 + 22);

    if (s32Cycles <= (170+25)) 
    {
        return;     /*If we have exceeded the cycle count already, bail!*/
    }    
    else
    {
        while (s32Cycles > 0) /*148 cycles used to this point.*/
        {
            {_asm nop _endasm};              /* Delay one instruction cycle at a time, not absolutely necessary.*/
            s32Cycles -= 39;    /* Subtract cycles burned while doing each delay stage, 39 in this case.*/
        }
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
