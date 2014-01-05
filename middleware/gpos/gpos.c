/*--------------------------------------------------------------------------------------------------
  "THE BEER-WARE LICENSE" (Revision 42):
  <phk@FreeBSD.ORG> wrote this file. As long as you retain this notice you
  can do whatever you want with this stuff. If we meet some day, and you think
  this stuff is worth it, you can buy me a beer in return to Hotboards crew
  (Beer-ware license created by Poul-Henning Kamp)

  \file         gpos.c
  \author       Diego
  \email        diego.perez@hotboards.org
  \ver          1.0
  \date         July 25, 2013
  \target       8-bit uC

  \brief        El driver gpos permite tener en los pines digitales del micro configurados de manera
                que oscilen continuamente, se mantengan encendidas o apagadas por cierto tiempo o
                totalmente apagadas o encendidas sin cambiar nunca, este driver no utiliza ningún
                hardware del uC y solo es eficiente con periodos de tiempo mayores a 1ms..

                Example:
                #define BUZZER_SHORT()			Gpos_SetMode(0, GPOS_DELAYLOW_MODE, 5)
                #define BUZZER_LONG()			Gpos_SetMode(1, GPOS_DELAYLOW_MODE, 50)

                Gpos_Init()
                ............
                for(;;)
                {
                    Gpos_Task();
                }
                ..............................
                somewhere in code...
                BUZZER_SHORT();
 -------------------------------------------------------------------------------------------------*/
/*-- Includes --*/
#include "gpos.h"
#include "gpios/gpios.h"
#include "hardware_profile.h"


/*-- Private Definitions--*/
#ifndef GPOS_N_PINS
    #error "Se debe definir al menos una salida en el archivo hardware_profile.h"
    //#define GPOS_N_PINS         1 definace como minimo en hardware_profile.h
#endif

#define GPOS_BYTE_ARRAY()            (((_U08)GPOS_N_PINS - (_U08)1)/(_U08)8) + (_U08)1

typedef struct
{
    _U08 u8State;
    _U08 u8Counter;
    _U08 u8Shadow;
}_sGPOS_State;


/*-- Global variables --*/
static _U08 gua8Status[GPOS_BYTE_ARRAY()];
static _sGPOS_State gsGpoStates[GPOS_N_PINS];


/*-- Private Macros --*/
#if GPOS_N_PINS > 0
    #define GPOS_P0_HIGH()			Gpios_WritePin(GPOS_P0_P, GPOS_P0_B, 1u)
    #define GPOS_P0_LOW()			Gpios_WritePin(GPOS_P0_P, GPOS_P0_B, 0u)
    #define GPOS_P0_INIT()			Gpios_PinDirection(GPOS_P0_P, GPOS_P0_B, GPIOS_OUTPUT)
#endif
#if GPOS_N_PINS > 1
    #define GPOS_P1_HIGH()			Gpios_WritePin(GPOS_P1_P, GPOS_P1_B, 1u)
    #define GPOS_P1_LOW()			Gpios_WritePin(GPOS_P1_P, GPOS_P1_B, 0u)
    #define GPOS_P1_INIT()			Gpios_PinDirection(GPOS_P1_P, GPOS_P1_B, GPIOS_OUTPUT)
#endif
#if GPOS_N_PINS > 2
    #define GPOS_P2_HIGH()			Gpios_WritePin(GPOS_P2_P, GPOS_P2_B, 1u)
    #define GPOS_P2_LOW()			Gpios_WritePin(GPOS_P2_P, GPOS_P2_B, 0u)
    #define GPOS_P2_INIT()			Gpios_PinDirection(GPOS_P2_P, GPOS_P2_B, GPIOS_OUTPUT)
#endif
#if GPOS_N_PINS > 3
    #define GPOS_P3_HIGH()			Gpios_WritePin(GPOS_P3_P, GPOS_P3_B, 1u)
    #define GPOS_P3_LOW()			Gpios_WritePin(GPOS_P3_P, GPOS_P3_B, 0u)
    #define GPOS_P3_INIT()			Gpios_PinDirection(GPOS_P3_P, GPOS_P3_B, GPIOS_OUTPUT)
#endif
#if GPOS_N_PINS > 4
    #define GPOS_P4_HIGH()			Gpios_WritePin(GPOS_P4_P, GPOS_P4_B, 1u)
    #define GPOS_P4_LOW()			Gpios_WritePin(GPOS_P4_P, GPOS_P4_B, 0u)
    #define GPOS_P4_INIT()			Gpios_PinDirection(GPOS_P4_P, GPOS_P4_B, GPIOS_OUTPUT)
#endif
#if GPOS_N_PINS > 5
    #define GPOS_P5_HIGH()			Gpios_WritePin(GPOS_P5_P, GPOS_P5_B, 1u)
    #define GPOS_P5_LOW()			Gpios_WritePin(GPOS_P5_P, GPOS_P5_B, 0u)
    #define GPOS_P5_INIT()			Gpios_PinDirection(GPOS_P5_P, GPOS_P5_B, GPIOS_OUTPUT)
#endif
#if GPOS_N_PINS > 6
    #define GPOS_P6_HIGH()			Gpios_WritePin(GPOS_P6_P, GPOS_P6_B, 1u)
    #define GPOS_P6_LOW()			Gpios_WritePin(GPOS_P6_P, GPOS_P6_B, 0u)
    #define GPOS_P6_INIT()			Gpios_PinDirection(GPOS_P6_P, GPOS_P6_B, GPIOS_OUTPUT)
#endif
#if GPOS_N_PINS > 7
    #define GPOS_P7_HIGH()			Gpios_WritePin(GPOS_P7_P, GPOS_P7_B, 1u)
    #define GPOS_P7_LOW()			Gpios_WritePin(GPOS_P7_P, GPOS_P7_B, 0u)
    #define GPOS_P7_INIT()			Gpios_PinDirection(GPOS_P7_P, GPOS_P7_B, GPIOS_OUTPUT)
#endif
#if GPOS_N_PINS > 8
    #define GPOS_P8_HIGH()			Gpios_WritePin(GPOS_P8_P, GPOS_P8_B, 1u)
    #define GPOS_P8_LOW()			Gpios_WritePin(GPOS_P8_P, GPOS_P8_B, 0u)
    #define GPOS_P8_INIT()			Gpios_PinDirection(GPOS_P8_P, GPOS_P8_B, GPIOS_OUTPUT)
#endif
#if GPOS_N_PINS > 9
    #define GPOS_P9_HIGH()			Gpios_WritePin(GPOS_P9_P, GPOS_P9_B, 1u)
    #define GPOS_P9_LOW()			Gpios_WritePin(GPOS_P9_P, GPOS_P9_B, 0u)
    #define GPOS_P9_INIT()			Gpios_PinDirection(GPOS_P9_P, GPOS_P9_B, GPIOS_OUTPUT)
#endif
#if GPOS_N_PINS > 10
    #define GPOS_P10_HIGH()			Gpios_WritePin(GPOS_P10_P, GPOS_P10_B, 1u)
    #define GPOS_P10_LOW()			Gpios_WritePin(GPOS_P10_P, GPOS_P10_B, 0u)
    #define GPOS_P10_INIT()			Gpios_PinDirection(GPOS_P10_P, GPOS_P10_B, GPIOS_OUTPUT)
#endif
#if GPOS_N_PINS > 11
    #define GPOS_P11_HIGH()			Gpios_WritePin(GPOS_P11_P, GPOS_P11_B, 1u)
    #define GPOS_P11_LOW()			Gpios_WritePin(GPOS_P11_P, GPOS_P11_B, 0u)
    #define GPOS_P11_INIT()			Gpios_PinDirection(GPOS_P11_P, GPOS_P11_B, GPIOS_OUTPUT)
#endif
#if GPOS_N_PINS > 12
    #define GPOS_P12_HIGH()			Gpios_WritePin(GPOS_P12_P, GPOS_P12_B, 1u)
    #define GPOS_P12_LOW()			Gpios_WritePin(GPOS_P12_P, GPOS_P12_B, 0u)
    #define GPOS_P12_INIT()			Gpios_PinDirection(GPOS_P12_P, GPOS_P12_B, GPIOS_OUTPUT)
#endif
#if GPOS_N_PINS > 13
    #define GPOS_P13_HIGH()			Gpios_WritePin(GPOS_P13_P, GPOS_P13_B, 1u)
    #define GPOS_P13_LOW()			Gpios_WritePin(GPOS_P13_P, GPOS_P13_B, 0u)
    #define GPOS_P13_INIT()			Gpios_PinDirection(GPOS_P13_P, GPOS_P13_B, GPIOS_OUTPUT)
#endif
#if GPOS_N_PINS > 14
    #define GPOS_P14_HIGH()			Gpios_WritePin(GPOS_P14_P, GPOS_P14_B, 1u)
    #define GPOS_P14_LOW()			Gpios_WritePin(GPOS_P14_P, GPOS_P14_B, 0u)
    #define GPOS_P14_INIT()			Gpios_PinDirection(GPOS_P14_P, GPOS_P14_B, GPIOS_OUTPUT)
#endif
#if GPOS_N_PINS > 15
    #define GPOS_P15_HIGH()			Gpios_WritePin(GPOS_P15_P, GPOS_P15_B, 1u)
    #define GPOS_P15_LOW()			Gpios_WritePin(GPOS_P15_P, GPOS_P15_B, 0u)
    #define GPOS_P15_INIT()			Gpios_PinDirection(GPOS_P15_P, GPOS_P15_B, GPIOS_OUTPUT)
#endif
#if GPOS_N_PINS > 16
    #define GPOS_P16_HIGH()			Gpios_WritePin(GPOS_P16_P, GPOS_P16_B, 1u)
    #define GPOS_P16_LOW()			Gpios_WritePin(GPOS_P16_P, GPOS_P16_B, 0u)
    #define GPOS_P16_INIT()			Gpios_PinDirection(GPOS_P16_P, GPOS_P16_B, GPIOS_OUTPUT)
#endif
#if GPOS_N_PINS > 17
    #define GPOS_P17_HIGH()			Gpios_WritePin(GPOS_P17_P, GPOS_P17_B, 1u)
    #define GPOS_P17_LOW()			Gpios_WritePin(GPOS_P17_P, GPOS_P17_B, 0u)
    #define GPOS_P17_INIT()			Gpios_PinDirection(GPOS_P17_P, GPOS_P17_B, GPIOS_OUTPUT)
#endif
#if GPOS_N_PINS > 18
    #define GPOS_P18_HIGH()			Gpios_WritePin(GPOS_P18_P, GPOS_P18_B, 1u)
    #define GPOS_P18_LOW()			Gpios_WritePin(GPOS_P18_P, GPOS_P18_B, 0u)
    #define GPOS_P18_INIT()			Gpios_PinDirection(GPOS_P18_P, GPOS_P18_B, GPIOS_OUTPUT)
#endif
#if GPOS_N_PINS > 19
    #define GPOS_P19_HIGH()			Gpios_WritePin(GPOS_P19_P, GPOS_P19_B, 1u)
    #define GPOS_P19_LOW()			Gpios_WritePin(GPOS_P19_P, GPOS_P19_B, 0u)
    #define GPOS_P19_INIT()			Gpios_PinDirection(GPOS_P19_P, GPOS_P19_B, GPIOS_OUTPUT)
#endif
#if GPOS_N_PINS > 20
    #define GPOS_P20_HIGH()			Gpios_WritePin(GPOS_P20_P, GPOS_P20_B, 1u)
    #define GPOS_P20_LOW()			Gpios_WritePin(GPOS_P20_P, GPOS_P20_B, 0u)
    #define GPOS_P20_INIT()			Gpios_PinDirection(GPOS_P20_P, GPOS_P20_B, GPIOS_OUTPUT)
#endif
#if GPOS_N_PINS > 21
    #define GPOS_P21_HIGH()			Gpios_WritePin(GPOS_P21_P, GPOS_P21_B, 1u)
    #define GPOS_P21_LOW()			Gpios_WritePin(GPOS_P21_P, GPOS_P21_B, 0u)
    #define GPOS_P21_INIT()			Gpios_PinDirection(GPOS_P21_P, GPOS_P21_B, GPIOS_OUTPUT)
#endif
#if GPOS_N_PINS > 22
    #define GPOS_P22_HIGH()			Gpios_WritePin(GPOS_P22_P, GPOS_P22_B, 1u)
    #define GPOS_P22_LOW()			Gpios_WritePin(GPOS_P22_P, GPOS_P22_B, 0u)
    #define GPOS_P22_INIT()			Gpios_PinDirection(GPOS_P22_P, GPOS_P22_B, GPIOS_OUTPUT)
#endif
#if GPOS_N_PINS > 23
    #define GPOS_P23_HIGH()			Gpios_WritePin(GPOS_P23_P, GPOS_P23_B, 1u)
    #define GPOS_P23_LOW()			Gpios_WritePin(GPOS_P23_P, GPOS_P23_B, 0u)
    #define GPOS_P23_INIT()			Gpios_PinDirection(GPOS_P23_P, GPOS_P23_B, GPIOS_OUTPUT)
#endif


/*-- Private functions prototypes --*/
static void RefreshGpo(_U08 u8Gpo);


/*-- External functions --*/
/**-----------------------------------------------------------------------------------------------*/
void Gpos_Init(void)
{
    _U08 i;

    #if GPOS_N_PINS > 0
        GPOS_P0_INIT();
    #endif
    #if GPOS_N_PINS > 1
        GPOS_P1_INIT();
    #endif
    #if GPOS_N_PINS > 2
        GPOS_P2_INIT();
    #endif
    #if GPOS_N_PINS > 3
        GPOS_P3_INIT();
    #endif
    #if GPOS_N_PINS > 4
        GPOS_P4_INIT();
    #endif
    #if GPOS_N_PINS > 5
        GPOS_P5_INIT();
    #endif
    #if GPOS_N_PINS > 6
        GPOS_P6_INIT();
    #endif
    #if GPOS_N_PINS > 7
        GPOS_P7_INIT();
    #endif
    #if GPOS_N_PINS > 8
        GPOS_P8_INIT();
    #endif
    #if GPOS_N_PINS > 9
        GPOS_P9_INIT();
    #endif
    #if GPOS_N_PINS > 10
        GPOS_P10_INIT();
    #endif
    #if GPOS_N_PINS > 11
        GPOS_P11_INIT();
    #endif
    #if GPOS_N_PINS > 12
        GPOS_P12_INIT();
    #endif
    #if GPOS_N_PINS > 13
        GPOS_P13_INIT();
    #endif
    #if GPOS_N_PINS > 14
        GPOS_P14_INIT();
    #endif
    #if GPOS_N_PINS > 15
        GPOS_P15_INIT();
    #endif
    #if GPOS_N_PINS > 16
        GPOS_P16_INIT();
    #endif
    #if GPOS_N_PINS > 17
        GPOS_P17_INIT();
    #endif
    #if GPOS_N_PINS > 18
        GPOS_P18_INIT();
    #endif
    #if GPOS_N_PINS > 19
        GPOS_P19_INIT();
    #endif
    #if GPOS_N_PINS > 20
        GPOS_P20_INIT();
    #endif
    #if GPOS_N_PINS > 21
        GPOS_P21_INIT();
    #endif
    #if GPOS_N_PINS > 22
        GPOS_P22_INIT();
    #endif
    #if GPOS_N_PINS > 23
        GPOS_P23_INIT();
    #endif

    for(i=0;i<GPOS_N_PINS;i++)
    {
        gsGpoStates[i].u8State==GPOS_LOW_MODE;
    }
}
/**-----------------------------------------------------------------------------------------------*/

/**-----------------------------------------------------------------------------------------------*/
void Gpos_Task(void)
{
    _U08 i;

    for(i=0u;i<GPOS_N_PINS;i++)
    {
        if((gsGpoStates[i].u8State==GPOS_OSC_MODE)||(gsGpoStates[i].u8State==GPOS_DELAYHIGH_MODE)||(gsGpoStates[i].u8State==GPOS_DELAYLOW_MODE))
        {
            if(gsGpoStates[i].u8Counter == 0u)
            {
                TOGGLE_8BIT(gua8Status[i>>(_U08)3], i&(_U08)7);
                RefreshGpo(i);
                switch(gsGpoStates[i].u8State)
                {
                    case GPOS_OSC_MODE:
                        gsGpoStates[i].u8Counter = gsGpoStates[i].u8Shadow;
                    break;
                    case GPOS_DELAYHIGH_MODE:
                        gsGpoStates[i].u8State = GPOS_HIGH_MODE;
                    break;
                    case GPOS_DELAYLOW_MODE:
                        gsGpoStates[i].u8State = GPOS_LOW_MODE;
                    break;
                    default:
                        /* MISRA */
                    break;
                }
            }
            else
            {
                gsGpoStates[i].u8Counter--;
            }
        }
    }
}
/**-----------------------------------------------------------------------------------------------*/

/**-----------------------------------------------------------------------------------------------*/
void Gpos_SetMode(_U08 u8Gpo, _eGPOS_MODES eModes, _U08 u8Counter)
{
    gsGpoStates[u8Gpo].u8State = (_U08)eModes;

    if(gsGpoStates[u8Gpo].u8State == GPOS_OSC_MODE)
    {/*|-|_|-|_|-*/
    	gsGpoStates[u8Gpo].u8Counter = u8Counter--;
    	gsGpoStates[u8Gpo].u8Shadow = gsGpoStates[u8Gpo].u8Counter;
    	SET_8BIT(gua8Status[u8Gpo>>(_U08)3], u8Gpo&(_U08)7);
    }
    else if(gsGpoStates[u8Gpo].u8State == GPOS_DELAYLOW_MODE)
    {/*________|--------*/
    	gsGpoStates[u8Gpo].u8Counter = u8Counter--;
    	gsGpoStates[u8Gpo].u8Shadow = (_U08)0;
    	SET_8BIT(gua8Status[u8Gpo>>(_U08)3], u8Gpo&(_U08)7);
    }
    else if(gsGpoStates[u8Gpo].u8State == GPOS_DELAYHIGH_MODE)
    {/*--------|________*/
    	gsGpoStates[u8Gpo].u8Counter = u8Counter--;
    	gsGpoStates[u8Gpo].u8Shadow = (_U08)0;
    	CLEAR_8BIT(gua8Status[u8Gpo>>(_U08)3], u8Gpo&(_U08)7);
    }
    else if(gsGpoStates[u8Gpo].u8State == GPOS_HIGH_MODE)
    {/*----------------*/
    	gsGpoStates[u8Gpo].u8Counter = (_U08)0;
    	gsGpoStates[u8Gpo].u8Shadow = (_U08)0;
    	SET_8BIT(gua8Status[u8Gpo>>(_U08)3], u8Gpo&(_U08)7);
    }
    else if(gsGpoStates[u8Gpo].u8State == GPOS_LOW_MODE)
    {/*________________*/
    	gsGpoStates[u8Gpo].u8Counter = (_U08)0;
        gsGpoStates[u8Gpo].u8Shadow = (_U08)0;
    	CLEAR_8BIT(gua8Status[u8Gpo>>(_U08)3], u8Gpo&(_U08)7);
    }
    else
    {
    	/*MISRA*/
    }

    RefreshGpo(u8Gpo);
}
/**-----------------------------------------------------------------------------------------------*/


/*-- Private functions --*/
/**-------------------------------------------------------------------------------------------------
  \brief        Update the status that the output pin should have
  \param        None
  \return       None
  \warning      None
--------------------------------------------------------------------------------------------------*/
static void RefreshGpo(_U08 u8Gpo)
{
    switch(u8Gpo)
    {
    #if GPOS_N_PINS > 0
    case 0:
        if(gua8Status[0] & 0x01)/*1u == QUERY_8BIT(gua8Status[0],0)*/
        {
            GPOS_P0_HIGH();
        }
        else
        {
            GPOS_P0_LOW();
        }
    break;
    #endif
    #if GPOS_N_PINS > 1
    case 1:
        if(1u == QUERY_8BIT(gua8Status[0],1))
        {
            GPOS_P1_HIGH();
        }
        else
        {
            GPOS_P1_LOW();
        }
    break;
    #endif
    #if GPOS_N_PINS > 2
    case 2:
        if(1u == QUERY_8BIT(gua8Status[0],2))
        {
            GPOS_P2_HIGH();
        }
        else
        {
            GPOS_P2_LOW();
        }
    break;
    #endif
    #if GPOS_N_PINS > 3
    case 3:
        if(1u == QUERY_8BIT(gua8Status[0],3))
        {
            GPOS_P3_HIGH();
        }
        else
        {
            GPOS_P3_LOW();
        }
    break;
    #endif
    #if GPOS_N_PINS > 4
    case 4:
        if(1u == QUERY_8BIT(gua8Status[0],4))
        {
            GPOS_P4_HIGH();
        }
        else
        {
            GPOS_P4_LOW();
        }
    break;
    #endif
    #if GPOS_N_PINS > 5
    case 5:
        if(1u == QUERY_8BIT(gua8Status[0],5))
        {
            GPOS_P5_HIGH();
        }
        else
        {
            GPOS_P5_LOW();
        }
    break;
    #endif
    #if GPOS_N_PINS > 6
    case 6:
        if(1u == QUERY_8BIT(gua8Status[0],6))
        {
            GPOS_P6_HIGH();
        }
        else
        {
            GPOS_P6_LOW();
        }
    break;
    #endif
    #if GPOS_N_PINS > 7
    case 7:
        if(1u == QUERY_8BIT(gua8Status[0],7))
        {
            GPOS_P7_HIGH();
        }
        else
        {
            GPOS_P7_LOW();
        }
    break;
    #endif
    #if GPOS_N_PINS > 8
    case 8:
        if(1u == QUERY_8BIT(gua8Status[1],0))
        {
            GPOS_P8_HIGH();
        }
        else
        {
            GPOS_P8_LOW();
        }
    break;
    #endif
    #if GPOS_N_PINS > 9
    case 9:
        if(1u == QUERY_8BIT(gua8Status[1],1))
        {
            GPOS_P9_HIGH();
        }
        else
        {
            GPOS_P9_LOW();
        }
    break;
    #endif
    #if GPOS_N_PINS > 10
    case 10:
        if(1u == QUERY_8BIT(gua8Status[1],2))
        {
            GPOS_P10_HIGH();
        }
        else
        {
            GPOS_P10_LOW();
        }
    break;
    #endif
    #if GPOS_N_PINS > 11
    case 11:
        if(1u == QUERY_8BIT(gua8Status[1],3))
        {
            GPOS_P11_HIGH();
        }
        else
        {
            GPOS_P11_LOW();
        }
    break;
    #endif
    #if GPOS_N_PINS > 12
    case 12:
        if(1u == QUERY_8BIT(gua8Status[1],4))
        {
            GPOS_P12_HIGH();
        }
        else
        {
            GPOS_P12_LOW();
        }
    break;
    #endif
    #if GPOS_N_PINS > 13
    case 13:
        if(1u == QUERY_8BIT(gua8Status[1],5))
        {
            GPOS_P13_HIGH();
        }
        else
        {
            GPOS_P13_LOW();
        }
    break;
    #endif
    #if GPOS_N_PINS > 14
    case: 14
        if(1u == QUERY_8BIT(gua8Status[1],6))
        {
            GPOS_P14_HIGH();
        }
        else
        {
            GPOS_P14_LOW();
        }
    break;
    #endif
    #if GPOS_N_PINS > 15
    case: 15
        if(1u == QUERY_8BIT(gua8Status[1],7))
        {
            GPOS_P15_HIGH();
        }
        else
        {
            GPOS_P15_LOW();
        }
    break;
    #endif
    #if GPOS_N_PINS > 16
    case: 16
        if(1u == QUERY_8BIT(gua8Status[2],0))
        {
            GPOS_P16_HIGH();
        }
        else
        {
            GPOS_P16_LOW();
        }
    break;
    #endif
    #if GPOS_N_PINS > 17
    case: 17
        if(1u == QUERY_8BIT(gua8Status[2],1))
        {
            GPOS_P17_HIGH();
        }
        else
        {
            GPOS_P17_LOW();
        }
    break;
    #endif
    #if GPOS_N_PINS > 18
    case: 18
        if(1u == QUERY_8BIT(gua8Status[2],2))
        {
            GPOS_P18_HIGH();
        }
        else
        {
            GPOS_P18_LOW();
        }
    break;
    #endif
    #if GPOS_N_PINS > 19
    case: 19
        if(1u == QUERY_8BIT(gua8Status[2],3))
        {
            GPOS_P19_HIGH();
        }
        else
        {
            GPOS_P19_LOW();
        }
    break;
    #endif
    #if GPOS_N_PINS > 20
    case: 20
        if(1u == QUERY_8BIT(gua8Status[2],4))
        {
            GPOS_P20_HIGH();
        }
        else
        {
            GPOS_P20_LOW();
        }
    break;
    #endif
    #if GPOS_N_PINS > 21
    case: 21
        if(1u == QUERY_8BIT(gua8Status[2],5))
        {
            GPOS_P21_HIGH();
        }
        else
        {
            GPOS_P21_LOW();
        }
    break;
    #endif
    #if GPOS_N_PINS > 22
    case: 22
        if(1u == QUERY_8BIT(gua8Status[2],6))
        {
            GPOS_P22_HIGH();
        }
        else
        {
            GPOS_P22_LOW();
        }
    break;
    #endif
    #if GPOS_N_PINS > 23
    case: 23
        if(1u == QUERY_8BIT(gua8Status[2],7))
        {
            GPOS_P23_HIGH();
        }
        else
        {
            GPOS_P23_LOW();
        }
    break;
    #endif
    default:
    break;
}
}
