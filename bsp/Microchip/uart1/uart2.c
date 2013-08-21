/*--------------------------------------------------------------------------------------------------
  "THE BEER-WARE LICENSE" (Revision 42):
  <phk@FreeBSD.ORG> wrote this file. As long as you retain this notice you
  can do whatever you want with this stuff. If we meet some day, and you think
  this stuff is worth it, you can buy me a beer in return to Hotboards crew 
  (Beer-ware license created by Poul-Henning Kamp)

  \file         uart2.c
  \author       Diego
  \email        diego.perez@hotboards.org
  \ver          1.0
  \date         August 20, 2013
  \target       PIC18FJ series

  \brief        El driver de puerto serial maneja el periférico EUART2 para que realize transmisiones
                de 8 y 9 bits. El driver fue creado principalmente para que este funcione mediante
                interrupciones y no sea necesario ciclar al uC de manera innecesaria, sin embargo se
                crearon un par de funciones de tx de caracteres y cadenas, para cuando se requiera
                implementar una prueba rápida o si la aplicación no requiere demasiado procesamiento
                multitarea.
 -------------------------------------------------------------------------------------------------*/
/*-- Includes --*/
#include "uart2.h"
#include <p18cxxx.h>
#include <string.h>
#include "bsp_profile.h"


/*-- Private Definitions--*/
#ifndef BSP_CLOCK
    #define UART2_CLOCK              12000000
#else
    #define UART2_CLOCK              BSP_CLOCK
#endif
#ifndef UART2_ENABLE_RX
    #define UART2_ENABLE_RX          0
#endif
#ifndef UART2_TX_BUFFER
    #define UART2_TX_BUFFER          30
#endif


/*-- Global variables --*/
static _BOOL bTxFlag = 0;
static _U08 guaTxBuffer[UART2_TX_BUFFER];
static _U08 gu8Index = UART2_TX_BUFFER;
static _U08 gu8Counter = 1;


/*-- Private Macros --*/
#define BAUD_RATE(baudrate)                     SPBRGH2 = ((baudrate)>>8); SPBRG2 = (baudrate)
#define TX_BUFFER_BUSY()                        (QUERY_8BIT(TXSTA2, 1))
#define TX_BUFFER(data)                         (TXREG2 = (data))
#define RX_BUFFER()                             (RCREG2)
#define ENABLE_TXINT()                          (SET_8BIT(PIE3, 4))
#define DISABLE_TXINT()                         (CLEAR_8BIT(PIE3, 4))


/*-- Private functions prototypes --*/
static _U16 u16BaudRateL(const _U32 u32BaudRate);
static _U16 u16BaudRateH(const _U32 u32BaudRate);
static _U32 u32GetBaudRate(void);


/*-- External functions --*/
/**-----------------------------------------------------------------------------------------------*/
_U32 Uart2_Init(const _U32 u32BaudRate)
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

    SET_8BIT(BAUDCON2, 3);  /*set 16 bit baudreg mode*/
    BAUD_RATE(u16Baud);     /*set baud register*/
    CLEAR_8BIT(IPR3, 4);    /*set tx interrupt to low priority*/
    SET_8BIT(RCSTA2, 7);    /*uart on*/
    SET_8BIT(TXSTA2, 5);    /*trasnmitter enable*/
    
    #if UART2_ENABLE_RX == 1
    SET_8BIT(RCSTA2, 4);    /*enable rx*/
    SET_8BIT(PIE3, 5);      /*enable tx interrupt*/
    CLEAR_8BIT(IPR3, 5);    /*set rx interrupt to low priority*/
    #endif
    return u32GetBaudRate();
}
/**-----------------------------------------------------------------------------------------------*/

/**-----------------------------------------------------------------------------------------------*/
void Uart2_TxInterruptProprity(const _BOOL bPriority)
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
void Uart2_RxInterruptProprity(const _BOOL bPriority)
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
void Uart2_PutChar(const _U08 u8Char)
{
    while(TX_BUFFER_BUSY()==0){}
    TX_BUFFER(u8Char);
}
/**-----------------------------------------------------------------------------------------------*/

/**-----------------------------------------------------------------------------------------------*/
void Uart2_PutString(const rom _S08 *strString)
{
    while(*strString != 0)
    {
        Uart2_PutChar((_U08)*strString);
        strString++;
    }
}
/**-----------------------------------------------------------------------------------------------*/

/**-----------------------------------------------------------------------------------------------*/
void Uart2_TxBuffer(const _U08 *pu8Data, const _U08 u8Lenght)
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
void Uart2_TxFlashBuffer(const rom _U08 *pu8Data, const _U08 u8Lenght)
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
_BOOL Uart2_TxBusy(void)
{
    return bTxFlag;
}
/**-----------------------------------------------------------------------------------------------*/

/**-----------------------------------------------------------------------------------------------*/
void Uart2_TxIsr(void)
{
    if((QUERY_8BIT(PIR3, 4) != 0u) && (bTxFlag == 1u)) /*enter this interrupt only if the int was cause by the EUSART tx*/
    {
        gu8Counter--;
        if((gu8Counter != 0u) && (gu8Index < UART2_TX_BUFFER))
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
void Uart2_RxIsr(void)
{
    if(QUERY_8BIT(PIR3, 5) != 0u) /*enter this interrupt only if the int was cause by the EUSART rx*/
    {
    #if UART2_ENABLE_RX == 1
        Uart2_CallbackRx(RX_BUFFER());
    #endif
    }
}
/**-----------------------------------------------------------------------------------------------*/

/**-----------------------------------------------------------------------------------------------*/
_U08 Uart2_u8GetChar(void)
{
    while(QUERY_8BIT(PIR3, 5) == 0){}
    return RX_BUFFER();
}
/**-----------------------------------------------------------------------------------------------*/

/*-- Private functions --*/
/**-------------------------------------------------------------------------------------------------    
  \brief        Regresa el calculo del baud rate con un preescaler de 16
  \param        u32BaudRate.- valor de baudrate deseado
  \return       None
  \warning      Valor del registro que ira en BAUDREG
--------------------------------------------------------------------------------------------------*/
static _U16 u16BaudRateL(const _U32 u32BaudRate)
{
    _U16 u16Baud;

    CLEAR_8BIT(TXSTA2, 2);
    u16Baud = (_U16)((UART2_CLOCK/(u32BaudRate*(_U32)16))-(_U32)1);
    return u16Baud;
}

/**-------------------------------------------------------------------------------------------------
  \brief        Regresa el calculo del baud rate con un preescaler de 4
  \param        u32BaudRate.- valor de baudrate deseado
  \return       None
  \warning      Valor del registro que ira en BAUDREG
--------------------------------------------------------------------------------------------------*/
static _U16 u16BaudRateH(const _U32 u32BaudRate)
{
    _U16 u16Baud;

    SET_8BIT(TXSTA2, 2);
    u16Baud = (_U16)((UART2_CLOCK/(u32BaudRate*(_U32)4))-(_U32)1);
    return u16Baud;
}

/**-------------------------------------------------------------------------------------------------
  \brief        Regresa el calculo del baud rate actual con los parametros establecidos
  \param        None
  \return       Baud rate actual generado
  \warning      Valor actual del baud rate generado
--------------------------------------------------------------------------------------------------*/
static _U32 u32GetBaudRate(void)
{
    _U32 u32BaudRate;
    _U32 u32Mul;
    _U32 u32BaudReg;

    if(QUERY_8BIT(TXSTA2, 2) == 0)u32Mul = 16;
    else u32Mul = 4;
    u32BaudReg = ((_U16)SPBRGH2<<(_U16)8)|(_U16)SPBRG2;
    u32BaudRate = UART2_CLOCK/(u32Mul * (u32BaudReg + 1));
    return u32BaudRate;
}

