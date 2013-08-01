/*--------------------------------------------------------------------------------------------------
  "THE BEER-WARE LICENSE" (Revision 42):
  <phk@FreeBSD.ORG> wrote this file. As long as you retain this notice you
  can do whatever you want with this stuff. If we meet some day, and you think
  this stuff is worth it, you can buy me a beer in return to Hotboards crew 
  (Beer-ware license created by Poul-Henning Kamp)

  \file         uart.c
  \author       Diego
  \email        diego.perez@hotboards.org
  \ver          1.0
  \date         July 5, 2013
  \target       
  \brief        Write here a nice description about your driver.
 -------------------------------------------------------------------------------------------------*/
/*-- Includes --*/
#include "uart1.h"
#include <p18cxxx.h>
#include <string.h>
#include "bsp_profile.h"


/*-- Private Definitions--*/
#ifndef BSP_CLOCK
    #define UART1_CLOCK              12000000
#else
    #define UART1_CLOCK              BSP_CLOCK
#endif
#ifndef UART1_ENABLE_RX
    #define UART1_ENABLE_RX          0
#endif
#ifndef UART1_TX_BUFFER
    #define UART1_TX_BUFFER          30
#endif


/*-- Global variables --*/
static _BOOL bTxFlag = 0;
static _U08 guaTxBuffer[UART1_TX_BUFFER];
static _U08 gu8Index = UART1_TX_BUFFER;
static _U08 gu8Counter = 1;


/*-- Private Macros --*/
#define BAUD_RATE(baudrate)                     SPBRGH = ((baudrate)>>8); SPBRG = (baudrate)
#define TX_BUFFER_BUSY()                        (QUERY_8BIT(TXSTA, 1))
#define TX_BUFFER(data)                         (TXREG = (data))
#define RX_BUFFER()                             (RCREG)
#define ENABLE_TXINT()                          (SET_8BIT(PIE1, 4))
#define DISABLE_TXINT()                         (CLEAR_8BIT(PIE1, 4))


/*-- Private functions prototypes --*/
_U16 u16BaudRateL(const _U32 u32BaudRate);
_U16 u16BaudRateH(const _U32 u32BaudRate);
_U32 u32GetBaudRate(void);


/*-- External functions --*/
/**-----------------------------------------------------------------------------------------------*/
_U32 Uart1_Init(const _U32 u32BaudRate)
{
    _U16 u16Baud;
    u16Baud = u16BaudRateL(u32BaudRate);
    if(u16Baud < 10)
    {
        u16Baud = u16BaudRateH(u32BaudRate);
        if(u16Baud == 0)
        {
            return 0;
        }
    }

    SET_8BIT(BAUDCON, 3);  /*set 16 bit baudreg mode*/
    BAUD_RATE(u16Baud);     /*set baud register*/
    CLEAR_8BIT(IPR1, 4);    /*set tx interrupt to low priority*/
    SET_8BIT(RCSTA, 7);    /*uart on*/
    SET_8BIT(TXSTA, 5);    /*trasnmitter enable*/
    
    #if UART1_ENABLE_RX == 1
    SET_8BIT(RCSTA1, 4);    /*enable rx*/
    SET_8BIT(PIE1, 5);      /*enable tx interrupt*/
    CLEAR_8BIT(IPR1, 5);    /*set rx interrupt to low priority*/
    #endif
    return u32GetBaudRate();
}
/**-----------------------------------------------------------------------------------------------*/

/**-----------------------------------------------------------------------------------------------*/
void Uart1_TxInterruptProprity(const _BOOL bPriority)
{
    if(bPriority == 0)
    {
        CLEAR_8BIT(IPR1, 4);
    }
    else
    {
        SET_8BIT(IPR1, 4);
    }
}
/**-----------------------------------------------------------------------------------------------*/

/**-----------------------------------------------------------------------------------------------*/
void Uart1_RxInterruptProprity(const _BOOL bPriority)
{
    if(bPriority == 0)
    {
        CLEAR_8BIT(IPR1, 5);
    }
    else
    {
        SET_8BIT(IPR1, 5);
    }
}
/**-----------------------------------------------------------------------------------------------*/

/**-----------------------------------------------------------------------------------------------*/
void Uart1_PutChar(const _S08 u8Char)
{
    while(TX_BUFFER_BUSY()==0){}
    TX_BUFFER(u8Char);
}
/**-----------------------------------------------------------------------------------------------*/

/**-----------------------------------------------------------------------------------------------*/
void Uart1_PutString(const rom _S08 *strString)
{
    while(*strString != 0)
    {
        Uart1_PutChar(*strString);
        strString++;
    }
}
/**-----------------------------------------------------------------------------------------------*/

/**-----------------------------------------------------------------------------------------------*/
void Uart1_TxBuffer(const _U08 *pu8Data, const _U08 u8Lenght)
{
    if((bTxFlag == 0u)&&(u8Lenght != 0u))
    {
        bTxFlag = 1u;
        gu8Index = 0;
        gu8Counter = u8Lenght;
        (void)memcpy((void*)&guaTxBuffer[0], (const void*)pu8Data, u8Lenght);
        TX_BUFFER(guaTxBuffer[gu8Index]);
        ENABLE_TXINT();
    }
}
/**-----------------------------------------------------------------------------------------------*/

/**-----------------------------------------------------------------------------------------------*/
void Uart1_TxFlashBuffer(const rom _U08 *pu8Data, const _U08 u8Lenght)
{
    if((bTxFlag == 0u)&&(u8Lenght != 0u))
    {
        bTxFlag = 1u;
        gu8Index = 0;
        gu8Counter = u8Lenght;
        (void)memcpypgm2ram((void*)&guaTxBuffer[0], (const far rom void*)pu8Data, u8Lenght);
        TX_BUFFER(guaTxBuffer[gu8Index]);
        ENABLE_TXINT();
    }
}
/**-----------------------------------------------------------------------------------------------*/

/**-----------------------------------------------------------------------------------------------*/
_BOOL Uart1_TxBusy(void)
{
    return bTxFlag;
}
/**-----------------------------------------------------------------------------------------------*/

/**-----------------------------------------------------------------------------------------------*/
void Uart1_TxIsr(void)
{
    if((QUERY_8BIT(PIR1, 4) != 0u) && (bTxFlag == 1u)) /*enter this interrupt only if the int was cause by the TMR0 overflow*/
    {
        gu8Counter--;
        if((gu8Counter != 0u) && (gu8Index < UART1_TX_BUFFER))
        {
            gu8Index++;
            TX_BUFFER(guaTxBuffer[gu8Index]);
        }
        else
        {
            DISABLE_TXINT();
            bTxFlag = 0u;
        }
    }
}
/**-----------------------------------------------------------------------------------------------*/

/**-----------------------------------------------------------------------------------------------*/
void Uart1_RxIsr(void)
{
    if(QUERY_8BIT(PIR1, 5) != 0u) /*enter this interrupt only if the int was cause by the TMR0 overflow*/
    {
    #if UART1_ENABLE_RX == 1
        Uart1_CallbackRx(RX_BUFFER());
    #endif
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
_U16 u16BaudRateL(const _U32 u32BaudRate)
{
    _U16 u16Baud;

    CLEAR_8BIT(TXSTA, 2);
    u16Baud = (_U16)((UART1_CLOCK/(u32BaudRate*(_U32)16))-(_U32)1);
    return u16Baud;
}

/**-------------------------------------------------------------------------------------------------
  \brief        None
  \param        None
  \return       None
  \warning      None
--------------------------------------------------------------------------------------------------*/
_U16 u16BaudRateH(const _U32 u32BaudRate)
{
    _U16 u16Baud;

    SET_8BIT(TXSTA, 2);
    u16Baud = (_U16)((UART1_CLOCK/(u32BaudRate*(_U32)4))-(_U32)1);
    return u16Baud;
}

/**-------------------------------------------------------------------------------------------------
  \brief        None
  \param        None
  \return       None
  \warning      None
--------------------------------------------------------------------------------------------------*/
_U32 u32GetBaudRate(void)
{
    _U32 u32BaudRate;
    _U32 u32Mul;
    _U32 u32BaudReg;

    if(QUERY_8BIT(TXSTA, 2) == 0)u32Mul = 16;
    else u32Mul = 4;
    u32BaudReg = ((_U16)SPBRGH<<(_U16)8)|(_U16)SPBRG;
    u32BaudRate = UART1_CLOCK/(u32Mul * (u32BaudReg + 1));
    return u32BaudRate;
}

