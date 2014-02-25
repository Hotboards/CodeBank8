/*--------------------------------------------------------------------------------------------------
  "THE BEER-WARE LICENSE" (Revision 42):
  <phk@FreeBSD.ORG> wrote this file. As long as you retain this notice you
  can do whatever you want with this stuff. If we meet some day, and you think
  this stuff is worth it, you can buy me a beer in return to Hotboards crew 
  (Beer-ware license created by Poul-Henning Kamp)

  \file         system.h
  \author       Diego
  \email        diego.perez@hotboards.org
  \ver          1.0
  \date         August 20, 2013
  \target       PIC18F series

  \brief        Este driver maneja operaciones internas realizadas por el CPU tales como activacion/
                desactivacion de interrupciones, reset del contador de watchdog, reset por software,
                y la activacion interna del PLL para que aumente la frecuencia de operacion a 48MHz
 -------------------------------------------------------------------------------------------------*/
#ifndef __SYSTEM_H__
#define __SYSTEM_H__


    /*-- Includes --*/
    #include "types.h"
     

    /*-- Defines --*/
    /**--------------------------------------------------------------------------------------------- 
      \def        _ePPS
      \brief      perifericos que pueden ser reubicados en los diferentes pines "RPn"
    ----------------------------------------------------------------------------------------------*/ 
    typedef enum
    {
        ExternalInterrupt1 = 0,
        ExternalInterrupt2,
        ExternalInterrupt3,
        Timer0ExternalClockInput,
        Timer3ExternalClockInput,
        InputCapture1,
        InputCapture2,
        Timer1GateInput,
        Timer3GateInput,
        AsynchronousReceive2,
        AsynchronousClockInput2,
        SPI2DataInput,
        SPI2ClockInput,
        SPI2SlaveSelect,
        PWMFaultInput,
        Comparator1Output = 101,
        Comparator2Output,
        AsynchronousTransmit2 = 105,
        SynchronousTransmit2,
        SPI2DataOutput = 109,
        SPI2ClockOutput,
        SPIDMASlaveSelect = 112,
        UltraLow_PowerWake_upEvent,
        CompareOrPWMOutputChannel1A,
        EnhancedPWMOutputChannel1B,
        EnhancedPWMOutputChannel1C,
        EnhancedPWMOutputChannel1D,
        CompareOrPWMOutputChannel2A,
        EnhancedPWMOutputChannel2B,
        EnhancedPWMOutputChannel2C,
        EnhancedPWMOutputChannel2D
    }_ePPS;


    /*-- Macros --*/
    /**--------------------------------------------------------------------------------------------- 
      \def        ENABLE_INTERRUPTS/DISABLE_INTERRUPTS
      \brief      Habilita o desabilita las interrupciones globales del uC
    ----------------------------------------------------------------------------------------------*/ 
    #define __ENABLE_INTERRUPTS()                 System_EnableInterrupts()
    #define __DISABLE_INTERRUPTS()                System_DisableInterrupts()

    /**---------------------------------------------------------------------------------------------
      \def        __KICK_THE_DOG()
      \brief      Resetea el contador del Watchdog timer
    ----------------------------------------------------------------------------------------------*/
    #define __KICK_THE_DOG()                      {_asm clrwdt _endasm}

    /**---------------------------------------------------------------------------------------------
      \def        __RESET()
      \brief      Provoca un reset del uC
    ----------------------------------------------------------------------------------------------*/
    #define __RESET()                             {_asm reset _endasm}

    /**---------------------------------------------------------------------------------------------
      \def        __NOP()
      \brief      Ejecuta una instruccion que consume un ciclo en el procesador
    ----------------------------------------------------------------------------------------------*/
    #define __NOP()                               {_asm nop _endasm}

    
    /*-- Functions --*/
    /**---------------------------------------------------------------------------------------------
      \brief      Activa el PLL interno para que la frecuecia interne aumente a 48MHz
      \param      None
      \return     None
      \warning    Esta funcion ciclara al uC por espacio de 2ms
    ----------------------------------------------------------------------------------------------*/
    void System_EnablePLL(void);

    /**---------------------------------------------------------------------------------------------
      \brief      Activa interrupciones globales con prioridad
      \param      None
      \return     None
      \warning    Esta funcion activa los bits GEIH, GEIL y IPEN
    ----------------------------------------------------------------------------------------------*/
    void System_EnableInterrupts(void);

    /**---------------------------------------------------------------------------------------------
      \brief      Desactiva interrupciones globales con prioridad
      \param      None
      \return     None
      \warning    Esta funcion limpia los bits GIEH, GIEL e IPEN
    ----------------------------------------------------------------------------------------------*/
    void System_DisableInterrupts(void);

    /**---------------------------------------------------------------------------------------------
      \brief      Asigna pines remapeables a perifericos que posean esa capacidad
      \param      ePeripheral.- salida/entrada de periferico a mapear
      \param      u8Pin.- cualquier pin marcado como "RPn" en el uC
      \return     None
      \warning    Se recomienda usar esta funcion solo en la inicializacion
    ----------------------------------------------------------------------------------------------*/
    void System_PeripheralPinSelect(_ePPS ePeripheral, _U08 u8Pin);


#endif   
 