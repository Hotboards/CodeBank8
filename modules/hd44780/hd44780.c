/*--------------------------------------------------------------------------------------------------
  "THE BEER-WARE LICENSE" (Revision 42):
  <phk@FreeBSD.ORG> wrote this file. As long as you retain this notice you
  can do whatever you want with this stuff. If we meet some day, and you think
  this stuff is worth it, you can buy me a beer in return to Hotboards crew
  (Beer-ware license created by Poul-Henning Kamp)

  \file         hd44780.h
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
#include "hd44780.h"
#include "6800/_6800.h"
#include "delays/delays.h"
#include "hardware_profile.h"


/*-- Private Definitions--*/
#ifndef HD44780_ROWS
    #define HD44780_ROWS                    2
#endif

#ifndef HD44780_COLUMNS
    #define HD44780_COLUMNS                 16
#endif

/* Display ON/OFF Control defines */
#define DON         		(_U08)0b00001111  /* Display on      */
#define DOFF        		(_U08)0b00001011  /* Display off     */
#define CURSOR_ON   		(_U08)0b00001111  /* Cursor on       */
#define CURSOR_OFF  		(_U08)0b00001101  /* Cursor off      */
#define BLINK_ON    		(_U08)0b00001111  /* Cursor Blink    */
#define BLINK_OFF   		(_U08)0b00001110  /* Cursor No Blink */

/* Cursor or Display Shift defines */
#define SHIFT_CUR_LEFT    	(_U08)0b00000100  /* Cursor shifts to the left   */
#define SHIFT_CUR_RIGHT   	(_U08)0b00000101  /* Cursor shifts to the right  */
#define SHIFT_DISP_LEFT   	(_U08)0b00000110  /* Display shifts to the left  */
#define SHIFT_DISP_RIGHT  	(_U08)0b00000111  /* Display shifts to the right */

/* Function Set defines */
#define FOUR_BIT   		(_U08)0b00101100  /* 4-bit Interface               */
#define EIGHT_BIT  		(_U08)0b00111100  /* 8-bit Interface               */
#define LINE_5X7   		(_U08)0b00110000  /* 5x7 characters, single line   */
#define LINE_5X10  		(_U08)0b00110100  /* 5x10 characters               */
#define LINES_5X7  		(_U08)0b00111000  /* 5x7 characters, multiple line */


/*-- Global variables --*/
#if HD44780_ROWS == 1 &&  HD44780_COLUMNS == 16
	const _U08 au8Buffer[4]={0x00u,0x00u,0x00u,0x00u};
#elif HD44780_ROWS == 2 &&  HD44780_COLUMNS == 16
	const _U08 au8Buffer[4]={0x00u,0x40u,0x00u,0x00u};
#elif HD44780_ROWS == 4 &&  HD44780_COLUMNS == 16
	const _U08 au8Buffer[4]={0x00u,0x40u,0x14u,0x54u};
#elif HD44780_ROWS == 2 &&  HD44780_COLUMNS == 32
	const _U08 au8Buffer[4]={0x00u,0x40u,0x00u,0x00u};
#elif HD44780_ROWS == 2 &&  HD44780_COLUMNS == 20
	const _U08 au8Buffer[4]={0x00u,0x40u,0x00u,0x00u};
#elif HD44780_ROWS == 2 &&  HD44780_COLUMNS == 40
	const _U08 au8Buffer[4]={0x00u,0x40u,0x00u,0x00u};
#elif HD44780_ROWS == 4 &&  HD44780_COLUMNS == 20
	const _U08 au8Buffer[4]={0x00u,0x40u,0x14u,0x54u};
#else
	#error "configuration not supported"
#endif
	

/*-- Private Macros --*/


/*-- Private functions prototypes --*/


/*-- External functions --*/
/**-----------------------------------------------------------------------------------------------*/    
void HD44780_Init(void)
{
    _6800_Init();
    /*secuencia para incilizar controlador*/
    Delays_ms((_U08)15);                /* 15 ms para que el controlador encienda */
    _6800_WriteCommand((_U08)0x30);	/* Secuecia de encendido en modo 8 bits */
    Delays_ms((_U08)5);
    _6800_WriteCommand((_U08)0x30);
    Delays_10us((_U08)20);
    _6800_WriteCommand((_U08)0x30);
    Delays_10us((_U08)4);

    #if _6800_BUSLENGHT == 8
        _6800_WriteCommand(0x38);           /*modo 8 bits todas las lineas activas*/
        Delays_10us((_U08)4);
        _6800_WriteCommand(DOFF);           /*Se apaga el controlador*/
        Delays_10us((_U08)4);
        _6800_WriteCommand((_U08)0x01);     /*se enciende y limpia el display*/
        Delays_ms((_U08)2);
        _6800_WriteCommand(SHIFT_DISP_LEFT);/*incremento de cursor a la derecha*/
        Delays_10us((_U08)4);
        _6800_WriteCommand(DON & CURSOR_OFF & BLINK_OFF); /*se despliega cursor y parpadeo*/
        Delays_10us(4);
        _6800_WriteCommand((_U08)0x80);                 /* Set Display data ram address to 0 */
        Delays_10us(4);
    #elif _6800_BUSLENGHT == 4
        _6800_WriteCommand(0x20);
        Delays_10us((_U08)4);
        _6800_WriteCommand(0x20);           /*modo 8 bits todas las lineas activas*/
        _6800_WriteCommand(0x80);
        Delays_10us((_U08)4);
        _6800_WriteCommand(DOFF);           /*Se apaga el controlador*/
        _6800_WriteCommand(DOFF<<4u);
        Delays_10us((_U08)4);
        _6800_WriteCommand((_U08)0x01);     /*se enciende y limpia el display*/
        _6800_WriteCommand((_U08)0x01<<4u);
        Delays_ms((_U08)2);
        _6800_WriteCommand(SHIFT_DISP_LEFT);/*incremento de cursor a la derecha*/
        _6800_WriteCommand(SHIFT_DISP_LEFT<<4);
        Delays_10us((_U08)4);
        _6800_WriteCommand(DON & CURSOR_OFF & BLINK_OFF); /*se despliega cursor y parpadeo*/
        _6800_WriteCommand((DON & CURSOR_OFF & BLINK_OFF)<<4u);
        Delays_10us(4);
        _6800_WriteCommand((_U08)0x80);                 /* Set Display data ram address to 0 */
        _6800_WriteCommand((_U08)0x80<<4);
        Delays_10us(4);
    #endif
}
/**-----------------------------------------------------------------------------------------------*/    

/**-----------------------------------------------------------------------------------------------*/    
_BOOL HD44780_bBusyFlag(void)
{
    _U08 bFlag;
    #if _6800_BUSLENGHT == 8
        bFlag = (_BOOL)_6800_u8ReadAddr();
    #elif _6800_BUSLENGHT == 4
        bFlag = _6800_u8ReadAddr();
        bFlag |= (_6800_u8ReadAddr()>>4);
    #endif
    return QUERY_8BIT(bFlag, 7);
}
/**-----------------------------------------------------------------------------------------------*/    

/**-----------------------------------------------------------------------------------------------*/    
void HD44780_SetCursor(_U08 u8Row, _U08 u8Col)
{
    _U08 u8Add;
	
    if((u8Row < HD44780_ROWS) && (u8Col < HD44780_COLUMNS))
    {
        u8Add = au8Buffer[u8Row] + u8Col;
        #if _6800_BUSLENGHT == 8
            _6800_WriteCommand(0x80 | u8Add);
        #elif _6800_BUSLENGHT == 4
            _6800_WriteCommand(0x80 | u8Add);
            _6800_WriteCommand((0x80 | u8Add)<<4u);
        #endif
        Delays_10us(4);
    }
}
/**-----------------------------------------------------------------------------------------------*/    

/**-----------------------------------------------------------------------------------------------*/    
void HD44780_WriteData(_U08 u8Data)
{
    #if _6800_BUSLENGHT == 8
        _6800_WriteData(u8Data);
    #elif _6800_BUSLENGHT == 4
        _6800_WriteData(u8Data);
        _6800_WriteData(u8Data<<4);
    #endif
    Delays_10us(4);
}
/**-----------------------------------------------------------------------------------------------*/    

/**-----------------------------------------------------------------------------------------------*/
void HD44780_WriteCommand(_U08 u8Data)
{

    #if _6800_BUSLENGHT == 8
        _6800_WriteCommand(u8Data);
    #elif _6800_BUSLENGHT == 4
        _6800_WriteCommand(u8Data);
        _6800_WriteCommand(u8Data<<4);
    #endif
    Delays_10us(4);
}
/**-----------------------------------------------------------------------------------------------*/

/**-----------------------------------------------------------------------------------------------*/    
void HD44780_WriteString(const _U08 *strString)
{
    while(*strString != 0)
    {
        HD44780_WriteData(*strString);
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


