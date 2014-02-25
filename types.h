/*--------------------------------------------------------------------------------------------------
  "THE BEER-WARE LICENSE" (Revision 42):
  <phk@FreeBSD.ORG> wrote this file. As long as you retain this notice you
  can do whatever you want with this stuff. If we meet some day, and you think
  this stuff is worth it, you can buy me a beer in return to Hotboards crew 
  (Beer-ware license created by Poul-Henning Kamp)

  \file         types.h
  \author       Diego
  \email        diego.perez@hotboards.org
  \ver          1.0
  \date         July 25, 2013
  \target       Generic
  \brief        Header file used to redefine the variable types and several standard macros.
                Warning: this file should (must) not contain compiler-native macros, if needed,
                they should (must) be added in a separate file.
 -------------------------------------------------------------------------------------------------*/
#ifndef __TYPES_H__
#define __TYPES_H__

    
    /**---------------------------------------------------------------------------------------------
      \brief Redefinition of types
    ----------------------------------------------------------------------------------------------*/    
    typedef unsigned char           _U08;
    typedef unsigned short          _U16;
    typedef unsigned long           _U32;
    typedef signed char             _S08;
    typedef signed short            _S16;
    typedef signed long             _S32;
    typedef unsigned char           _BOOL;
    typedef signed char *           _STRING;
    
    /**---------------------------------------------------------------------------------------------
      \def      _uFLAGS 
      \brief    Bits handling
    ----------------------------------------------------------------------------------------------*/
    typedef union
    {
        _U08 u8Flags;
        struct
        {
            unsigned b0  :1;
            unsigned b1  :1;
            unsigned b2  :1;
            unsigned b3  :1;
            unsigned b4  :1;
            unsigned b5  :1;
            unsigned b6  :1;
            unsigned b7  :1;
        }_sBITS;
    }_uFLAGS;

    /**---------------------------------------------------------------------------------------------
      \def      _u4BYTES
      \brief    4-Bytes handling
    ----------------------------------------------------------------------------------------------*/
    typedef union
    {
        _U32 u32Long;
        _U16 u16Short[2];
        _U08 u8Byte[4];
    }_u4BYTES;

    /**---------------------------------------------------------------------------------------------
      \def      _u2BYTES
      \brief    2-Bytes handling
    ----------------------------------------------------------------------------------------------*/
    typedef union
    {
        _U16 u16Short;
        _U08 u8Byte[2];
    }_u2BYTES;

    /**---------------------------------------------------------------------------------------------
      \def      _sSM
      \brief    State machine structure
    ----------------------------------------------------------------------------------------------*/
    typedef struct
    {
        _U08 u8PrevState;
        _U08 u8NextState;
        _U08 u8ActualState;
        _U08 u8ErrorState;
    }_sSM;
    
    /**--------------------------------------------------------------------------------------------- 
      \def      _sTIME
      \brief    struct with the time information (hh:mm:ss), hours are given in 24 hrs format
    ----------------------------------------------------------------------------------------------*/ 
    typedef struct
    {
        _U08 u8Hour;
        _U08 u8Minutes;
        _U08 u8Seconds;
    }_sTIME;
    
    /**--------------------------------------------------------------------------------------------- 
      \def      _sDATE
      \brief    struct with the date information (yy:mm:dd), and day of the week
    ----------------------------------------------------------------------------------------------*/ 
    typedef struct
    {
        _U08 u8Day;
        _U08 u8Dayofweek;
        _U08 u8Month;
        _U08 u8Year;
    }_sDATE;

    /**---------------------------------------------------------------------------------------------
      \brief    Bit Manipulation (_U08 types)
    ----------------------------------------------------------------------------------------------*/
    #define SET_8BIT(var, bit)                ((var) |=  (_U08)((_U08)1<<(_U08)(bit)))
    #define CLEAR_8BIT(var, bit)              ((var) &= ~(_U08)((_U08)1<<(_U08)(bit)))
    #define QUERY_8BIT(var, bit)              ((_BOOL)((var)>>(_U08)(bit)) & ((_U08)1))
    #define TOGGLE_8BIT(var, bit)             ((var) ^=  (_U08)((_U08)1<<(_U08)(bit)))

    /**---------------------------------------------------------------------------------------------
      \brief    Bit Manipulation (_U16 types)
    ----------------------------------------------------------------------------------------------*/
    #define SET_16BIT(var, bit)               ((var) |=  (_U16)((_U16)1<<(_U16)(bit)))
    #define CLEAR_16BIT(var, bit)             ((var) &= ~(_U16)((_U16)1<<(_U16)(bit)))
    #define QUERY_16BIT(var, bit)             ((_BOOL)((var)>>(_U16)(bit)) & ((_U16)1))
    #define TOGGLE_16BIT(var, bit)            ((var) ^=  (_U16)((_U16)1<<(_U16)(bit)))

    /**---------------------------------------------------------------------------------------------
      \brief    Bit Manipulation  (_U32 types)
    ----------------------------------------------------------------------------------------------*/
    #define SET_32BIT(var, bit)               ((var) |=  (_U32)((_U32)1<<(_U32)(bit)))
    #define CLEAR_32BIT(var, bit)             ((var) &= ~(_U32)((_U32)1<<(_U32)(bit)))
    #define QUERY_32BIT(var, bit)             ((_BOOL)((var)>>(_U32)(bit)) & ((_U32)1))
    #define TOGGLE_32BIT(var, bit)            ((var) ^=  (_U32)((_U32)1<<(_U32)(bit)))

    /**---------------------------------------------------------------------------------------------
      \brief    Shift left or right a variable a numer of fixed positions (_U08 types)
    ----------------------------------------------------------------------------------------------*/
    #define LEFT_8SHIFT(var, pos)             ((var) <<= (_U08)(pos))
    #define RIGHT_8SHIFT(var, pos)            ((var) >>= (_U08)(pos))

    /**---------------------------------------------------------------------------------------------
      \brief    Shift left or right a variable a numer of fixed positions (_U16 types)
    ----------------------------------------------------------------------------------------------*/
    #define LEFT_16SHIFT(var, pos)            ((var) <<= (_U16)(pos))
    #define RIGHT_16SHIFT(var, pos)           ((var) >>= (_U16)(pos))

    /**---------------------------------------------------------------------------------------------
      \brief    Shift left or right a variable a numer of fixed positions (_U32 types)
    ----------------------------------------------------------------------------------------------*/
    #define LEFT_32SHIFT(var, pos)            ((var) <<= (_U32)(pos))
    #define RIGHT_32SHIFT(var, pos)           ((var) >>= (_U32)(pos))

    
    /**---------------------------------------------------------------------------------------------
      \brief Pin values
    ----------------------------------------------------------------------------------------------*/
    #define _HIGH                             1u
    #define _LOW                              0u
    
    /**---------------------------------------------------------------------------------------------
      \brief Error values
    ----------------------------------------------------------------------------------------------*/
    #define _ERROR                            0u
    #define _SUCCES                           1u
    
    /**---------------------------------------------------------------------------------------------
      \brief Boolean values
    ----------------------------------------------------------------------------------------------*/
    #define _TRUE                             (_BOOL)1
    #define _FALSE                            (_BOOL)0
    

#endif