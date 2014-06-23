/*--------------------------------------------------------------------------------------------------
  "THE BEER-WARE LICENSE" (Revision 42):
  <phk@FreeBSD.ORG> wrote this file. As long as you retain this notice you
  can do whatever you want with this stuff. If we meet some day, and you think
  this stuff is worth it, you can buy me a beer in return to Hotboards crew 
  (Beer-ware license created by Poul-Henning Kamp)

  \file         gpios.c
  \author       Diego
  \email        diego.perez@hotboards.org
  \ver          2.0
  \date         June 22, 2014
  \target       PIC18F series

  \brief        El driver de bajo nivel gpios agrupa una colecci�n de funciones que permiten manejar 
                los puertos de una manera m�s manejable y portable. Mediante las funciones descritas 
                en el archivo gpios.h, podr�s establecer la direcci�n, escribir y/o leer, ya sea el 
                puerto entero o pin a pin.
 -------------------------------------------------------------------------------------------------*/
/*-- Includes --*/
#include "gpios.h"
#include <xc.h>


/*-- Private Definitions--*/


/*-- Global variables --*/
volatile _U08 *const guap8Direction[] =
{
#if defined RA0_bit
    &TRISA,
#else
    (0),
#endif
#if defined RB0_bit
    &TRISB,
#else
    (0),
#endif
#if defined RC0_bit
    &TRISC,
#else
    (0),
#endif
#if defined RD0_bit
    &TRISD,
#else
    (0),
#endif
#if defined RE0_bit
    &TRISE,
#else
    (0),
#endif
}; /* tris */
volatile _U08 *const guap8Input[]     =
{
#if defined RA0_bit
    &PORTA,
#else
    (0),
#endif
#if defined RB0_bit
    &PORTB,
#else
    (0),
#endif
#if defined RC0_bit
    &PORTC,
#else
    (0),
#endif
#if defined RD0_bit
    &PORTD,
#else
    (0),
#endif
#if defined RE0_bit
    &PORTE,
#else
    (0),
#endif
}; /* input */
volatile _U08 *const guap8Output[]    =
{
#if defined RA0_bit
    &LATA,
#else
    (0),
#endif
#if defined RB0_bit
    &LATB,
#else
    (0),
#endif
#if defined RC0_bit
    &LATC,
#else
    (0),
#endif
#if defined RD0_bit
    &LATD,
#else
    (0),
#endif
#if defined RE0_bit
    &LATE,
#else
    (0),
#endif
};  /* output */

/*-- Private Macros --*/


/*-- Private functions prototypes --*/


/*-- External functions --*/
/**-----------------------------------------------------------------------------------------------*/
void Gpios_PinDirection(_eGPIOS_PORT ePort, _U08 u8Pin, _BOOL bDirection)
{
    u8Pin &= (_U08)0x07; /*just to be sure you didn't write a value bigger than 7*/
    bDirection &= (_BOOL)0x01; /*just to be sure you didn't write a value bigger than 1*/
    
    if(GPIOS_OUTPUT == bDirection)
    {/*pin as output*/
        CLEAR_8BIT(*guap8Direction[(_U08)ePort], u8Pin);
    }
    else
    {/*pin as input*/
        SET_8BIT(*guap8Direction[(_U08)ePort], u8Pin);
    }
}
/**-----------------------------------------------------------------------------------------------*/

/**-----------------------------------------------------------------------------------------------*/
void Gpios_WritePin(_eGPIOS_PORT ePort, _U08 u8Pin, _BOOL bValue)
{
    u8Pin &= (_U08)0x07; /*just to be sure you didn't write a value bigger than 7*/
    bValue &= (_BOOL)0x01; /*just to be sure you didn't write a value bigger than 1*/
    
    if((_BOOL)1 == bValue)
    {/*set pin to high*/
        SET_8BIT(*guap8Output[(_U08)ePort], u8Pin);
    }
    else
    {/*set pin to low*/
        CLEAR_8BIT(*guap8Output[(_U08)ePort], u8Pin);
    }
}
/**-----------------------------------------------------------------------------------------------*/

/**-----------------------------------------------------------------------------------------------*/
void Gpios_PullupPin(_eGPIOS_PORT ePort, _U08 u8Pin, _BOOL bValue)
{
    /*this function is only here for compatibility.
    Microchip use another mechanism for pullup activation*/
}
/**-----------------------------------------------------------------------------------------------*/

/**-----------------------------------------------------------------------------------------------*/
//void Gpios_TogglePin(_eGPIOS_PORT ePort, _U08 u8Pin)
//{
  //  u8Pin &= (_U08)0x07; /*just to be sure you didn't write a value bigger than 7*/
  //  TOGGLE_8BIT(*guap8Output[(_U08)ePort], u8Pin);
//}
/**-----------------------------------------------------------------------------------------------*/

/**-----------------------------------------------------------------------------------------------*/
//_BOOL Gpios_bReadPin(_eGPIOS_PORT ePort, _U08 u8Pin)
//{
    //u8Pin &= (_U08)0x07; /*just to be sure you didn't write a value bigger than 7*/
    //return QUERY_8BIT(*guap8Input[(_U08)ePort], u8Pin);
//}
/**-----------------------------------------------------------------------------------------------*/

/**-----------------------------------------------------------------------------------------------*/
//void Gpios_WritePort(_eGPIOS_PORT ePort, _U08 u8Value)
//{
    //*guap8Output[(_U08)ePort] = u8Value;
//}
/**-----------------------------------------------------------------------------------------------*/

/**-----------------------------------------------------------------------------------------------*/
//_U08 Gpios_u8ReadPort(_eGPIOS_PORT ePort)
//{
    //return *guap8Input[(_U08)ePort];
//}
/**-----------------------------------------------------------------------------------------------*/

/**-----------------------------------------------------------------------------------------------*/
//void Gpios_WriteTris(_eGPIOS_PORT ePort, _U08 u8Value)
//{
    //*guap8Direction[(_U08)ePort] = u8Value;
//}
/**-----------------------------------------------------------------------------------------------*/

/**-----------------------------------------------------------------------------------------------*/
//_U08 Gpios_u8ReadTris(_eGPIOS_PORT ePort)
//{
    //return *guap8Direction[(_U08)ePort];
//}
/**-----------------------------------------------------------------------------------------------*/

/**-----------------------------------------------------------------------------------------------*/
void Gpios_SetPullups(_eGPIOS_PORT ePort, _U08 u8Value)
{
    /*this function is only here for compatibility.
    Microchip uses another mechanism for pullup activation*/
}
/**-----------------------------------------------------------------------------------------------*/

/**-----------------------------------------------------------------------------------------------*/
_U08 Gpios_u8ReadPullups(_eGPIOS_PORT ePort)
{
    /*this function is only here for compatibility.
    Microchip uses another mechanism for pullup activation*/
    return 0x00;
}
/**-----------------------------------------------------------------------------------------------*/


/*-- Private functions --*/
/**-------------------------------------------------------------------------------------------------    
  \brief        None
  \param        None
  \return       None
  \warning      None   	
--------------------------------------------------------------------------------------------------*/
