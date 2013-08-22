/*--------------------------------------------------------------------------------------------------
  "THE BEER-WARE LICENSE" (Revision 42):
  <phk@FreeBSD.ORG> wrote this file. As long as you retain this notice you
  can do whatever you want with this stuff. If we meet some day, and you think
  this stuff is worth it, you can buy me a beer in return to Hotboards crew
  (Beer-ware license created by Poul-Henning Kamp)

  \file         keyboard.h
  \author       Diego
  \email        diego.perez@hotboards.org
  \ver          1.0
  \date         August 21, 2013
  \target       Generic 8-bits uC

  \brief        Este código permite manejar un teclado matricial de n x m, es decir la cantidad de
                lineas y columnas son configurables de 1 a 8, lo cual permite manejar un máximo de 64
                botones en el teclado. El driver se utiliza únicamente por poleo y es necesario mandar
                llamar una función periódica cada x tiempo. Ademas es configurable por el usuario el
                tiempo de debounce y los pines a los cuales se puede conectar.

                Example:

                Keyboard_Init();

                for(;;)
                {
                    Keyboard_Task();
                    if(Keyboard_bIsPresed()==1)
                    {
                        key = Keyboard_u8GetKey();
                        do something....
                    }
                }
--------------------------------------------------------------------------------------------------*/
/*-- Includes --*/
#include "keyboard.h"
#include "gpios/gpios.h"
#include "middleware_profile.h"


/*-- Private Definitions--*/
#ifndef KEYBOARD_DEBOUNCE
    #define KEYBOARD_DEBOUNCE                	5
#endif
#if KEYBOARD_DEBOUNCE < 3
    #error "El valor minimo permitido de GPIS_DEBOUNCE es de 3"
#endif

#ifndef KEYBOARD_PULLUPS
    #define KEYBOARD_PULLUPS                    0
#endif
#if KEYBOARD_PULLUPS > 1
    #error "Los valores permitidos de GPIS_PULLUPS son 0 o 1"
#endif

#ifndef KEYBOARD_ROWS
    #error "Se debe definir al menos una fila en el archivo middleware_profile.h"
    //#define KEYBOARD_ROWS                    	1
#endif
#ifndef KEYBOARD_COLUMNS
    #error "Se debe definir al menos una columna en el archivo middleware_profile.h"
    //#define KEYBOARD_COLUMNS                 	1
#endif

#define MASK_R0                                 (_U08)0xFE
#define MASK_R1                                 (_U08)0xFD
#define MASK_R2                                 (_U08)0xFB
#define MASK_R3                                 (_U08)0xF7
#define MASK_R4                                 (_U08)0xEF
#define MASK_R5                                 (_U08)0xDF
#define MASK_R6                                 (_U08)0xBF
#define MASK_R7                                 (_U08)0x7F
#define KEYBOARD_N_KEYS()                       ((_U08)KEYBOARD_ROWS * (_U08)KEYBOARD_COLUMNS)
#define KEYBOARD_BIT_ARRAY()			(_U08)(((KEYBOARD_N_KEYS() - (_U08)1)/ (_U08)8) + (_U08)1)


/*-- Global variables --*/
static const _U08 gua8RowCode[] = {MASK_R0,MASK_R1,MASK_R2,MASK_R3,MASK_R4,MASK_R5,MASK_R6,MASK_R7};
static _U08 gua8ColumnCode[KEYBOARD_ROWS] = {0};
static _U08 gua8Status[KEYBOARD_BIT_ARRAY()] = {0};
static _U08 gua8StatusShadow[KEYBOARD_BIT_ARRAY()] = {0};
static _U08 gua8Flags[KEYBOARD_BIT_ARRAY()] = {0};
static _U08 gua8Keys[KEYBOARD_BIT_ARRAY()] = {0};
static _U08 gua8Debounce[KEYBOARD_N_KEYS()] = {0};
static _U08 gau8SetDebounce[KEYBOARD_N_KEYS()] = {0};
static _U08 gau8Cancelation[KEYBOARD_BIT_ARRAY()] = {0};
static _BOOL gbKeyPressed = 0;
static _U08 gu8LastKeyPressed = 0;


/*-- Private Macros --*/
#if KEYBOARD_ROWS > 0
    #define KEYBOARD_ROW0_WRITE(data)           Gpios_WritePin(KEYBOARD_R0_P, KEYBOARD_R0_B, (data))
    #define KEYBOARD_ROW0_SET(data)		Gpios_PinDirection(KEYBOARD_R0_P, KEYBOARD_R0_B, (data))
#endif
#if KEYBOARD_ROWS > 1
    #define KEYBOARD_ROW1_WRITE(data)           Gpios_WritePin(KEYBOARD_R1_P, KEYBOARD_R1_B, (data))
    #define KEYBOARD_ROW1_SET(data)		Gpios_PinDirection(KEYBOARD_R1_P, KEYBOARD_R1_B, (data))
#endif
#if KEYBOARD_ROWS > 2
    #define KEYBOARD_ROW2_WRITE(data)           Gpios_WritePin(KEYBOARD_R2_P, KEYBOARD_R2_B, (data))
    #define KEYBOARD_ROW2_SET(data)		Gpios_PinDirection(KEYBOARD_R2_P, KEYBOARD_R2_B, (data))
#endif
#if KEYBOARD_ROWS > 3
    #define KEYBOARD_ROW3_WRITE(data)           Gpios_WritePin(KEYBOARD_R3_P, KEYBOARD_R3_B, (data))
    #define KEYBOARD_ROW3_SET(data)		Gpios_PinDirection(KEYBOARD_R3_P, KEYBOARD_R3_B, (data))
#endif
#if KEYBOARD_ROWS > 4
    #define KEYBOARD_ROW4_WRITE(data)           Gpios_WritePin(KEYBOARD_R4_P, KEYBOARD_R4_B, (data))
    #define KEYBOARD_ROW4_SET(data)		Gpios_PinDirection(KEYBOARD_R4_P, KEYBOARD_R4_B, (data))
#endif
#if KEYBOARD_ROWS > 5
    #define KEYBOARD_ROW5_WRITE(data)           Gpios_WritePin(KEYBOARD_R5_P, KEYBOARD_R5_B, (data))
    #define KEYBOARD_ROW5_SET(data)		Gpios_PinDirection(KEYBOARD_R5_P, KEYBOARD_R5_B, (data))
#endif
#if KEYBOARD_ROWS > 6
    #define KEYBOARD_ROW6_WRITE(data)           Gpios_WritePin(KEYBOARD_R6_P, KEYBOARD_R6_B, (data))
    #define KEYBOARD_ROW6_SET(data)		Gpios_PinDirection(KEYBOARD_R6_P, KEYBOARD_R6_B, (data))
#endif
#if KEYBOARD_ROWS > 7
    #define KEYBOARD_ROW7_WRITE(data)           Gpios_WritePin(KEYBOARD_R7_P, KEYBOARD_R7_B, (data))
    #define KEYBOARD_ROW7_SET(data)		Gpios_PinDirection(KEYBOARD_R7_P, KEYBOARD_R7_B, (data))
#endif
    
#if KEYBOARD_COLUMNS > 0
    #define KEYBOARD_COLUMN0_INIT()             Gpios_PinDirection(KEYBOARD_C0_P, KEYBOARD_C0_B, GPIOS_INPUT)
    #define KEYBOARD_COLUMN0_READ()             Gpios_bReadPin(KEYBOARD_C0_P, KEYBOARD_C0_B)
    #define KEYBOARD_PULLUP0()                  Gpios_PullupPin(KEYBOARD_C0_P, KEYBOARD_C0_B, 1)
#endif
#if KEYBOARD_COLUMNS > 1
    #define KEYBOARD_COLUMN1_INIT()             Gpios_PinDirection(KEYBOARD_C1_P, KEYBOARD_C1_B, GPIOS_INPUT)
    #define KEYBOARD_COLUMN1_READ()             Gpios_bReadPin(KEYBOARD_C1_P, KEYBOARD_C1_B)
    #define KEYBOARD_PULLUP1()                  Gpios_PullupPin(KEYBOARD_C1_P, KEYBOARD_C1_B, 1)
#endif
#if KEYBOARD_COLUMNS > 2
    #define KEYBOARD_COLUMN2_INIT()             Gpios_PinDirection(KEYBOARD_C2_P, KEYBOARD_C2_B, GPIOS_INPUT)
    #define KEYBOARD_COLUMN2_READ()             Gpios_bReadPin(KEYBOARD_C2_P, KEYBOARD_C2_B)
    #define KEYBOARD_PULLUP2()                  Gpios_PullupPin(KEYBOARD_C2_P, KEYBOARD_C2_B, 1)
#endif
#if KEYBOARD_COLUMNS > 3
    #define KEYBOARD_COLUMN3_INIT()             Gpios_PinDirection(KEYBOARD_C3_P, KEYBOARD_C3_B, GPIOS_INPUT)
    #define KEYBOARD_COLUMN3_READ()             Gpios_bReadPin(KEYBOARD_C3_P, KEYBOARD_C3_B)
    #define KEYBOARD_PULLUP3()                  Gpios_PullupPin(KEYBOARD_C3_P, KEYBOARD_C3_B, 1)
#endif
#if KEYBOARD_COLUMNS > 4
    #define KEYBOARD_COLUMN4_INIT()             Gpios_PinDirection(KEYBOARD_C4_P, KEYBOARD_C4_B, GPIOS_INPUT)
    #define KEYBOARD_COLUMN4_READ()             Gpios_bReadPin(KEYBOARD_C4_P, KEYBOARD_C4_B)
    #define KEYBOARD_PULLUP4()                  Gpios_PullupPin(KEYBOARD_C4_P, KEYBOARD_C4_B, 1)
#endif
#if KEYBOARD_COLUMNS > 5
    #define KEYBOARD_COLUMN5_INIT()             Gpios_PinDirection(KEYBOARD_C5_P, KEYBOARD_C5_B, GPIOS_INPUT)
    #define KEYBOARD_COLUMN5_READ()             Gpios_bReadPin(KEYBOARD_C5_P, KEYBOARD_C5_B)
    #define KEYBOARD_PULLUP5()                  Gpios_PullupPin(KEYBOARD_C5_P, KEYBOARD_C5_B, 1)
#endif
#if KEYBOARD_COLUMNS > 6
    #define KEYBOARD_COLUMN6_INIT()             Gpios_PinDirection(KEYBOARD_C6_P, KEYBOARD_C6_B, GPIOS_INPUT)
    #define KEYBOARD_COLUMN6_READ()             Gpios_bReadPin(KEYBOARD_C6_P, KEYBOARD_C6_B)
    #define KEYBOARD_PULLUP6()                  Gpios_PullupPin(KEYBOARD_C6_P, KEYBOARD_C6_B, 1)
#endif
#if KEYBOARD_COLUMNS > 7
    #define KEYBOARD_COLUMN7_INIT()             Gpios_PinDirection(KEYBOARD_C7_P, KEYBOARD_C7_B, GPIOS_INPUT)
    #define KEYBOARD_COLUMN7_READ()             Gpios_bReadPin(KEYBOARD_C7_P, KEYBOARD_C7_B)
    #define KEYBOARD_PULLUP7()                  Gpios_PullupPin(KEYBOARD_C7_P, KEYBOARD_C7_B, 1)
#endif


/*-- Private functions prototypes --*/
static void read_hal(void);
static void decode_inputs(void);
static void cancelation(void);
static void debounce(void);
static void edge_detection(void);


/*-- External functions --*/
/**-----------------------------------------------------------------------------------------------*/
void Keyboard_Init(void)
{
    _U08 i;
	
    #if KEYBOARD_COLUMNS > 0
        KEYBOARD_COLUMN0_INIT();
        #if KEYBOARD_PULLUPS == 1
            KEYBOARD_PULLUP0();
        #endif
    #endif
    #if KEYBOARD_COLUMNS > 1
        KEYBOARD_COLUMN1_INIT();
        #if KEYBOARD_PULLUPS == 1
            KEYBOARD_PULLUP1();
        #endif
    #endif
    #if KEYBOARD_COLUMNS > 2
        KEYBOARD_COLUMN2_INIT();
        #if KEYBOARD_PULLUPS == 1
            KEYBOARD_PULLUP2();
        #endif
    #endif
    #if KEYBOARD_COLUMNS > 3
        KEYBOARD_COLUMN3_INIT();
        #if KEYBOARD_PULLUPS == 1
            KEYBOARD_PULLUP3();
        #endif
    #endif
    #if KEYBOARD_COLUMNS > 4
        KEYBOARD_COLUMN4_INIT();
        #if KEYBOARD_PULLUPS == 1
            KEYBOARD_PULLUP4();
        #endif
    #endif
    #if KEYBOARD_COLUMNS > 5
        KEYBOARD_COLUMN5_INIT();
        #if KEYBOARD_PULLUPS == 1
            KEYBOARD_PULLUP5();
        #endif
    #endif
    #if KEYBOARD_COLUMNS > 6
        KEYBOARD_COLUMN6_INIT();
        #if KEYBOARD_PULLUPS == 1
            KEYBOARD_PULLUP6();
        #endif
    #endif
    #if KEYBOARD_COLUMNS > 7
        KEYBOARD_COLUMN7_INIT();
        #if KEYBOARD_PULLUPS == 1
            KEYBOARD_PULLUP7();
        #endif
    #endif
    
    
    for(i=0;i<KEYBOARD_N_KEYS();i++)
    {
        gua8Debounce[i] = 0u;
        gau8SetDebounce[i] = (_U08)KEYBOARD_DEBOUNCE;
    }
    
    gbKeyPressed = 0u;
}
/**-----------------------------------------------------------------------------------------------*/
    
/**-----------------------------------------------------------------------------------------------*/
void Keyboard_Task(void)
{
    read_hal();
    decode_inputs();
    cancelation();
    debounce();
    edge_detection();
}
/**-----------------------------------------------------------------------------------------------*/

/**-----------------------------------------------------------------------------------------------*/
void Keyboard_SetDebounce(const _U08 u8Key, const _U08 u8Debounce)
{
    if((u8Key < KEYBOARD_N_KEYS()) && (u8Debounce >= 3))
    {
        gau8SetDebounce[u8Key] = u8Debounce;
    }
}
/**-----------------------------------------------------------------------------------------------*/

/**-----------------------------------------------------------------------------------------------*/
_BOOL Keyboard_bIsPressed(void)
{
    return gbKeyPressed;
}
/**-----------------------------------------------------------------------------------------------*/

/**-----------------------------------------------------------------------------------------------*/
_U08 Keyboard_u8GetKey(void)
{
    gbKeyPressed = 0u;
    return gu8LastKeyPressed;
}
/**-----------------------------------------------------------------------------------------------*/

/**-----------------------------------------------------------------------------------------------*/
_BOOL Keyboard_u8GetStatus(const _U08 u8Key)
{
    _BOOL bRes = 0;
    if(u8Key < KEYBOARD_N_KEYS())
    {
        bRes = QUERY_8BIT(gua8Status[u8Key>>(_U08)3], u8Key&(_U08)7);
    }
    return bRes;
}
/**-----------------------------------------------------------------------------------------------*/


/*-- Private functions --*/
/**-------------------------------------------------------------------------------------------------    
  \brief    None
  \param    None
  \return   None
  \warning  None   	
--------------------------------------------------------------------------------------------------*/
static void read_hal(void)
{
    _U08 i;
    
    for(i=0u;i<KEYBOARD_ROWS;i++)
    {
    #if KEYBOARD_ROWS > 0
        if((gua8RowCode[i] & 1)==0)//if(QUERY_8BIT(gua8RowCode[i], 0)==0)
        {
            KEYBOARD_ROW0_WRITE(0);
            KEYBOARD_ROW0_SET(GPIOS_OUTPUT);
        }
        else
        {
            KEYBOARD_ROW0_WRITE(1);
            KEYBOARD_ROW0_SET(GPIOS_INPUT);
        }
    #endif
    #if KEYBOARD_ROWS > 1
        if(QUERY_8BIT(gua8RowCode[i], 1)==0)
        {
            KEYBOARD_ROW1_WRITE(0);
            KEYBOARD_ROW1_SET(GPIOS_OUTPUT);
        }
        else
        {
            KEYBOARD_ROW1_WRITE(1);
            KEYBOARD_ROW1_SET(GPIOS_INPUT);
        }
    #endif
    #if KEYBOARD_ROWS > 2
        if(QUERY_8BIT(gua8RowCode[i], 2)==0)
        {
            KEYBOARD_ROW2_WRITE(0);
            KEYBOARD_ROW2_SET(GPIOS_OUTPUT);
        }
        else
        {
            KEYBOARD_ROW2_WRITE(1);
            KEYBOARD_ROW2_SET(GPIOS_INPUT);
        }
    #endif
    #if KEYBOARD_ROWS > 3
        if(QUERY_8BIT(gua8RowCode[i], 3)==0)
        {
            KEYBOARD_ROW3_WRITE(0);
            KEYBOARD_ROW3_SET(GPIOS_OUTPUT);
        }
        else
        {
            KEYBOARD_ROW3_WRITE(1);
            KEYBOARD_ROW3_SET(GPIOS_INPUT);
        }
    #endif
    #if KEYBOARD_ROWS > 4
        if(QUERY_8BIT(gua8RowCode[i], 4)==0)
        {
            KEYBOARD_ROW4_WRITE(0);
            KEYBOARD_ROW4_SET(GPIOS_OUTPUT);
        }
        else
        {
            KEYBOARD_ROW4_WRITE(1);
            KEYBOARD_ROW4_SET(GPIOS_INPUT);
        }
    #endif
    #if KEYBOARD_ROWS > 5
        if(QUERY_8BIT(gua8RowCode[i], 5)==0)
        {
            KEYBOARD_ROW5_WRITE(0);
            KEYBOARD_ROW5_SET(GPIOS_OUTPUT);
        }
        else
        {
            KEYBOARD_ROW5_WRITE(1);
            KEYBOARD_ROW5_SET(GPIOS_INPUT);
        }
    #endif
    #if KEYBOARD_ROWS > 6
        if(QUERY_8BIT(gua8RowCode[i], 6)==0)
        {
            KEYBOARD_ROW6_WRITE(0);
            KEYBOARD_ROW6_SET(GPIOS_OUTPUT);
        }
        else
        {
            KEYBOARD_ROW6_WRITE(1);
            KEYBOARD_ROW6_SET(GPIOS_INPUT);
        }
    #endif
    #if KEYBOARD_ROWS > 7
        if(QUERY_8BIT(gua8RowCode[i], 7)==0)
        {
            KEYBOARD_ROW7_WRITE(0);
            KEYBOARD_ROW7_SET(GPIOS_OUTPUT);
        }
        else
        {
            KEYBOARD_ROW7_WRITE(1);
            KEYBOARD_ROW7_SET(GPIOS_INPUT);
        }
    #endif
        
    #if KEYBOARD_COLUMNS > 0
        if(KEYBOARD_COLUMN0_READ() == 1u)
        {
            gua8ColumnCode[i] |= 1;//SET_8BIT(gua8ColumnCode[i], 0);
        }
        else
        {
            gua8ColumnCode[i] &= ~1;//CLEAR_8BIT(gua8ColumnCode[i], 0);
        }
    #endif
    #if KEYBOARD_COLUMNS > 1
        if(KEYBOARD_COLUMN1_READ() == 1u)
        {
            SET_8BIT(gua8ColumnCode[i], 1);
        }
        else
        {
            CLEAR_8BIT(gua8ColumnCode[i], 1);
        }
    #endif
    #if KEYBOARD_COLUMNS > 2
        if(KEYBOARD_COLUMN2_READ() == 1u)
        {
            SET_8BIT(gua8ColumnCode[i], 2);
        }
        else
        {
            CLEAR_8BIT(gua8ColumnCode[i], 2);
        }
    #endif
    #if KEYBOARD_COLUMNS > 3
        if(KEYBOARD_COLUMN3_READ() == 1u)
        {
            SET_8BIT(gua8ColumnCode[i], 3);
        }
        else
        {
            CLEAR_8BIT(gua8ColumnCode[i], 3);
        }
    #endif
    #if KEYBOARD_COLUMNS > 4
        if(KEYBOARD_COLUMN4_READ() == 1u)
        {
            SET_8BIT(gua8ColumnCode[i], 4);
        }
        else
        {
            CLEAR_8BIT(gua8ColumnCode[i], 4);
        }
    #endif
    #if KEYBOARD_COLUMNS > 5
         if(KEYBOARD_COLUMN5_READ() == 1u)
         {
            SET_8BIT(gua8ColumnCode[i], 5);
         }
         else
         {
            CLEAR_8BIT(gua8ColumnCode[i], 5);
         }
    #endif
    #if KEYBOARD_COLUMNS > 6
        if(KEYBOARD_COLUMN6_READ() == 1u)
        {
            SET_8BIT(gua8ColumnCode[i], 6);
        }
        else
        {
            CLEAR_8BIT(gua8ColumnCode[i], 6);
        }
    #endif
    #if KEYBOARD_COLUMNS > 7
        if(KEYBOARD_COLUMN7_READ() == 1u)
        {
            SET_8BIT(gua8ColumnCode[i], 7);
        }
        else
        {
            CLEAR_8BIT(gua8ColumnCode[i], 7);
        }
    #endif
    }
}

/**-------------------------------------------------------------------------------------------------    
  \brief    None
  \param    None
  \return   None
  \warning  None   	
--------------------------------------------------------------------------------------------------*/
static void decode_inputs(void)
{
    _U08 i;
    _U08 j;
    _U08 u8Key;
    
    for(i=0u;i<KEYBOARD_ROWS;i++)
    {
        for(j=0u;j<KEYBOARD_COLUMNS;j++)
        {
            u8Key = (i + (j * KEYBOARD_ROWS));
            if(QUERY_8BIT(gua8ColumnCode[i], j)==0u)
            {
                SET_8BIT(gua8Keys[u8Key >> (_U08)3], (u8Key & (_U08)7));
            }
            else
            {   
                CLEAR_8BIT(gua8Keys[u8Key >> (_U08)3], (u8Key & (_U08)7));
            }
        }
    }
}

/**-------------------------------------------------------------------------------------------------
  \brief        if a buttons have been deactivated, seto to 0
  \param        None
  \return       None
  \warning      None
--------------------------------------------------------------------------------------------------*/
static void cancelation(void)
{
    _U08 i = KEYBOARD_N_KEYS();

    while(i--)
    {
        if(QUERY_8BIT(gau8Cancelation[i>>(_U08)3], i&(_U08)7))
        {
            CLEAR_8BIT(gua8Keys[i>>(_U08)3], i&(_U08)7);
        }
    }
}

/**-------------------------------------------------------------------------------------------------    
  \brief    None
  \param    None
  \return   None
  \warning  None   	
--------------------------------------------------------------------------------------------------*/
static void debounce(void)
{
    _U08 i = KEYBOARD_N_KEYS();
        
    while(i--)
    {
        if(QUERY_8BIT(gua8Keys[i >> (_U08)3], (i & (_U08)7)))
        {
            gua8Debounce[i]++;
            if(gua8Debounce[i] == gau8SetDebounce[i])//KEYBOARD_DEBOUNCE)
            {
                SET_8BIT(gua8Status[i >> (_U08)3], (i & (_U08)7));
                gua8Debounce[i] = 3u;//gua8Debounce[i] = KEYBOARD_DEBOUNCE-(_U08)1;
            }
        }
        else
        {
            gua8Debounce[i]--;
            if(gua8Debounce[i] < 1)
            {
                gua8Debounce[i] = 1;
                CLEAR_8BIT(gua8Status[i >> (_U08)3], (i & (_U08)7));
            }                   
        }
    }
}

/**-------------------------------------------------------------------------------------------------    
  \brief    None
  \param    None
  \return   None
  \warning  None   	
--------------------------------------------------------------------------------------------------*/
static void edge_detection(void)
{
    _U08 i = KEYBOARD_N_KEYS();
    _U08 u8Tempa, u8Tempb;        
    
    while(i--)
    {
        u8Tempa = QUERY_8BIT(gua8Status[i >> (_U08)3], (i & (_U08)7));
        u8Tempb = QUERY_8BIT(gua8StatusShadow[i >> (_U08)3], (i & (_U08)7));
        if(u8Tempa && !u8Tempb) /*Rising Edge*/
        {    
            gbKeyPressed = 1u;
            gu8LastKeyPressed = i;
            SET_8BIT(gua8Flags[i >> (_U08)3], (i & (_U08)7));       
        }
        else if(!u8Tempa && u8Tempb)/*Falling Edge*/
        {
            CLEAR_8BIT(gua8Flags[i >> (_U08)3], (i & (_U08)7));
        }
        else
        {
            /*MISRA*/
        }
    }
    
    i = KEYBOARD_BIT_ARRAY();
    while(i--)
    {
        gua8StatusShadow[i] = gua8Status[i];    
    }
}
