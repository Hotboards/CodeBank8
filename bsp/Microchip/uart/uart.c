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
  \date         August 5, 2013
  \target       PIC18FJ series

  \brief        El driver de puerto serial maneja el periférico EUART para que realize transmisiones
                de 8 y 9 bits. El driver fue creado principalmente para que este funcione mediante
                interrupciones y no sea necesario ciclar al uC de manera innecesaria, sin embargo se
                crearon un par de funciones de tx de caracteres y cadenas, para cuando se requiera
                implementar una prueba rápida o si la aplicación no requiere demasiado procesamiento
                multitarea.
 -------------------------------------------------------------------------------------------------*/
/*-- Includes --*/
#include "uart.h"
#include <p18cxxx.h>
#include <string.h>
#include "hardware_profile.h"


/*-- Private Definitions--*/
#ifndef BSP_CLOCK
    #define UART_CLOCK              12000000
#else
    #define UART_CLOCK              BSP_CLOCK
#endif
#ifndef UART_ENABLE_RX1
    #define UART_ENABLE_RX1          0
#endif
#ifndef UART_ENABLE_RX2
    #define UART_ENABLE_RX2          0
#endif

typedef struct
{
    _BOOL bTxFlag;
    _U08 u8Counter;
    rom _U08 *pu8TxBufferFlash;
    _U08 *pu8TxBufferRam;
    _BOOL bRam;
}_sUART;


/*-- Global variables --*/
static volatile near unsigned char *gau8SPBRGH[3]  = {NULL, &SPBRGH1, &SPBRGH2};
static volatile near unsigned char *gau8SPBRG[3]   = {NULL, &SPBRG1, &SPBRG2};
static volatile near unsigned char *gau8TXSTA[3]   = {NULL, &TXSTA1, &TXSTA2};
static volatile near unsigned char *gau8RCSTA[3]   = {NULL, &RCSTA1, &RCSTA2};
static volatile near unsigned char *gau8TXREG[3]   = {NULL, &TXREG1, &TXREG2};
static volatile near unsigned char *gau8RCREG[3]   = {NULL, &RCREG1, &RCREG2};
static volatile near unsigned char *gau8BAUDCON[3] = {NULL, &BAUDCON1, &BAUDCON2};
static _sUART gasUarts[3];


/*-- Private Macros --*/


/*-- Private functions prototypes --*/
static _U16 u16BaudRateL(const _U08 u8Uart, const _U32 u32BaudRate);
static _U16 u16BaudRateH(const _U08 u8Uart, const _U32 u32BaudRate);
static _U32 u32GetBaudRate(const _U08 u8Uart);


/*-- External functions --*/
/**-----------------------------------------------------------------------------------------------*/
_U32 Uart_Init(const _U08 u8Uart, const _U32 u32BaudRate)
{
    _U16 u16Baud;

    u16Baud = u16BaudRateL(u8Uart, u32BaudRate);
    if(u16Baud < 10)
    {
        u16Baud = u16BaudRateH(u8Uart, u32BaudRate);
        if(u16Baud == 0)
        {
            return 0;
        }
    }
    
    gasUarts[u8Uart].bTxFlag = _FALSE;
    SET_8BIT(*gau8BAUDCON[u8Uart], 3);  /*set 16 bit baudreg mode*/
    *gau8SPBRGH[u8Uart] = u16Baud>>(_U16)8;
    *gau8SPBRG[u8Uart] = (_U08)u16Baud;     /*set baud register*/
    if(u8Uart == UART_PORT1)
    {
        CLEAR_8BIT(IPR1, 4);    /*set tx interrupt to low priority*/
    }
    else if(u8Uart == UART_PORT1)
    {
        CLEAR_8BIT(IPR3, 4);    /*set tx interrupt to low priority*/
    }
    CLEAR_8BIT(IPR3, 4);    /*set tx interrupt to low priority*/
    SET_8BIT(*gau8RCSTA[u8Uart], 7);    /*uart on*/
    SET_8BIT(*gau8TXSTA[u8Uart], 5);    /*trasnmitter enable*/

    #if UART_ENABLE_RX1 == 1
        SET_8BIT(*gau8RCSTA[u8Uart], 4);    /*enable rx*/
        SET_8BIT(PIE1, 5);      /*enable tx interrupt*/
        CLEAR_8BIT(IPR1, 5);    /*set rx interrupt to low priority*/
    #endif

    #if UART2_ENABLE_RX == 1
        SET_8BIT(*gau8RCSTA[u8Uart], 4);    /*enable rx*/
        SET_8BIT(PIE3, 5);      /*enable tx interrupt*/
        CLEAR_8BIT(IPR3, 5);    /*set rx interrupt to low priority*/
    #endif

    return u32GetBaudRate(u8Uart);
}
/**-----------------------------------------------------------------------------------------------*/

/**-----------------------------------------------------------------------------------------------*/
void Uart1_TxInterruptProprity(const _U08 u8Uart, const _BOOL bPriority)
{
    if((bPriority == _LOW) && (u8Uart == UART_PORT1))
    {
        CLEAR_8BIT(IPR1, 4);
    }
    else if((bPriority == _HIGH) && (u8Uart == UART_PORT1))
    {
        SET_8BIT(IPR1, 4);
    }
    else if((bPriority == _LOW) && (u8Uart == UART_PORT2))
    {
        CLEAR_8BIT(IPR3, 4);
    }
    else if((bPriority == _HIGH) && (u8Uart == UART_PORT2))
    {
        SET_8BIT(IPR3, 4);
    }
}
/**-----------------------------------------------------------------------------------------------*/

/**-----------------------------------------------------------------------------------------------*/
void Uart_RxInterruptProprity(const _U08 u8Uart, const _BOOL bPriority)
{
    if((bPriority == _LOW) && (u8Uart == UART_PORT1))
    {
        CLEAR_8BIT(IPR1, 5);
    }
    else if((bPriority == _HIGH) && (u8Uart == UART_PORT1))
    {
        SET_8BIT(IPR1, 5);
    }
    else if((bPriority == _LOW) && (u8Uart == UART_PORT2))
    {
        CLEAR_8BIT(IPR3, 5);
    }
    else if((bPriority == _HIGH) && (u8Uart == UART_PORT2))
    {
        SET_8BIT(IPR3, 5);
    }
}
/**-----------------------------------------------------------------------------------------------*/

/**-----------------------------------------------------------------------------------------------*/
void Uart_PutChar(const _U08 u8Uart, const _U08 u8Char)
{
    while(QUERY_8BIT(*gau8TXSTA[u8Uart], 1)==0){}
    *gau8TXREG[u8Uart] = u8Char;
}
/**-----------------------------------------------------------------------------------------------*/

/**-----------------------------------------------------------------------------------------------*/
void Uart_PutString(const _U08 u8Uart, const rom _S08 *strString)
{
    while(*strString != 0)
    {
        Uart_PutChar(u8Uart, (_U08)*strString);
        strString++;
    }
}
/**-----------------------------------------------------------------------------------------------*/

/**-----------------------------------------------------------------------------------------------*/
void Uart_TxBuffer(const _U08 u8Uart, const _U08 *pu8Data, const _U08 u8Lenght)
{
    if((gasUarts[u8Uart].bTxFlag == _FALSE) && (u8Lenght != 0u))
    {
        gasUarts[u8Uart].bTxFlag = _TRUE;
        gasUarts[u8Uart].u8Counter = u8Lenght;
        gasUarts[u8Uart].pu8TxBufferRam = (_U08 *)pu8Data;
        gasUarts[u8Uart].bRam = _TRUE;
        *gau8TXREG[u8Uart] = *gasUarts[u8Uart].pu8TxBufferRam;
        if(u8Uart == UART_PORT1)
        {
            SET_8BIT(PIE1, 4);
        }
        else if(u8Uart == UART_PORT2)
        {
            SET_8BIT(PIE3, 4);
        }
    }
}
/**-----------------------------------------------------------------------------------------------*/

/**-----------------------------------------------------------------------------------------------*/
void Uart_TxFlashBuffer(const _U08 u8Uart, const rom _U08 *pu8Data, const _U08 u8Lenght)
{
    if((gasUarts[u8Uart].bTxFlag == _FALSE) && (u8Lenght != 0u))
    {
        gasUarts[u8Uart].bTxFlag = _TRUE;
        gasUarts[u8Uart].u8Counter = u8Lenght;
        gasUarts[u8Uart].pu8TxBufferFlash = (rom _U08 *)pu8Data;
        gasUarts[u8Uart].bRam = _FALSE;
        *gau8TXREG[u8Uart] = *gasUarts[u8Uart].pu8TxBufferFlash;
        if(u8Uart == UART_PORT1)
        {
            SET_8BIT(PIE1, 4);
        }
        else if(u8Uart == UART_PORT2)
        {
            SET_8BIT(PIE3, 4);
        }
    }
}
/**-----------------------------------------------------------------------------------------------*/

/**-----------------------------------------------------------------------------------------------*/
_BOOL Uart_TxBusy(const _U08 u8Uart)
{
    return gasUarts[u8Uart].bTxFlag;
}
/**-----------------------------------------------------------------------------------------------*/

/**-----------------------------------------------------------------------------------------------*/
void Uart1_TxIsr(void)
{
    if((QUERY_8BIT(PIR1, 4) != 0u) && (gasUarts[1].bTxFlag == _TRUE)) /*enter this interrupt only if the int was cause by the EUSART tx*/
    {
        gasUarts[1].u8Counter--;
        if(gasUarts[1].u8Counter != 0u)
        {
            if(gasUarts[1].bRam == _TRUE)
            {
                gasUarts[1].pu8TxBufferRam++;
                *gau8TXREG[1] = *gasUarts[1].pu8TxBufferRam;
            }
            else
            {
                gasUarts[1].pu8TxBufferFlash++;
                *gau8TXREG[1] = *gasUarts[1].pu8TxBufferFlash;
            }
        }
        else
        {
            CLEAR_8BIT(PIE1, 4);
            gasUarts[1].bTxFlag = _FALSE;
        }
    }
}
/**-----------------------------------------------------------------------------------------------*/

/**-----------------------------------------------------------------------------------------------*/
void Uart2_TxIsr(void)
{
    if((QUERY_8BIT(PIR3, 4) != 0u) && (gasUarts[2].bTxFlag == _TRUE)) /*enter this interrupt only if the int was cause by the EUSART tx*/
    {
        gasUarts[2].u8Counter--;
        if(gasUarts[2].u8Counter != 0u)
        {
            if(gasUarts[2].bRam == _TRUE)
            {
                gasUarts[2].pu8TxBufferRam++;
                *gau8TXREG[2] = *gasUarts[2].pu8TxBufferRam;
            }
            else
            {
                gasUarts[2].pu8TxBufferFlash++;
                *gau8TXREG[2] = *gasUarts[2].pu8TxBufferFlash;
            }
        }
        else
        {
            CLEAR_8BIT(PIE3, 4);
            gasUarts[2].bTxFlag = _FALSE;
        }
    }
}
/**-----------------------------------------------------------------------------------------------*/

/**-----------------------------------------------------------------------------------------------*/
void Uart1_RxIsr(void)
{
    if(QUERY_8BIT(PIR1, 5) != 0u) /*enter this interrupt only if the int was cause by the EUSART rx*/
    {
    #if UART_ENABLE_RX1 == 1
        Uart1_CallbackRx(*gau8RCREG[1]);
    #endif
    }
}
/**-----------------------------------------------------------------------------------------------*/

/**-----------------------------------------------------------------------------------------------*/
void Uart2_RxIsr(void)
{
    if(QUERY_8BIT(PIR3, 5) != 0u) /*enter this interrupt only if the int was cause by the EUSART rx*/
    {
    #if UART_ENABLE_RX2 == 1
        Uart2_CallbackRx(*gau8RCREG[2]);
    #endif
    }
}
/**-----------------------------------------------------------------------------------------------*/

/**-----------------------------------------------------------------------------------------------*/
_U08 Uart_u8GetChar(const _U08 u8Uart)
{
    if(u8Uart == UART_PORT1)
    {
        while(QUERY_8BIT(PIR1, 5) == 0){}
    }
    else if(u8Uart == UART_PORT2)
    {
        while(QUERY_8BIT(PIR3, 5) == 0){}
    }

    return *gau8RCREG[u8Uart];
}
/**-----------------------------------------------------------------------------------------------*/

/*-- Private functions --*/
/**-------------------------------------------------------------------------------------------------    
  \brief        Regresa el calculo del baud rate con un preescaler de 16
  \param        u32BaudRate.- valor de baudrate deseado
  \return       None
  \warning      Valor del registro que ira en BAUDREG
--------------------------------------------------------------------------------------------------*/
static _U16 u16BaudRateL(const _U08 u8Uart, const _U32 u32BaudRate)
{
    _U16 u16Baud;

    CLEAR_8BIT(*gau8TXSTA[u8Uart], 2);
    u16Baud = (_U16)((UART_CLOCK/(u32BaudRate*(_U32)16))-(_U32)1);
    return u16Baud;
}

/**-------------------------------------------------------------------------------------------------
  \brief        Regresa el calculo del baud rate con un preescaler de 4
  \param        u32BaudRate.- valor de baudrate deseado
  \return       None
  \warning      Valor del registro que ira en BAUDREG
--------------------------------------------------------------------------------------------------*/
static _U16 u16BaudRateH(const _U08 u8Uart, const _U32 u32BaudRate)
{
    _U16 u16Baud;

    SET_8BIT(*gau8TXSTA[u8Uart], 2);
    u16Baud = (_U16)((UART_CLOCK/(u32BaudRate*(_U32)4))-(_U32)1);
    return u16Baud;
}

/**-------------------------------------------------------------------------------------------------
  \brief        Regresa el calculo del baud rate actual con los parametros establecidos
  \param        None
  \return       Baud rate actual generado
  \warning      Valor actual del baud rate generado
--------------------------------------------------------------------------------------------------*/
static _U32 u32GetBaudRate(const _U08 u8Uart)
{
    _U32 u32BaudRate;
    _U32 u32Mul;
    _U32 u32BaudReg;

    if(QUERY_8BIT(*gau8TXSTA[u8Uart], 2) == 0)
    {
        u32Mul = 16;
    }
    else
    {
        u32Mul = 4;
    }
    u32BaudReg = ((_U16)*gau8SPBRGH[u8Uart]<<(_U16)8)|(_U16)*gau8SPBRG[u8Uart];
    u32BaudRate = UART_CLOCK/(u32Mul * (u32BaudReg + 1));
    return u32BaudRate;
}

