
Microchip BSP
-------------
-------

**NOTA: este branch es experimnetal, se esta trabajando en la migracion del codigo para el mas nuevo compilador XC8. Se considerea altamente recomendable eperar a que ocurra el release 2.0 del banco de codigo**

Funciones de bajo nivel que comunican las aplicaciones con los registros de los uC de 8 bits de la marca Microchip (PIC18F) Usando el compilador XC8.

Actualmente este BSP a sido probado en los siguientes equipos, pero en teoría dentro de los dispositivos PIC18F deben ser compatibles, solo basta averiguar si los registros internos de los periféricos son iguales.

####Compatibilidad
- PIC18F24J50
- PIC18F25J50
- PIC18F26J50
- PIC18F44J50
- PIC18F45J50
- PIC18F46J50 

####Directorios
- **gpios.-** Manejo de puertos de propósito general
- **system.-** Configuraciones internas del cpu del uC
- **delays.-** Retardos por software
- **swtimers.-** Multiplicadores de bases de tiempo
- **uart.-** Lectura y escritura del puerto serial
- **adc.-** Convertidor análogo digital
- **spi.-** Serial peripheral Comunications
- **usb.-** Bus Serial Universal
