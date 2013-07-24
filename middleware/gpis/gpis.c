/*--------------------------------------------------------------------------------------------------
  "THE BEER-WARE LICENSE" (Revision 42):
  <phk@FreeBSD.ORG> wrote this file. As long as you retain this notice you
  can do whatever you want with this stuff. If we meet some day, and you think
  this stuff is worth it, you can buy me a beer in return to Hotboards crew
  (Beer-ware license created by Poul-Henning Kamp)

  \file         gpis.c
  \author       Diego
  \email        diego.perez@hotboards.org
  \ver          1.0
  \date         July 17, 2013
  \target       8-bit uC (Generic)

  \brief        Esta pieza de código maneja la complejidad detrás de la activación de interruptores
                mecánicos, toma en cuenta la eliminación de rebotes, detección de flancos de subida,
                tiempo de presionado y el estado actual de cada botón.

                Example:
                Gpis_Init()
		............
		for(;;)
		{
                    Gpis_Task();
		}
                ..............................
                somewhere in code...
                if(Gpis_bGetInput(2)==1u)
                {
                   do something..........
                }
 -------------------------------------------------------------------------------------------------*/
/*-- Includes --*/
#include "gpis.h"
#include "gpios/gpios.h"
#include "middleware_profile.h"


/*-- Private Definitions--*/
#ifndef GPIS_N_INPUTS
    #error "Se debe definir al menos una entrada en el archivo middleware_profile.h"
    //#define GPIS_N_INPUTS         1 definace como minimo en middleware_profile.h
#endif

#if GPIS_N_INPUTS > 16
    #error "El numero maximo de entradas permitido es 16"
#endif

#ifndef GPIS_ON
    #define GPIS_ON                         0
#endif
#if GPIS_ON > 1
    #error "Los valores permitidos de GPIS_ON son 0 o 1"
#endif

#ifndef GPIS_DEBOUNCE
    #define GPIS_DEBOUNCE                   5
#endif
#if GPIS_DEBOUNCE < 3
    #error "El valor minimo permitido de GPIS_DEBOUNCE es de 3"
#endif

#ifndef GPIS_PULLUPS
    #define GPIS_PULLUPS                   0
#endif
#if GPIS_PULLUPS > 1
    #error "Los valores permitidos de GPIS_PULLUPS son 0 o 1"
#endif

#if GPIS_PULLUPS == 1
    #undef GPIS_ON
    #define GPIS_ON     0
#endif

#define GPIS_BIT_ARRAY					(_U08)(((GPIS_N_INPUTS - (_U08)1)/ (_U08)8) + (_U08)1)


/*-- Global variables --*/
static _U08 gau8Status[GPIS_BIT_ARRAY] = {0};
static _U08 gau8Flags[GPIS_BIT_ARRAY] = {0};
static _U08 gau8Activations[GPIS_BIT_ARRAY] = {0};
static _U08 gau8StatusShadow[GPIS_BIT_ARRAY] = {0};
static _U08 gau8Cancelation[GPIS_BIT_ARRAY] = {0};
static _U08 gau8Debounce[GPIS_N_INPUTS] = {0};
static _U08 gau8SetDebounce[GPIS_N_INPUTS] = {0};


/*-- Private Macros --*/
#if GPIS_N_INPUTS > 0
    #define GPIS_B0_INIT()                  Gpios_PinDirection(GPIS_B0_P, GPIS_B0_B, GPIOS_INPUT)
    #define GPIS_B0_PULLUP()                Gpios_PullupPin(GPIS_B0_P, GPIS_B0_B, 1)
    #define GPIS_B0()                       Gpios_bReadPin(GPIS_B0_P, GPIS_B0_B)
#endif
#if GPIS_N_INPUTS > 1
    #define GPIS_B1_INIT()                  Gpios_PinDirection(GPIS_B1_P,GPIS_B1_B, GPIOS_INPUT)
    #define GPIS_B1_PULLUP()            	Gpios_PullupPin(GPIS_B1_P, GPIS_B1_B, 1)
    #define GPIS_B1()                       Gpios_bReadPin(GPIS_B1_P,GPIS_B1_B)
#endif
#if GPIS_N_INPUTS > 2
    #define GPIS_B2_INIT()                  Gpios_PinDirection(GPIS_B2_P,GPIS_B2_B, GPIOS_INPUT)
    #define GPIS_B2_PULLUP()                Gpios_PullupPin(GPIS_B2_P, GPIS_B2_B, 1)
    #define GPIS_B2()                       Gpios_bReadPin(GPIS_B2_P,GPIS_B2_B)
#endif
#if GPIS_N_INPUTS > 3
    #define GPIS_B3_INIT()                  Gpios_PinDirection(GPIS_B3_P,GPIS_B3_B, GPIOS_INPUT)
    #define GPIS_B3_PULLUP()                Gpios_PullupPin(GPIS_B3_P, GPIS_B3_B, 1)
    #define GPIS_B3()                       Gpios_bReadPin(GPIS_B3_P,GPIS_B3_B)
#endif
#if GPIS_N_INPUTS > 4
    #define GPIS_B4_INIT()                  Gpios_PinDirection(GPIS_B4_P,GPIS_B4_B, GPIOS_INPUT)
    #define GPIS_B4_PULLUP()                Gpios_PullupPin(GPIS_B4_P, GPIS_B4_B, 1)
    #define GPIS_B4()                       Gpios_bReadPin(GPIS_B4_P,GPIS_B4_B)
#endif
#if GPIS_N_INPUTS > 5
    #define GPIS_B5_INIT()                  Gpios_PinDirection(GPIS_B5_P,GPIS_B5_B, GPIOS_INPUT)
    #define GPIS_B5_PULLUP()                Gpios_PullupPin(GPIS_B5_P, GPIS_B5_B, 1)
    #define GPIS_B5()                       Gpios_bReadPin(GPIS_B5_P,GPIS_B5_B)
#endif
#if GPIS_N_INPUTS > 6
    #define GPIS_B6_INIT()                  Gpios_PinDirection(GPIS_B6_P,GPIS_B6_B, GPIOS_INPUT)
    #define GPIS_B6_PULLUP()                Gpios_PullupPin(GPIS_B6_P, GPIS_B6_B, 1)
    #define GPIS_B6()                       Gpios_bReadPin(GPIS_B6_P,GPIS_B6_B)
#endif
#if GPIS_N_INPUTS > 7
    #define GPIS_B7_INIT()                  Gpios_PinDirection(GPIS_B7_P,GPIS_B7_B, GPIOS_INPUT)
    #define GPIS_B7_PULLUP()                Gpios_PullupPin(GPIS_B7_P, GPIS_B7_B, 1)
    #define GPIS_B7()                       Gpios_bReadPin(GPIS_B7_P,GPIS_B7_B)
#endif
#if GPIS_N_INPUTS > 8
    #define GPIS_B8_INIT()                  Gpios_PinDirection(GPIS_B8_P,GPIS_B8_B, GPIOS_INPUT)
    #define GPIS_B8_PULLUP()                Gpios_PullupPin(GPIS_B8_P, GPIS_B8_B, 1)
    #define GPIS_B8()                       Gpios_bReadPin(GPIS_B8_P,GPIS_B8_B)
#endif
#if GPIS_N_INPUTS > 9
    #define GPIS_B9_INIT()                  Gpios_PinDirection(GPIS_B9_P,GPIS_B9_B, GPIOS_INPUT)
    #define GPIS_B9_PULLUP()                Gpios_PullupPin(GPIS_B9_P, GPIS_B9_B, 1)
    #define GPIS_B9()                       Gpios_bReadPin(GPIS_B9_P,GPIS_B9_B)
#endif
#if GPIS_N_INPUTS > 10
    #define GPIS_B10_INIT()                 Gpios_PinDirection(GPIS_B10_P,GPIS_B10_B, GPIOS_INPUT)
    #define GPIS_B10_PULLUP()               Gpios_PullupPin(GPIS_B10_P, GPIS_B10_B, 1)
    #define GPIS_B10()                      Gpios_bReadPin(GPIS_B10_P,GPIS_B10_B)
#endif
#if GPIS_N_INPUTS > 11
    #define GPIS_B11_INIT()                 Gpios_PinDirection(GPIS_B11_P,GPIS_B11_B, GPIOS_INPUT)
    #define GPIS_B11_PULLUP()                   Gpios_PullupPin(GPIS_B11_P, GPIS_B11_B, 1)
    #define GPIS_B11()                          Gpios_bReadPin(GPIS_B11_P,GPIS_B11_B)
#endif
#if GPIS_N_INPUTS > 12
    #define GPIS_B12_INIT()                 Gpios_PinDirection(GPIS_B12_P,GPIS_B12_B, GPIOS_INPUT)
    #define GPIS_B12_PULLUP()               Gpios_PullupPin(GPIS_B12_P, GPIS_B12_B, 1)
    #define GPIS_B12()                      Gpios_bReadPin(GPIS_B12_P,GPIS_B12_B)
#endif
#if GPIS_N_INPUTS > 13
    #define GPIS_B13_INIT()                 Gpios_PinDirection(GPIS_B13_P,GPIS_B13_B, GPIOS_INPUT)
    #define GPIS_B13_PULLUP()               Gpios_PullupPin(GPIS_B13_P, GPIS_B13_B, 1)
    #define GPIS_B13()                      Gpios_bReadPin(GPIS_B13_P,GPIS_B13_B)
#endif
#if GPIS_N_INPUTS > 14
    #define GPIS_B14_INIT()                 Gpios_PinDirection(GPIS_B14_P,GPIS_B14_B, GPIOS_INPUT)
    #define GPIS_B14_PULLUP()               Gpios_PullupPin(GPIS_B14_P, GPIS_B14_B, 1)
    #define GPIS_B14()                      Gpios_bPinReadn(GPIS_B14_P,GPIS_B14_B)
#endif
#if GPIS_N_INPUTS > 15
    #define GPIS_B15_INIT()                 Gpios_PinDirection(GPIS_B15_P,GPIS_B15_B, GPIOS_INPUT)
    #define GPIS_B15_PULLUP()               Gpios_PullupPin(GPIS_B15_P, GPIS_B15_B, 1)
    #define GPIS_B15()                      Gpios_bReadPin(GPIS_B15_P,GPIS_B15_B)
#endif


/*-- Private functions prototypes --*/
static void read_hal(void);
static void cancelation(void);
static void debounce(void);
static void edge_detection(void);


/*-- External functions --*/
/**-----------------------------------------------------------------------------------------------*/
void Gpis_Init(void)
{
    _U08 i;

    #if GPIS_N_INPUTS > 0
        GPIS_B0_INIT();
        #if GPIS_PULLUPS == 1
            GPIS_B0_PULLUP();
        #endif
    #endif
    #if GPIS_N_INPUTS > 1
        GPIS_B1_INIT();
        #if GPIS_PULLUPS == 1
            GPIS_B1_PULLUP();
        #endif
    #endif
    #if GPIS_N_INPUTS > 2
        GPIS_B2_INIT();
        #if GPIS_PULLUPS == 1
            GPIS_B2_PULLUP();
        #endif
    #endif
    #if GPIS_N_INPUTS > 3
        GPIS_B3_INIT();
        #if GPIS_PULLUPS == 1
            GPIS_B3_PULLUP();
        #endif
    #endif
    #if GPIS_N_INPUTS > 4
        GPIS_B4_INIT();
        #if GPIS_PULLUPS == 1
            GPIS_B4_PULLUP();
        #endif
    #endif
    #if GPIS_N_INPUTS > 5
        GPIS_B5_INIT();
        #if GPIS_PULLUPS == 1
            GPIS_B5_PULLUP();
        #endif
    #endif
    #if GPIS_N_INPUTS > 6
        GPIS_B6_INIT();
        #if GPIS_PULLUPS == 1
            GPIS_B6_PULLUP();
        #endif
    #endif
    #if GPIS_N_INPUTS > 7
        GPIS_B7_INIT();
        #if GPIS_PULLUPS == 1
            GPIS_B7_PULLUP();
        #endif
    #endif
    #if GPIS_N_INPUTS > 8
        GPIS_B8_INIT();
        #if GPIS_PULLUPS == 1
            GPIS_B8_PULLUP();
        #endif
    #endif
    #if GPIS_N_INPUTS > 9
        GPIS_B9_INIT();
        #if GPIS_PULLUPS == 1
            GPIS_B9_PULLUP();
        #endif
    #endif
    #if  GPIS_N_INPUTS > 10
        GPIS_B10_INIT();
        #if GPIS_PULLUPS == 1
            GPIS_B10_PULLUP();
        #endif
    #endif
    #if GPIS_N_INPUTS > 11
        GPIS_B11_INIT();
        #if GPIS_PULLUPS == 1
            GPIS_B11_PULLUP();
        #endif
    #endif
    #if GPIS_N_INPUTS > 12
        GPIS_B12_INIT();
        #if GPIS_PULLUPS == 1
            GPIS_B12_PULLUP();
        #endif
    #endif
    #if GPIS_N_INPUTS > 13
        GPIS_B13_INIT();
        #if GPIS_PULLUPS == 1
            GPIS_B13_PULLUP();
        #endif
    #endif
    #if GPIS_N_INPUTS > 14
        GPIS_B14_INIT();
        #if GPIS_PULLUPS == 1
            GPIS_B14_PULLUP();
        #endif
    #endif
    #if GPIS_N_INPUTS > 15
        GPIS_B15_INIT();
        #if GPIS_PULLUPS == 1
            GPIS_B15_PULLUP();
        #endif
    #endif

    for(i=0u;i<(_U08)GPIS_N_INPUTS;i++)
    {
        if((_U08)GPIS_DEBOUNCE < 3)
        {
            gau8SetDebounce[i] = 3;
        }
        else
        {
            gau8SetDebounce[i] = (_U08)GPIS_DEBOUNCE;
        }
        gau8Debounce[i] = 1u;
    }
}
/**-----------------------------------------------------------------------------------------------*/

/**-----------------------------------------------------------------------------------------------*/
void Gpis_Task(void)
{
    read_hal();
    cancelation();
    debounce();
    edge_detection();
}
/**-----------------------------------------------------------------------------------------------*/

/**-----------------------------------------------------------------------------------------------*/
void Gpis_Enable(_U08 u8Gpi, _BOOL bStatus)
{
    bStatus &= 0x01;

    if(0u == bStatus)
    {
        SET_8BIT(gau8Cancelation[u8Gpi>>(_U08)3], u8Gpi&(_U08)7);
        CLEAR_8BIT(gau8Status[u8Gpi>>(_U08)3], u8Gpi&(_U08)7);
        CLEAR_8BIT(gau8Flags[u8Gpi>>(_U08)3], u8Gpi&(_U08)7);
    }
    else
    {
        CLEAR_8BIT(gau8Cancelation[u8Gpi>>(_U08)3], u8Gpi&(_U08)7);
        gau8Debounce[u8Gpi] = (_U08)1;
    }
}
/**-----------------------------------------------------------------------------------------------*/

/**-----------------------------------------------------------------------------------------------*/
void Gpis_Debounce(_U08 u8Gpi, _U08 u8Debounce)
{
    if((u8Gpi < GPIS_N_INPUTS) && (u8Debounce >= 3))
    {
        gau8SetDebounce[u8Gpi] = u8Debounce;
    }
}
/**-----------------------------------------------------------------------------------------------*/

/**-----------------------------------------------------------------------------------------------*/
_BOOL Gpis_bGetInput(_U08 u8Gpi)
{
    _BOOL bRes = 0;

    if(u8Gpi < GPIS_N_INPUTS)
    {
        bRes = QUERY_8BIT(gau8Flags[u8Gpi>>(_U08)3], u8Gpi&(_U08)7);
        if(bRes == 1u)
        {
            CLEAR_8BIT(gau8Flags[u8Gpi>>(_U08)3], u8Gpi&(_U08)7);
        }
    }

    return bRes;
}
/**-----------------------------------------------------------------------------------------------*/

/**-----------------------------------------------------------------------------------------------*/
_BOOL Gpis_bGetStatus(_U08 u8Gpi)
{
    _BOOL bRes = 0;
    if(u8Gpi < GPIS_N_INPUTS)
    {
        bRes = QUERY_8BIT(gau8Status[u8Gpi>>(_U08)3], u8Gpi&(_U08)7);
    }
    return bRes;
}
/**-----------------------------------------------------------------------------------------------*/


/*-- Private functions --*/
/**-------------------------------------------------------------------------------------------------
  \brief        read the input pins assigned to buttons
  \param        None
  \return       None
  \warning      None
--------------------------------------------------------------------------------------------------*/
static void read_hal(void)
{
    #if GPIS_N_INPUTS > 0
        if(GPIS_B0() == GPIS_ON)
        {
            gau8Activations[0] |= 0x01;/*SET_8BIT(gau8Activations[0], 0u);*/
        }
        else
        {
            gau8Activations[0] &= ~(0x01);/*CLEAR_8BIT(gau8Activations[0], 0u);*/
        }
    #endif
    #if GPIS_N_INPUTS > 1
        if(GPIS_B1() == GPIS_ON)
        {
            SET_8BIT(gau8Activations[0], 1u);
        }
        else
        {
            CLEAR_8BIT(gau8Activations[0], 1u);
        }
    #endif
    #if GPIS_N_INPUTS > 2
        if(GPIS_B2() == GPIS_ON)
        {
            SET_8BIT(gau8Activations[0], 2u);
        }
        else
        {
            CLEAR_8BIT(gau8Activations[0], 2u);
        }
    #endif
    #if GPIS_N_INPUTS > 3
        if(GPIS_B3() == GPIS_ON)
        {
            SET_8BIT(gau8Activations[0], 3u);
        }
        else
        {
            CLEAR_8BIT(gau8Activations[0], 3u);
        }
    #endif
    #if GPIS_N_INPUTS > 4
        if(GPIS_B4() == GPIS_ON)
        {
            SET_8BIT(gau8Activations[0], 4u);
        }
        else
        {
            CLEAR_8BIT(gau8Activations[0], 4u);
        }
    #endif
    #if GPIS_N_INPUTS > 5
        if(GPIS_B5() == GPIS_ON)
        {
            SET_8BIT(gau8Activations[0], 5u);
        }
        else
        {
            CLEAR_8BIT(gau8Activations[0], 5u);
        }
    #endif
    #if GPIS_N_INPUTS > 6
        if(GPIS_B6() == GPIS_ON)
        {
            SET_8BIT(gau8Activations[0], 6u);
        }
        else
        {
            CLEAR_8BIT(gau8Activations[0], 6u);
        }
    #endif
    #if GPIS_N_INPUTS > 7
        if(GPIS_B7() == GPIS_ON)
        {
            SET_8BIT(gau8Activations[0], 7u);
        }
        else
        {
            CLEAR_8BIT(gau8Activations[0], 7u);
        }
    #endif
    #if GPIS_N_INPUTS > 8
        if(GPIS_B8() == GPIS_ON)
        {
            SET_8BIT(gau8Activations[1], 0u);
        }
        else
        {
            CLEAR_8BIT(gau8Activations[1], 0u);
        }
    #endif
    #if GPIS_N_INPUTS > 9
        if(GPIS_B9() == GPIS_ON)
        {
            SET_8BIT(gau8Activations[1], 1u);
        }
        else
        {
            CLEAR_8BIT(gau8Activations[1], 1u);
        }
    #endif
    #if GPIS_N_INPUTS > 10
        if(GPIS_B10() == GPIS_ON)
        {
            SET_8BIT(gau8Activations[1], 2u);
        }
        else
        {
            CLEAR_8BIT(gau8Activations[1], 2u);
        }
    #endif
    #if GPIS_N_INPUTS > 11
        if(GPIS_B11() == GPIS_ON)
        {
            SET_8BIT(gau8Activations[1], 3u);
        }
        else
        {
            CLEAR_8BIT(gau8Activations[1], 3u);
        }
    #endif
    #if GPIS_N_INPUTS > 12
        if(GPIS_B12() == GPIS_ON)
        {
            SET_8BIT(gau8Activations[1], 4u);
        }
        else
        {
            CLEAR_8BIT(gau8Activations[1], 4u);
        }
    #endif
    #if GPIS_N_INPUTS > 13
        if(GPIS_B13() == GPIS_ON)
        {
            SET_8BIT(gau8Activations[1], 5u);
        }
        else
        {
            CLEAR_8BIT(gau8Activations[1], 5u);
        }
    #endif
    #if GPIS_N_INPUTS > 14
        if(GPIS_B14() == GPIS_ON)
        {
            SET_8BIT(gau8Activations[1], 6u);
        }
        else
        {
            CLEAR_8BIT(gau8Activations[1], 6u);
        }
    #endif
    #if GPIS_N_INPUTS > 15
        if(GPIS_B15() == GPIS_ON)
        {
            SET_8BIT(gau8Activations[1], 7u);
        }
        else
        {
            CLEAR_8BIT(gau8Activations[1], 7u);
        }
    #elif GPIS_N_INPUTS > 15
        #error "Number of buttons out of range"
    #endif
}

/**-------------------------------------------------------------------------------------------------
  \brief        if a buttons have been deactivated, seto to 0
  \param        None
  \return       None
  \warning      None
--------------------------------------------------------------------------------------------------*/
static void cancelation(void)
{
    _U08 i = (_U08)GPIS_N_INPUTS;

    while(i--)
    {
        if(QUERY_8BIT(gau8Cancelation[i>>(_U08)3], i&(_U08)7))
        {
            CLEAR_8BIT(gau8Activations[i>>(_U08)3], i&(_U08)7);
        }
    }
}

/**-------------------------------------------------------------------------------------------------
  \brief        debounce input for a giben period of executions
  \param        None
  \return       None
  \warning      None
--------------------------------------------------------------------------------------------------*/
static void debounce(void)
{
    _U08 i = (_U08)GPIS_N_INPUTS;

    while(i--)
    {
        if(QUERY_8BIT(gau8Activations[i>>(_U08)3], i&(_U08)7))
        {
            gau8Debounce[i]++;
            if(gau8Debounce[i] == gau8SetDebounce[i])
            {
                SET_8BIT(gau8Status[i>>(_U08)3], i&(_U08)7);
                gau8Debounce[i] = 3u;//gau8SetDebounce[i]-(_U08)1;
            }
        }
        else
        {
            gau8Debounce[i]--;
            if(gau8Debounce[i] < (_U08)1)
            {
                gau8Debounce[i] = (_U08)1;
                CLEAR_8BIT(gau8Status[i>>(_U08)3], i&(_U08)7);
            }
        }
    }
}

/**-------------------------------------------------------------------------------------------------
  \brief        rising edge detection, after debounce
  \param        None
  \return       None
  \warning      None
--------------------------------------------------------------------------------------------------*/
static void edge_detection(void)
{
    _U08 i = (_U08)GPIS_N_INPUTS;
    _BOOL bStatus, bShadow;

    while(i--)
    {
        bStatus = QUERY_8BIT(gau8Status[i>>(_U08)3], i&(_U08)7);
        bShadow = QUERY_8BIT(gau8StatusShadow[i>>(_U08)3], i&(_U08)7);
        if((bStatus==1u) && (bShadow==0u)) /*Rising Edge*/
        {
            SET_8BIT(gau8Flags[i>>(_U08)3], i&(_U08)7);
        }
        else if((bStatus==0u) && (bShadow==1u))/*Falling Edge*/
        {
            CLEAR_8BIT(gau8Flags[i>>(_U08)3], i&(_U08)7);
        }
        else
        {
            /*MISRA*/
        }
    }

    i = GPIS_BIT_ARRAY;
    while(i--)
    {
        gau8StatusShadow[i] = gau8Status[i];
    }
}
