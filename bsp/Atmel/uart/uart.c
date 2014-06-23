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
#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>
#include "hardware_profile.h"


/*-- Private Definitions--*/
#ifndef BSP_CLOCK
    #define UART_CLOCK              8000000
#else
    #define UART_CLOCK              BSP_CLOCK
#endif
#ifndef UART_ENABLE_RX0
    #define UART_ENABLE_RX0          0
#endif
#ifndef UART_ENABLE_RX1
    #define UART_ENABLE_RX1          0
#endif

typedef struct
{
    _BOOL bTxFlag;
    _U08 u8Counter;
    _U08 *pu8TxBuffer;
}_sUART;


/*-- Global variables --*/
static volatile unsigned char *gau8SPBRGH[2]  = {&UBRR0H, &UBRR1H};
static volatile unsigned char *gau8SPBRG[2]   = {&UBRR0L, &UBRR1L};
static volatile unsigned char *gau8UCSRA[2]   = {&UCSR0A, &UCSR1A};
static volatile unsigned char *gau8UCSRB[2]   = {&UCSR0B, &UCSR1B};
static volatile unsigned char *gau8UCSRC[2]   = {&UCSR0C, &UCSR1C};
static volatile unsigned char *gau8TXREG[2]   = {&UDR0, &UDR1};
static volatile unsigned char *gau8RCREG[2]   = {&UDR0, &UDR1};
static _sUART gasUarts[2];


/*-- Private Macros --*/


/*-- Private functions prototypes --*/
static _U16 u16BaudRateL(const _U08 u8Uart, _U32 u32BaudRate);
static _U16 u16BaudRateH(const _U08 u8Uart, _U32 u32BaudRate);
static _U32 u32GetBaudRate(const _U08 u8Uart);


/*-- External functions --*/
/**-----------------------------------------------------------------------------------------------*/
_U32 Uart_Init(const _U08 u8Uart, _U32 u32BaudRate)
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
    *gau8SPBRGH[u8Uart] = u16Baud>>(_U16)8;
    *gau8SPBRG[u8Uart] = (_U08)u16Baud;     /*set baud register*/
    
    *gau8UCSRC[u8Uart] = 0x06;
    SET_8BIT(*gau8UCSRB[u8Uart], 3);    /*trasnmitter enable*/

    #if UART_ENABLE_RX0 == 1
        SET_8BIT(*gau8UCSRB[u8Uart], 4);    /*receiver enable*/
        SET_8BIT(*gau8UCSRB[u8Uart], 7);    /*enable rx interrupt*/
    #endif

    #if UART_ENABLE_RX1 == 1
        SET_8BIT(*gau8UCSRB[u8Uart], 4);    /*receiver enable*/
        SET_8BIT(*gau8UCSRB[u8Uart], 7);    /*enable rx interrupt*/
    #endif

    return u32GetBaudRate(u8Uart);
}
/**-----------------------------------------------------------------------------------------------*/

/**-----------------------------------------------------------------------------------------------*/
void Uart_PutChar(const _U08 u8Uart, _U08 u8Char)
{
    while(QUERY_8BIT(*gau8UCSRA[u8Uart], 5)==0){}
    *gau8TXREG[u8Uart] = u8Char;
}
/**-----------------------------------------------------------------------------------------------*/

/**-----------------------------------------------------------------------------------------------*/
void Uart_PutString(const _U08 u8Uart, const _U08 *strString)
{
    while(*strString != 0)
    {
        Uart_PutChar(u8Uart, (_U08)*strString);
        strString++;
    }
}
/**-----------------------------------------------------------------------------------------------*/

/**-----------------------------------------------------------------------------------------------*/
void Uart_TxBuffer(const _U08 u8Uart, const _U08 *pu8Data, _U08 u8Lenght)
{
    if((gasUarts[u8Uart].bTxFlag == _FALSE) && (u8Lenght != 0u))
    {
        gasUarts[u8Uart].bTxFlag = _TRUE;
        gasUarts[u8Uart].u8Counter = u8Lenght;
        gasUarts[u8Uart].pu8TxBuffer = (_U08 *)pu8Data;
        *gau8TXREG[u8Uart] = *gasUarts[u8Uart].pu8TxBuffer;
        SET_8BIT(*gau8UCSRB[u8Uart], 6);    /*enable tx interrupt*/
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
ISR(USART0_TX_vect)
{
    gasUarts[0].u8Counter--;
    if(gasUarts[0].u8Counter != 0u)
    {
        gasUarts[0].pu8TxBuffer++;
        *gau8TXREG[0] = *gasUarts[0].pu8TxBuffer;
    }
    else
    {
        CLEAR_8BIT(*gau8UCSRB[0], 6);    /*enable tx interrupt*/
        gasUarts[0].bTxFlag = _FALSE;
    }
}
/**-----------------------------------------------------------------------------------------------*/

/**-----------------------------------------------------------------------------------------------*/
ISR(USART1_TX_vect)
{
    gasUarts[1].u8Counter--;
    if(gasUarts[1].u8Counter != 0u)
    {
        gasUarts[1].pu8TxBuffer++;
        *gau8TXREG[1] = *gasUarts[1].pu8TxBuffer;
    }
    else
    {
        CLEAR_8BIT(*gau8UCSRB[1], 6);   
        gasUarts[1].bTxFlag = _FALSE;
    }
}
/**-----------------------------------------------------------------------------------------------*/

/**-----------------------------------------------------------------------------------------------*/
ISR(USART0_RX_vect)
{
    #if UART_ENABLE_RX0 == 1
    Uart0_CallbackRx(*gau8RCREG[0]);
    #endif
}
/**-----------------------------------------------------------------------------------------------*/

/**-----------------------------------------------------------------------------------------------*/
ISR(USART1_RX_vect)
{
    #if UART_ENABLE_RX1 == 1
    Uart1_CallbackRx(*gau8RCREG[1]);
    #endif
}
/**-----------------------------------------------------------------------------------------------*/

/**-----------------------------------------------------------------------------------------------*/
_U08 Uart_u8GetChar(const _U08 u8Uart)
{
    while(QUERY_8BIT(*gau8UCSRA[u8Uart], 7) == 0);

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
static _U16 u16BaudRateL(const _U08 u8Uart, _U32 u32BaudRate)
{
    _U16 u16Baud;

    CLEAR_8BIT(*gau8UCSRA[u8Uart], 1);
    u16Baud = (_U16)((UART_CLOCK/(u32BaudRate*(_U32)16))-(_U32)1);
    return u16Baud;
}

/**-------------------------------------------------------------------------------------------------
  \brief        Regresa el calculo del baud rate con un preescaler de 4
  \param        u32BaudRate.- valor de baudrate deseado
  \return       None
  \warning      Valor del registro que ira en BAUDREG
--------------------------------------------------------------------------------------------------*/
static _U16 u16BaudRateH(const _U08 u8Uart, _U32 u32BaudRate)
{
    _U16 u16Baud;

    SET_8BIT(*gau8UCSRA[u8Uart], 1);
    u16Baud = (_U16)((UART_CLOCK/(u32BaudRate*(_U32)8))-(_U32)1);
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

    if(QUERY_8BIT(*gau8UCSRA[u8Uart], 1) == 0)
    {
        u32Mul = 16;
    }
    else
    {
        u32Mul = 8;
    }
    u32BaudReg = ((_U16)*gau8SPBRGH[u8Uart]<<(_U16)8)|(_U16)*gau8SPBRG[u8Uart];
    u32BaudRate = UART_CLOCK/(u32Mul * (u32BaudReg + 1));
    return u32BaudRate;
}

