/*--------------------------------------------------------------------------------------------------
  "THE BEER-WARE LICENSE" (Revision 42):
  <phk@FreeBSD.ORG> wrote this file. As long as you retain this notice you
  can do whatever you want with this stuff. If we meet some day, and you think
  this stuff is worth it, you can buy me a beer in return to Hotboards crew
  (Beer-ware license created by Poul-Henning Kamp)

  \file         st7032.c
  \author       Diego
  \email        diego.perez@hotboards.org
  \ver          1.0
  \date         August 29, 2013
  \target       8-bits Generic

  \brief        Pieza de código para controlar un lcd alfanumérico externo con el controlador Hitachi
                hd44780 o compatible. este código solo se encarga de incializar el lcd, así como mandar
                comandos y/o datos al lcd, los detalles internos de comunicación se manejan a través
                del código que maneja la interfaz 6800 en 4 u 8 bits.
 -------------------------------------------------------------------------------------------------*/
/*-- Includes --*/
#include "st7032.h"
#include "spi/spi.h"
#include "delays/delays.h"
#include "gpios/gpios.h"
#include "hardware_profile.h"


/*-- Private Definitions--*/

	

/*-- Private Macros --*/
static _U32 gau8SpiPorts[ST7032_N_LCDS] =
{
    #if ST7032_N_LCDS > 0
        ST7032_SPI0,
    #endif
    #if ST7032_N_LCDS > 1
        ST7032_SPI1,
    #endif
    #if ST7032_N_LCDS > 2
        ST7032_SPI2,
    #endif
    #if ST7032_N_LCDS > 3
        ST7032_SPI3,
    #endif
    #if ST7032_N_LCDS > 4
        #error "numero de LCDs no soportado"
    #endif
};
static _U32 gau8PortSS[ST7032_N_LCDS] =
{
    #if ST7032_N_LCDS > 0
        ST7032_SS_P0,
    #endif
    #if ST7032_N_LCDS > 1
        ST7032_SS_P1,
    #endif
    #if ST7032_N_LCDS > 2
        ST7032_SS_P2,
    #endif
    #if ST7032_N_LCDS > 3
        ST7032_SS_P3,
    #endif
    #if ST7032_N_LCDS > 4
        #error "numero de lcds no soportado"
    #endif
};
static _U32 gau8PinSS[ST7032_N_LCDS] =
{
    #if ST7032_N_LCDS > 0
        ST7032_SS_B0,
    #endif
    #if ST7032_N_LCDS > 1
        ST7032_SS_B1,
    #endif
    #if ST7032_N_LCDS > 2
        ST7032_SS_B2,
    #endif
    #if ST7032_N_LCDS > 3
        ST7032_SS_B3,
    #endif
    #if ST7032_N_LCDS > 4
        #error "numero de memorias no soportado"
    #endif
};
static _U32 gau8PortRST[ST7032_N_LCDS] =
{
    #if ST7032_N_LCDS > 0
        ST7032_RST_P0,
    #endif
    #if ST7032_N_LCDS > 1
        ST7032_RST_P1,
    #endif
    #if ST7032_N_LCDS > 2
        ST7032_RST_P2,
    #endif
    #if ST7032_N_LCDS > 3
        ST7032_RST_P3,
    #endif
    #if ST7032_N_LCDS > 4
        #error "numero de lcds no soportado"
    #endif
};
static _U32 gau8PinRST[ST7032_N_LCDS] =
{
    #if ST7032_N_LCDS > 0
        ST7032_RST_B0,
    #endif
    #if ST7032_N_LCDS > 1
        ST7032_RST_B1,
    #endif
    #if ST7032_N_LCDS > 2
        ST7032_RST_B2,
    #endif
    #if ST7032_N_LCDS > 3
        ST7032_RST_B3,
    #endif
    #if ST7032_N_LCDS > 4
        #error "numero de memorias no soportado"
    #endif
};
static _U32 gau8PortRS[ST7032_N_LCDS] =
{
    #if ST7032_N_LCDS > 0
        ST7032_RS_P0,
    #endif
    #if ST7032_N_LCDS > 1
        ST7032_RS_P1,
    #endif
    #if ST7032_N_LCDS > 2
        ST7032_RS_P2,
    #endif
    #if ST7032_N_LCDS > 3
        ST7032_RS_P3,
    #endif
    #if ST7032_N_LCDS > 4
        #error "numero de lcds no soportado"
    #endif
};
static _U32 gau8PinRS[ST7032_N_LCDS] =
{
    #if ST7032_N_LCDS > 0
        ST7032_RS_B0,
    #endif
    #if ST7032_N_LCDS > 1
        ST7032_RS_B1,
    #endif
    #if ST7032_N_LCDS > 2
        ST7032_RS_B2,
    #endif
    #if ST7032_N_LCDS > 3
        ST7032_RS_B3,
    #endif
    #if ST7032_N_LCDS > 4
        #error "numero de memorias no soportado"
    #endif
};

/*-- Private functions prototypes --*/


/*-- External functions --*/
/**-----------------------------------------------------------------------------------------------*/    
void ST7032_Init(const _U08 u8Lcd)
{
    Gpios_PinDirection(gau8PortSS[u8Lcd], gau8PinSS[u8Lcd], GPIOS_OUTPUT);
    Gpios_PinDirection(gau8PortRS[u8Lcd], gau8PinRS[u8Lcd], GPIOS_OUTPUT);
    Gpios_PinDirection(gau8PortRST[u8Lcd], gau8PinRST[u8Lcd], GPIOS_OUTPUT);
    Gpios_WritePin(gau8PortSS[u8Lcd], gau8PinSS[u8Lcd], 1); /* CS = 1 */
    Gpios_WritePin(gau8PortRST[u8Lcd], gau8PinRST[u8Lcd], 1); /*RST = 1*/
    Gpios_WritePin(gau8PortRST[u8Lcd], gau8PinRST[u8Lcd], 0); /*RST = 0*/
    Delays_ms(1);
    Gpios_WritePin(gau8PortRST[u8Lcd], gau8PinRST[u8Lcd], 1); /*RST = 1*/
    Delays_ms(50);
    ST7032_WriteCommand(u8Lcd, 0x30);      /*wake up*/
    Delays_ms(2);
    ST7032_WriteCommand(u8Lcd, 0x30);      /*wake up*/
    Delays_ms(40);
    ST7032_WriteCommand(u8Lcd, 0x30);      /*wake up*/
    Delays_ms(40);
    ST7032_WriteCommand(u8Lcd, 0x39);      /*function set*/
    Delays_ms(40);
    ST7032_WriteCommand(u8Lcd, 0x14);      /*internal osc fecuency*/
    Delays_ms(40);
    ST7032_WriteCommand(u8Lcd, 0x56);      /*power control*/
    Delays_ms(40);
    ST7032_WriteCommand(u8Lcd, 0x6d);      /*follower control*/
    Delays_ms(200);
    ST7032_WriteCommand(u8Lcd, 0x70);      /*contrast*/
    ST7032_WriteCommand(u8Lcd, 0x0c);      /*display on*/
    ST7032_WriteCommand(u8Lcd, 0x06);      /*entry mode*/
    ST7032_WriteCommand(u8Lcd, 0x01);      /*clear*/
    Delays_ms(1);
}
/**-----------------------------------------------------------------------------------------------*/    


/**-----------------------------------------------------------------------------------------------*/    
void ST7032_SetCursor(const _U08 u8Lcd, const _U08 u8Row, const _U08 u8Col)
{
    _U08 Address;

    if(u8Row != 1)
    {
        Address = 0x40;
    }
    else
    {
        Address = 0;
    }

    if(u8Col >= 1)
    {
        Address+= u8Col-1;
    }

    ST7032_WriteCommand(u8Lcd, 0x80|Address);
}
/**-----------------------------------------------------------------------------------------------*/    

/**-----------------------------------------------------------------------------------------------*/    
void ST7032_WriteData(const _U08 u8Lcd, const _U08 u8Data)
{
    Gpios_WritePin(gau8PortSS[u8Lcd], gau8PinSS[u8Lcd],0); /* CS = 0 */
    Gpios_WritePin(gau8PortRS[u8Lcd], gau8PinRS[u8Lcd], 1); /*RS = 1*/
    Spi_u8Transfer(gau8SpiPorts[u8Lcd], u8Data);
    Gpios_WritePin(gau8PortSS[u8Lcd], gau8PinSS[u8Lcd], 1); /* CS = 1 */
    Delays_10us(3);
}
/**-----------------------------------------------------------------------------------------------*/    

/**-----------------------------------------------------------------------------------------------*/
void ST7032_WriteCommand(const _U08 u8Lcd, const _U08 u8Cmd)
{
    Gpios_WritePin(gau8PortSS[u8Lcd], gau8PinSS[u8Lcd],0); /* CS = 0 */
    Gpios_WritePin(gau8PortRS[u8Lcd], gau8PinRS[u8Lcd], 0); /*RS = 0*/
    Spi_u8Transfer(gau8SpiPorts[u8Lcd], u8Cmd);
    Gpios_WritePin(gau8PortSS[u8Lcd], gau8PinSS[u8Lcd], 1); /* CS = 1 */
    Delays_10us(3);
}
/**-----------------------------------------------------------------------------------------------*/

/**-----------------------------------------------------------------------------------------------*/    
void ST7032_WriteString(const _U08 u8Lcd, const rom _S08 *strString)
{
    while(*strString != 0)
    {
        ST7032_WriteData(u8Lcd, *strString);
        strString++;
    }
}
/**-----------------------------------------------------------------------------------------------*/    

/*-- Private functions --*/
/**-------------------------------------------------------------------------------------------------    
  \fn         HD44780_SetDDRamAddr
  \brief      init the parallel master port internal registers 
  \param	  None
  \return     None
  \warning	  None   	
--------------------------------------------------------------------------------------------------*/


