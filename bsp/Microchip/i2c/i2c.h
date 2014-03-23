/*--------------------------------------------------------------------------------------------------
  "THE BEER-WARE LICENSE" (Revision 42):
  <phk@FreeBSD.ORG> wrote this file. As long as you retain this notice you
  can do whatever you want with this stuff. If we meet some day, and you think
  this stuff is worth it, you can buy me a beer in return to Hotboards crew
  (Beer-ware license created by Poul-Henning Kamp)

  \file         i2c.h
  \author       Diego
  \email        diego.perez@hotboards.org
  \ver          1.0
  \date         Marzo 19, 2014
  \target       PIC18FJ

  \brief        Driver para controlar la generación de señales necesarias para implementar el
                protocolo de comunicaciones sincrona I2C. El driver hace uso del periférico MSSP
                por lo cual una vez seteado no se puede usar como spi, además sólo opera en modo
                maestro.
--------------------------------------------------------------------------------------------------*/
#ifndef __I2C_H__
#define __I2C_H__


    /*-- Includes --*/
    #include "types.h"
     

    /*-- Defines --*/
    /**---------------------------------------------------------------------------------------------
      \def      I2C_CLOCK
      \brief    Esta definicion es interna unicamente, autuando solo como valor por default cuando
                cuando no se define BSP_CLOCK en hardware_profile.h
    ----------------------------------------------------------------------------------------------*/
    //#define I2C_CLOCK                 12000000 (move to i2c.c)

    /**---------------------------------------------------------------------------------------------
      \def      _eI2C
      \brief    Moulos i2c que pueden ser usados en el driver
    ----------------------------------------------------------------------------------------------*/
    typedef enum
    {
        I2C_PORT1 = 1,
        I2C_PORT2
    }_eI2C;

    /**---------------------------------------------------------------------------------------------
      \def      _eACK
      \brief    Pulsos de Acknowledge y Not Acknowledge a mandar o recivir
    ----------------------------------------------------------------------------------------------*/
    typedef enum
    {
        I2C_ACK = 0,
        I2C_NACK
    }_eACK;

    
    /*-- Macros --*/
    /**---------------------------------------------------------------------------------------------
      \def        ADDR_x
      \brief      Macros para indicar una transmision de lectura o escritura en el bus
    ----------------------------------------------------------------------------------------------*/
    #define ADDR_WRITE(add)          (((add) << 1) & (0xfe))
    #define ADDR_READ(add)           (((add) << 1) | (0x01))


    /*-- Functions --*/
    /**---------------------------------------------------------------------------------------------
      \brief      Inicializa el periférico MSSPx en modo I2C en mdod maestro y con la frceuncia
                  indicada
      \param	  u8I2c.- Modulo I2C a ser inicializado
      \param      u32Frec.- Frecuencia a la cual se comunicara el modulo (solo valroes estandar)
      \return     None
      \warning	  Debes tomar en cuenta por estandar se manejan velocidades de 100KHz, 400KHz y 1MHz
    ----------------------------------------------------------------------------------------------*/
    void I2c_Init(_U08 u8I2c, _U32 u32Frec);

    /**---------------------------------------------------------------------------------------------
      \brief      Genera la condicion de start en las lineas de DATA y CLK en el modulo indicado
      \param	  u8I2c.- Modulo I2C en el cuale se generara la condicion de start
      \return     None
      \warning	  None
    ----------------------------------------------------------------------------------------------*/
    void I2c_Start(_U08 u8I2c);

    /**---------------------------------------------------------------------------------------------
      \brief      Transmite un byte por I2C 
      \param	  u8I2c.- Modulo I2C en el cuale se mnadara el byte
      \param	  u8Byte.- byte a transmitir
      \return     El valor de aknowlege que regresa el dispositivo esclavo (valores ACK y NACK)
      \warning	  None
    ----------------------------------------------------------------------------------------------*/
    _BOOL I2c_bTxByte(_U08 u8I2c, _U08 u8Byte);
    
    /**---------------------------------------------------------------------------------------------
      \brief      Recive un byte por I2C 
      \param	  u8I2c.- Modulo I2C en el cuale se recivira el byte
      \param	  bAck.- ACK o NACK a mandar al dispositivo esclavo
      \return     El byte que mando el dispositivo esclavo 
      \warning	  None
    ----------------------------------------------------------------------------------------------*/
    _U08 I2c_u8RxByte(_U08 u8I2c, _BOOL bAck);

    /**---------------------------------------------------------------------------------------------
      \brief      Genera la condicion de repeated start en las lineas de DATA y CLK en el modulo indicado
      \param	  u8I2c.- Modulo I2C en el cuale se generara la condicion
      \return     None
      \warning	  None
    ----------------------------------------------------------------------------------------------*/
    void I2c_RepeatedStart(_U08 u8I2c);

    /**---------------------------------------------------------------------------------------------
      \brief      Genera la condicion de stop en las lineas de DATA y CLK en el modulo indicado
      \param	  u8I2c.- Modulo I2C en el cuale se generara la condicion
      \return     None
      \warning	  None
    ----------------------------------------------------------------------------------------------*/
    void I2c_Stop(_U08 u8I2c);


#endif   
 