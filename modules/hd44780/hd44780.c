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
  \date         June 23, 2014
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
#define lcdDON         		(_U08)0b00001111  /* Display on      */
#define lcdDOFF        		(_U08)0b00001011  /* Display off     */
#define lcdCURSOR_ON   		(_U08)0b00001111  /* Cursor on       */
#define lcdCURSOR_OFF  		(_U08)0b00001101  /* Cursor off      */
#define lcdBLINK_ON    		(_U08)0b00001111  /* Cursor Blink    */
#define lcdBLINK_OFF   		(_U08)0b00001110  /* Cursor No Blink */

/* Cursor or Display Shift defines */
#define lcdSHIFT_CUR_LEFT    	(_U08)0b00000100  /* Cursor shifts to the left   */
#define lcdSHIFT_CUR_RIGHT   	(_U08)0b00000101  /* Cursor shifts to the right  */
#define lcdSHIFT_DISP_LEFT   	(_U08)0b00000110  /* Display shifts to the left  */
#define lcdSHIFT_DISP_RIGHT  	(_U08)0b00000111  /* Display shifts to the right */

/* Function Set defines */
#define lcdFOUR_BIT   		(_U08)0b00101100  /* 4-bit Interface               */
#define lcdEIGHT_BIT  		(_U08)0b00111100  /* 8-bit Interface               */
#define lcdLINE_5X7   		(_U08)0b00110000  /* 5x7 characters, single line   */
#define lcdLINE_5X10  		(_U08)0b00110100  /* 5x10 characters               */
#define lcdLINES_5X7  		(_U08)0b00111000  /* 5x7 characters, multiple line */


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
    Delays_ms(15);                /* 15 ms para que el controlador encienda */
    _6800_WriteCommand(0x30);	/* Secuecia de encendido en modo 8 bits */
    Delays_ms(5);
    _6800_WriteCommand(0x30);
    Delays_ms(2);
    _6800_WriteCommand(0x30);
    Delays_ms(1);

    #if _6800_BUSLENGHT == 8
        HD44780_WriteCommand(0x38);              /*modo 8 bits todas las lineas activas*/
        HD44780_WriteCommand(lcdDOFF);           /*Se apaga el controlador*/
        HD44780_WriteCommand(0x01);              /*se enciende y limpia el display*/
        HD44780_WriteCommand(lcdSHIFT_DISP_LEFT);/*incremento de cursor a la derecha*/
        HD44780_WriteCommand(lcdDON & lcdCURSOR_OFF & lcdBLINK_OFF); /*se despliega cursor y parpadeo*/
        HD44780_WriteCommand(0x80);              /* Set Display data ram address to 0 */
    #elif _6800_BUSLENGHT == 4
        _6800_WriteCommand(0x20);                /*modo 4 bits todas las lineas activas*/
        while(HD44780_bBusyFlag() == 1);
        HD44780_WriteCommand(lcdDOFF);           /*Se apaga el controlador*/
        HD44780_WriteCommand(0x01);              /*se enciende y limpia el display*/
        HD44780_WriteCommand(lcdSHIFT_DISP_LEFT);/*incremento de cursor a la derecha*/
        HD44780_WriteCommand(lcdDON & lcdCURSOR_OFF & lcdBLINK_OFF); /*se despliega cursor y parpadeo*/
        HD44780_WriteCommand(0x80);              /* Set Display data ram address to 0 */
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
        while(HD44780_bBusyFlag() == 1);
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
    while(HD44780_bBusyFlag() == 1);
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
    while(HD44780_bBusyFlag() == 1);
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
  \param      None
  \return     None
  \warning    None
--------------------------------------------------------------------------------------------------*/
