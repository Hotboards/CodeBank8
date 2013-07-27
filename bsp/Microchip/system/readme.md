Configuraciones internas el CPU (system)
----------------------------------------
-----------

Este driver maneja operaciones internas realizadas por el CPU tales como activación/desactivacion de interrupciones, reset del contador de watchdog, reset por software y la activación interna del PLL para que aumente la frecuencia de operación a 48MHz.

Actualmente este driver no puede cambiar las configuraciones de reloj, a reloj secundario (31KHz) ni tampoco a modos de operación mas avanzadas como IDLE, SLEEP o DEEP SLEEP en un futuro se pretende agregar esas opciones.

La manera en la que podemos aumentar la frecuencia de operación es activando el PLL interno que por default incrementa a la maxima de 48MHz
```
System_EnablePLL();	/*es recomendable que esta función se llama inmediatamente después de reset*/
```

Se sugiere activar y desactivar las interrupciones globales usando los siguientes macros
```
__ENABLE_INTERRUPTS(); 
....
__DISABLE_INTERRUPTS();
```

####API
```
	/**--------------------------------------------------------------------------------------------- 
      \def        ENABLE_INTERRUPTS/DISABLE_INTERRUPTS
      \brief      Habilita o deshabilita las interrupciones globales del uC
    ----------------------------------------------------------------------------------------------*/ 
    #define __ENABLE_INTERRUPTS()                 System_EnableInterrupts()
    #define __DISABLE_INTERRUPTS()                System_DisableInterrupts()

    /**---------------------------------------------------------------------------------------------
      \def        __KICK_THE_DOG()
      \brief      Reinicia el contador del Watchdog timer
    ----------------------------------------------------------------------------------------------*/
    #define __KICK_THE_DOG()                      {_asm clrwdt _endasm}

    /**---------------------------------------------------------------------------------------------
      \def        __RESET()
      \brief      Provoca un reset del uC
    ----------------------------------------------------------------------------------------------*/
    #define __RESET()                             {_asm reset _endasm}

    /**---------------------------------------------------------------------------------------------
      \def        __NOP()
      \brief      Ejecuta una instrucción que consume un ciclo en el procesador
    ----------------------------------------------------------------------------------------------*/
    #define __NOP()                               {_asm nop _endasm}

    
    /*-- Functions --*/
    /**---------------------------------------------------------------------------------------------
      \brief      Activa el PLL interno para que la frecuencia interna aumente a 48MHz
      \param      None
      \return     None
      \warning    Esta función ciclara al uC por espacio de 2ms
    ----------------------------------------------------------------------------------------------*/
    void System_EnablePLL(void);

    /**---------------------------------------------------------------------------------------------
      \brief      Activa interrupciones globales con prioridad
      \param      None
      \return     None
      \warning    Esta función activa los bits GEIH, GEIL y IPEN
    ----------------------------------------------------------------------------------------------*/
    void System_EnableInterrupts(void);

    /**---------------------------------------------------------------------------------------------
      \brief      Desactiva interrupciones globales con prioridad
      \param      None
      \return     None
      \warning    Esta función limpia los bits GIEH, GIEL e IPEN
    ----------------------------------------------------------------------------------------------*/
    void System_DisableInterrupts(void);

```
####Ejemplos
Descomprime estos ejemplos en el mismo directorio donde tengas tu banco de código.

- [Ejemplo 1: Aumento de la frecuencia de operación][1]
- [Ejemplo 2: Parpadeo de led usando timers e interrupciones a 48MHz][2]


  [1]: http://www.hotboards.org/images/codigo/8bits/examples/system1.zip
  [2]: http://www.hotboards.org/images/codigo/8bits/examples/system2.zip
  