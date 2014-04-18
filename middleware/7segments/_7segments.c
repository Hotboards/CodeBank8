/*--------------------------------------------------------------------------------------------------
  "THE BEER-WARE LICENSE" (Revision 42):
  <phk@FreeBSD.ORG> wrote this file. As long as you retain this notice you
  can do whatever you want with this stuff. If we meet some day, and you think
  this stuff is worth it, you can buy me a beer in return to Hotboards crew
  (Beer-ware license created by Poul-Henning Kamp)

  \file         _7segments.c
  \author       Diego
  \email        diego.perez@hotboards.org
  \ver          1.0
  \date         April 17, 2014
  \target       8-bits Generic

  \brief        Esta pieza de código controla el multiplexado de display de 7 segmentos mediante un
                bus de datos comun. El driver puede controlar de uno hasta cuatro display en
                configuracion anodo comun o catodo comun, requiere de un puerto completo para el bus
                de datos y un pin por cada display.
                Es necesario mandar llamar una fucion periodica al menos cada 5ms para 4 displays.
                Una simple funcion se encarga de setear los valores que se desplegaran por display, lo
                que pemrite desplegar cualquier caracter que se desee combiando los 7 segmentos
                (mas un punto), donde los segmentos a = bit0, b = bit1, ..., g = bit6 y . = bit7
                            a
                           ---
                        f | g | b
                           ---
                        e | d | c
                           --- .
 -------------------------------------------------------------------------------------------------*/
/*-- Includes --*/
#include "_7segments.h"
#include "gpios/gpios.h"
#include "hardware_profile.h"


/*-- Private Definitions--*/
#ifndef _7SEGMENTS_DIGI_N
    #error "Se debe definir al menos un display en el archivo hardware_profile.h"
//#define _7SEGMENTS_DIGI_N         1 definace como minimo en hardware_profile.h
#endif

#if _7SEGMENTS_DIGI_N > 4
    #error "El numero maximo de displays permitido es 4"
#endif

#ifndef _7SEGMENTS_MODE
    #define _7SEGMENTS_MODE                     0
#endif
#if _7SEGMENTS_MODE > 1
    #error "Los valores permitidos de _7SEGMENTS_MODE son 0 o 1"
#endif
#define DIGI_OFF                                ~_7SEGMENTS_MODE
#define DIGI_ON                                 _7SEGMENTS_MODE


/*-- Global variables --*/
static _U08 gau8Values[_7SEGMENTS_DIGI_N] = {0};
static const _U08 gua8Numbers[10] = {_0,_1,_2,_3,_4,_5,_6,_7,_8,_9};


/*-- Private Macros --*/


/*-- Private functions prototypes --*/


/*-- External functions --*/
/**-----------------------------------------------------------------------------------------------*/
void _7segments_Init(void)
{
    Gpios_WriteTris(_7SEGMENTS_PORT, 0x00);
    Gpios_PinDirection(_7SEGMENTS_DIGI0_P, _7SEGMENTS_DIGI0_B, GPIOS_OUTPUT);
    Gpios_WritePin(_7SEGMENTS_DIGI0_P, _7SEGMENTS_DIGI0_B, DIGI_OFF);
    #if _7SEGMENTS_DIGI_N > 1
        Gpios_PinDirection(_7SEGMENTS_DIGI1_P, _7SEGMENTS_DIGI1_B, GPIOS_OUTPUT);
        Gpios_WritePin(_7SEGMENTS_DIGI1_P, _7SEGMENTS_DIGI1_B, DIGI_OFF);
    #endif
    #if _7SEGMENTS_DIGI_N > 2
        Gpios_PinDirection(_7SEGMENTS_DIGI2_P, _7SEGMENTS_DIGI2_B, GPIOS_OUTPUT);
        Gpios_WritePin(_7SEGMENTS_DIGI2_P, _7SEGMENTS_DIGI2_B, DIGI_OFF);
    #endif
    #if _7SEGMENTS_DIGI_N > 3
        Gpios_PinDirection(_7SEGMENTS_DIGI3_P, _7SEGMENTS_DIGI3_B, GPIOS_OUTPUT);
        Gpios_WritePin(_7SEGMENTS_DIGI3_P, _7SEGMENTS_DIGI3_B, DIGI_OFF);
    #endif
}
/**-----------------------------------------------------------------------------------------------*/

/**-----------------------------------------------------------------------------------------------*/
void _7segments_SetDisplay(const _U08 u8Display, const _U08 u8Value)
{
    if(u8Display < _7SEGMENTS_DIGI_N)
    {
        #if _7SEGMENTS_MODE == 1
            gau8Values[u8Display] = ~u8Value;
        #else
            gau8Values[u8Display] = u8Value;
        #endif
    }
}
/**-----------------------------------------------------------------------------------------------*/

/**-----------------------------------------------------------------------------------------------*/
_U08 _7segments_u8GetDisplay(const _U08 u8Display)
{
    _U08 u8Value = 0;

    if(u8Display < _7SEGMENTS_DIGI_N)
    {
        #if _7SEGMENTS_MODE == 1
            u8Value = ~gau8Values[u8Display];
        #else
            u8Value = gau8Values[u8Display];
        #endif
    }
    return u8Value;
}
/**-----------------------------------------------------------------------------------------------*/

/**-----------------------------------------------------------------------------------------------*/
void _7segments_SetNumber(const _U16 u16Value)
{
    _U08 u8Digi;

    u8Digi = (_U08)(u16Value % (_U16)10); /*unidades*/
    _7segments_SetDisplay(0, gua8Numbers[u8Digi]);
    #if _7SEGMENTS_DIGI_N > 1
        u8Digi = ((u16Value / (_U16)10) % (_U16)10); /*decenas*/
        _7segments_SetDisplay(1, gua8Numbers[u8Digi]);
    #endif
    #if _7SEGMENTS_DIGI_N > 2
        u8Digi = (_U08)((u16Value / (_U16)100) % (_U16)10); /*centenas*/
        _7segments_SetDisplay(2, gua8Numbers[u8Digi]);
    #endif
    #if _7SEGMENTS_DIGI_N > 3
        u8Digi = (_U08)(u16Value / (_U16)1000); /*millares*/
        _7segments_SetDisplay(3, gua8Numbers[u8Digi]);
    #endif
}
/**-----------------------------------------------------------------------------------------------*/

/**-----------------------------------------------------------------------------------------------*/
void _7segments_Task(void)
{
    static _U08 u8States = 0;

    switch(u8States)
    {
        case 0:
            #if _7SEGMENTS_DIGI_N > 1
                Gpios_WritePin(_7SEGMENTS_DIGI1_P, _7SEGMENTS_DIGI1_B, DIGI_OFF);
            #endif
            #if _7SEGMENTS_DIGI_N > 2
                Gpios_WritePin(_7SEGMENTS_DIGI2_P, _7SEGMENTS_DIGI2_B, DIGI_OFF);
            #endif
            #if _7SEGMENTS_DIGI_N > 3
                Gpios_WritePin(_7SEGMENTS_DIGI3_P, _7SEGMENTS_DIGI3_B, DIGI_OFF);
            #endif
            Gpios_WritePin(_7SEGMENTS_DIGI0_P, _7SEGMENTS_DIGI0_B, DIGI_ON);
            Gpios_WritePort(_7SEGMENTS_PORT, gau8Values[0]);
            #if _7SEGMENTS_DIGI_N > 1
                u8States = 1;
            #else
                u8States = 0;
            #endif
        break;
        case 1:
            Gpios_WritePin(_7SEGMENTS_DIGI0_P, _7SEGMENTS_DIGI0_B, DIGI_OFF);
            #if _7SEGMENTS_DIGI_N > 1
                Gpios_WritePin(_7SEGMENTS_DIGI1_P, _7SEGMENTS_DIGI1_B, DIGI_ON);
            #endif
            Gpios_WritePort(_7SEGMENTS_PORT, gau8Values[1]);
            #if _7SEGMENTS_DIGI_N > 2
                u8States = 2;
            #else
                u8States = 0;
            #endif
        break;
        case 2:
            #if _7SEGMENTS_DIGI_N > 1
                Gpios_WritePin(_7SEGMENTS_DIGI1_P, _7SEGMENTS_DIGI1_B, DIGI_OFF);
            #endif
            #if _7SEGMENTS_DIGI_N > 2
                Gpios_WritePin(_7SEGMENTS_DIGI2_P, _7SEGMENTS_DIGI2_B, DIGI_ON);
            #endif
            Gpios_WritePort(_7SEGMENTS_PORT, gau8Values[2]);
            #if _7SEGMENTS_DIGI_N > 3
                u8States = 3;
            #else
                u8States = 0;
            #endif
        break;
        case 3:
            #if _7SEGMENTS_DIGI_N > 2
                Gpios_WritePin(_7SEGMENTS_DIGI2_P, _7SEGMENTS_DIGI2_B, DIGI_OFF);
            #endif
            #if _7SEGMENTS_DIGI_N > 3
                Gpios_WritePin(_7SEGMENTS_DIGI3_P, _7SEGMENTS_DIGI3_B, DIGI_ON);
            #endif
            Gpios_WritePort(_7SEGMENTS_PORT, gau8Values[3]);
            u8States = 0;
        break;
        default:
            u8States = 0;
        break;
    }
}
/**-----------------------------------------------------------------------------------------------*/


/*-- Private functions --*/
