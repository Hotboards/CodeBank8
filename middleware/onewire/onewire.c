/*-------------------------------------------------------------------------------------------------/
/  Universal string handler for user console interface
/--------------------------------------------------------------------------------------------------/
/
/  Copyright (C) 2011, ChaN, all right reserved.
/
/ * This software is a free software and there is NO WARRANTY.
/ * No restriction on use. You can use, modify and redistribute it for
/   personal, non-profit or commercial products UNDER YOUR RESPONSIBILITY.
/ * Redistributions of source code must retain the above copyright notice.
/
/-------------------------------------------------------------------------------------------------*/
/*-- Includes --*/
#include "onewire.h"
#include "gpios/gpios.h"
#include "delays/delays.h"
#include "hardware_profile.h"


/*-- Private Definitions--*/


/*-- Global variables --*/
static _U08 gua8WirePorts[ONEWIRE_N_PORTS] =
{
    #if ONEWIRE_N_PORTS > 0
        ONEWIRE_PORT_P0,
    #endif
    #if ONEWIRE_N_PORTS > 1
        ONEWIRE_PORT_P1,
    #endif
    #if ONEWIRE_N_PORTS > 2
        ONEWIRE_PORT_P2,
    #endif
    #if ONEWIRE_N_PORTS > 3
        ONEWIRE_PORT_P3,
    #endif
}; 

static _U08 gua8WirePins[ONEWIRE_N_PORTS] =
{
    #if ONEWIRE_N_PORTS > 0
        ONEWIRE_PORT_B0,
    #endif
    #if ONEWIRE_N_PORTS > 1
        ONEWIRE_PORT_B1,
    #endif
    #if ONEWIRE_N_PORTS > 2
        ONEWIRE_PORT_B2,
    #endif
    #if ONEWIRE_N_PORTS > 3
        ONEWIRE_PORT_B3,
    #endif
};


/*-- Private Macros --*/


/*-- Private functions prototypes --*/
void write_bit( _U08 u8Port, _BOOL u8Bit );
_BOOL read_bit( _U08 u8Port );


/*-- External functions --*/
/**-----------------------------------------------------------------------------------------------*/
_BOOL OneWire_bResetPulse( _U08 u8Port )
{
    _BOOL bPresence;

    Gpios_PinDirection(gua8WirePorts[u8Port], gua8WirePins[u8Port], GPIOS_OUTPUT);
    Gpios_ClearPin(gua8WirePorts[u8Port], gua8WirePins[u8Port]); 				// Drive the bus low
    Delays_us(480);	  			// delay 480 microsecond (us)
    Gpios_SetPin(gua8WirePorts[u8Port], gua8WirePins[u8Port]); 				// Drive the bus low
    Delays_us(70);				// delay 70 microsecond (us)

    Gpios_PinDirection(gua8WirePorts[u8Port], gua8WirePins[u8Port], GPIOS_INPUT);
    bPresence = Gpios_bReadPin(gua8WirePorts[u8Port], gua8WirePins[u8Port]);	//Sample for presence pulse from slave
    Gpios_PinDirection(gua8WirePorts[u8Port], gua8WirePins[u8Port], GPIOS_OUTPUT);

    Delays_us(410);	  			// delay 410 microsecond (us)
    Gpios_SetPin(gua8WirePorts[u8Port], gua8WirePins[u8Port]); 				// Drive the bus low

    return bPresence;
}
/**-----------------------------------------------------------------------------------------------*/

/**-----------------------------------------------------------------------------------------------*/
void OneWire_WriteByte( _U08 u8Port, _U08 u8Byte )
{
    _U08 i;

    for(i=0;i<8;i++)
    {
        write_bit(u8Port, u8Byte&0x01); 	//Sending LS-bit first
        u8Byte >>= 1;                   // shift the data byte for the next bit to send
    }
}
/**-----------------------------------------------------------------------------------------------*/

/**-----------------------------------------------------------------------------------------------*/
_U08 OneWire_u8ReadByte( _U08 u8Port )
{
    _U08 i, u8Byte=0;

    for(i=0;i<8;i++)
    {
        u8Byte >>= 1;                   // shift the data byte for the next bit to send
        u8Byte = read_bit(u8Port); 	//Sending LS-bit first
    }
    return u8Byte;
}
/**-----------------------------------------------------------------------------------------------*/


/*-- Private functions --*/
/**-------------------------------------------------------------------------------------------------    
  \brief        None
  \param        None
  \return       None
  \warning      None       
--------------------------------------------------------------------------------------------------*/
void write_bit( _U08 u8Port, _BOOL u8Bit )
{
    Gpios_PinDirection(gua8WirePorts[u8Port], gua8WirePins[u8Port], GPIOS_OUTPUT);
    if(u8Bit == 0)
    {
        Gpios_ClearPin(gua8WirePorts[u8Port], gua8WirePins[u8Port]); 				// Drive the bus low
        Delays_us(60);	  			// delay 480 microsecond (us)
        Gpios_SetPin(gua8WirePorts[u8Port], gua8WirePins[u8Port]); 				// Drive the bus low
        Delays_us(10);				// delay 70 microsecond (us)
    }
    else
    {
        Gpios_ClearPin(gua8WirePorts[u8Port], gua8WirePins[u8Port]); 				// Drive the bus low
        Delays_us(6);	  			// delay 480 microsecond (us)
        Gpios_SetPin(gua8WirePorts[u8Port], gua8WirePins[u8Port]); 				// Drive the bus low
        Delays_us(64);				// delay 70 microsecond (us)
    }
}

/**-------------------------------------------------------------------------------------------------
  \brief        None
  \param        None
  \return       None
  \warning      None
--------------------------------------------------------------------------------------------------*/
_BOOL read_bit( _U08 u8Port )
{
    _BOOL bBit;

    Gpios_PinDirection(gua8WirePorts[u8Port], gua8WirePins[u8Port], GPIOS_OUTPUT);
    Gpios_ClearPin(gua8WirePorts[u8Port], gua8WirePins[u8Port]); 				// Drive the bus low
    Delays_us(6);						// delay 6 microsecond (us)
    Gpios_SetPin(gua8WirePorts[u8Port], gua8WirePins[u8Port]); 				// Drive the bus low
    Delays_us(9);						// delay 9 microsecond (us)

    Gpios_PinDirection(gua8WirePorts[u8Port], gua8WirePins[u8Port], GPIOS_INPUT);
    bBit = Gpios_bReadPin(gua8WirePorts[u8Port], gua8WirePins[u8Port]);	//Sample for presence pulse from slave

    Delays_us(55);						// delay 55 microsecond (us)
    return bBit;
}

