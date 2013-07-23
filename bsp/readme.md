Drivers de bajo nivel (BSP layer)
---------------------------------
-----------
Aquí encontraras las piezas de código que se comunican directamente con el hardware del microcontrolador, es decir son aquellas partes de código que controlan los periféricos (adc, timers, uart, pwm, etc.) y registros internos de la maquina. son dependientes del compilador y del fabricante.

La idea detrás de este tipo de código es separar la complejidad que implica codificar directamente con el micro y separarla de la capa de aplicación, dotándola de potabilidad entre fabricantes y reusabilidad sacrificando un poco de rendimiento.

Las interfaces (funciones y/o macros) comparten notaciones similares entre fabricantes. Así que las mismas funciones para trasmitir por puerto serial (ejemplo) podrán usarse con Atmel, Microchip o Freescale.

####Directorios
- **Microchip.-** Drivers de bajo nivel para la familia de 8 bits PIC18F.

- **Atmel.-** Drivers de bajo nivel para la familia de microntroladores Atmega (próximamente).  

- **Freescale.-** Drivers de bajo nivel para la familia S08 (próximamente).

