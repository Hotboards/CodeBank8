/*--------------------------------------------------------------------------------------------------
  "THE BEER-WARE LICENSE" (Revision 42):
  <phk@FreeBSD.ORG> wrote this file. As long as you retain this notice you
  can do whatever you want with this stuff. If we meet some day, and you think
  this stuff is worth it, you can buy me a beer in return to Hotboards crew
  (Beer-ware license created by Poul-Henning Kamp)

  \file         _6800.c
  \author       Diego
  \email        diego.perez@hotboards.org
  \ver          1.0
  \date         August 26, 2013
  \target       8-bits Generic

  \brief        Esta pieza de código emula la interfaz paralela 6800 mediante simples pines del uC,
                las tres señales de control (E, RS y RW) utilizadas en esta interfaz se generan
                mediante el driver **gpios**. La amplitud del bus solo puede ser de 4 u 8 bits y para
                ello se utiliza un puerto completo del uC, en caso de 4 bits solo se usan los bits
                mas significativos..
 -------------------------------------------------------------------------------------------------*/
/*-- Includes --*/
#include "_6800.h"
#include "system/system.h"
#include "hardware_profile.h"

/*-- Private Definitions--*/
#ifndef _6800_BUSLENGHT
    #define BUSLENGHT       4
#else
    #define BUSLENGHT       _6800_BUSLENGHT
#endif

#ifndef _6800_ENABLE_TIME
    #define _6800_ENABLE_TIME       1
#endif



/*-- Global variables --*/
static _U08 i;


/*-- Private Macros --*/
#define INIT_ENABLE_PIN()               Gpios_PinDirection(_6800_E_P, _6800_E_B, GPIOS_OUTPUT)
#define INIT_RS_PIN()                   Gpios_PinDirection(_6800_RS_P, _6800_RS_B, GPIOS_OUTPUT)
#define INIT_RW_PIN()                   Gpios_PinDirection(_6800_RW_P, _6800_RW_B, GPIOS_OUTPUT)
#if BUSLENGHT == 8
    #define DATAPORT_OUTPUT()               Gpios_WriteTris(_6800_DATAPORT, GPIOS_OUTPUT*0xFF)
    #define DATAPORT_INPUT()                Gpios_WriteTris(_6800_DATAPORT, GPIOS_INPUT*0xFF)
    #define DATAPORT_WRITE(data)            Gpios_WritePort(_6800_DATAPORT, (data))
    #define DATAPORT_READ()                 Gpios_u8ReadPort(_6800_DATAPORT)
#elif BUSLENGHT == 4
    #define DATAPORT_OUTPUT()               Gpios_WriteTris(_6800_DATAPORT, (GPIOS_OUTPUT*0xFF) | (0x0f & Gpios_u8ReadTris(_6800_DATAPORT)))
    #define DATAPORT_INPUT()                Gpios_WriteTris(_6800_DATAPORT, (GPIOS_INPUT*0xFF) | (0x0f & Gpios_u8ReadTris(_6800_DATAPORT)))
    #define DATAPORT_WRITE(data)            Gpios_WritePort(_6800_DATAPORT, ((data)&0xf0) | (0x0f & Gpios_u8ReadPort(_6800_DATAPORT)))
    #define DATAPORT_READ()                 Gpios_u8ReadPort(_6800_DATAPORT) & 0xf0
#endif
#define SET_ENABLE()                    Gpios_WritePin(_6800_E_P, _6800_E_B, 1u)
#define CLR_ENABLE()                    Gpios_WritePin(_6800_E_P, _6800_E_B, 0u)
#define RS_FOR_DATA()                   Gpios_WritePin(_6800_RS_P, _6800_RS_B, 1u)
#define RS_FOR_COMMAND()                Gpios_WritePin(_6800_RS_P, _6800_RS_B, 0u)
#define RW_FOR_WRITE()                  Gpios_WritePin(_6800_RW_P, _6800_RW_B, 0u)
#define RW_FOR_READ()                   Gpios_WritePin(_6800_RW_P, _6800_RW_B, 1u)

#define _6800_Delay()                   for(i=0;i<_6800_ENABLE_TIME;i++) __NOP();


/*-- Private functions prototypes --*/


/*-- External functions --*/
/**-----------------------------------------------------------------------------------------------*/
void _6800_Init(void)
{
    INIT_ENABLE_PIN();
    INIT_RS_PIN();
    INIT_RW_PIN();
}
/**-----------------------------------------------------------------------------------------------*/

/**-----------------------------------------------------------------------------------------------*/
void _6800_WriteData(_U08 u8Data)
{
    DATAPORT_OUTPUT();
    DATAPORT_WRITE(u8Data);
    RS_FOR_DATA();
    RW_FOR_WRITE();
    _6800_Delay();
    SET_ENABLE();
    _6800_Delay();
    CLR_ENABLE();
}
/**-----------------------------------------------------------------------------------------------*/

/**-----------------------------------------------------------------------------------------------*/
_U08 _6800_u8ReadData(void)
{
    _U08 u8Data;                     

    DATAPORT_INPUT();
    RW_FOR_READ();                   
    RS_FOR_DATA();
    _6800_Delay();
    SET_ENABLE();
    _6800_Delay();
    u8Data = DATAPORT_READ();
    CLR_ENABLE();

    return u8Data;                  
}
/**-----------------------------------------------------------------------------------------------*/

/**-----------------------------------------------------------------------------------------------*/
void _6800_WriteCommand(_U08 u8Cmd)
{
    DATAPORT_OUTPUT();               
    DATAPORT_WRITE(u8Cmd);           
    RW_FOR_WRITE();                  
    RS_FOR_COMMAND();    
    _6800_Delay();
    SET_ENABLE();
    _6800_Delay();
    CLR_ENABLE();
}
/**-----------------------------------------------------------------------------------------------*/

/**-----------------------------------------------------------------------------------------------*/
_U08 _6800_u8ReadAddr(void)    
{
    _U08 u8Addr;
    
    DATAPORT_INPUT();
    RW_FOR_READ();                   
    RS_FOR_COMMAND();
    _6800_Delay();
    SET_ENABLE();
    _6800_Delay();
    u8Addr = DATAPORT_READ();
    CLR_ENABLE();               	
	
    return u8Addr;
}
/**-----------------------------------------------------------------------------------------------*/


/*-- Private functions --*/
