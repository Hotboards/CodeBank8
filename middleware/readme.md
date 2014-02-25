Middleware
----------
-----
Piezas de código portables y reusables las cuales no son dependientes de la maquina en la que se compile y tampoco son dependientes del compilador, están escritas obedeciendo únicamente el estándar ANSI C11.

Algunas de etas piezas de código dependen directamente de drivers de bajo nivel para ejecutarse, tales como teclados o botones dependen directamente de drivers de gpios pero en ningún momento se comunican con registros internos del uC.

####Directorios

- **gpis.-** Manejo de entradas mecánicas de propósito general (botones)
- **gpos.-** Manejo de salidas oscilantes/retardadas de propósito general
- **printf.-** Emulacion la salida por pantalla de la libreria estandar
- **keyboard.-** Teclado matricial
- **6800.-** Interfaz paralela 6800 
