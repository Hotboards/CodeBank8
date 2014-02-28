CodeBank8 (maquinas de 8 bits)
==============================


En este repositorio se encuentra la librería oficial de **[Hotboards](http://hotboards.org/)** para maquinas de 8 bits. El código que encontrarás en esta página te permitirá desarrollar más rápido tus proyectos, fue diseñado para ser utilizado incluso por software de producción. Es perfectamente compatible con la técnica de programación de "Máquinas de Estado" la cual te permitirá programar aplicaciones multitarea. 

Revisa nuestra **[Wiki](https://github.com/Hotboards/CodeBank8/wiki)** para que conozcas mas acerca de como usar los códigos que aquí encontraras. Ademas no se te olvide clonar nuestro repositorio de **[ejemplos](https://github.com/Hotboards/Examples)**, los cuales te serán de gran ayuda y guía ante posibles dudas.

Los diferentes drivers y piezas de código que podrás descargar son de uso libre, solo obedecen a la siguiente licencia, la cual es opcional =)

```
/*--------------------------------------------------------------------------------
  "THE BEER-WARE LICENSE" (Revision 42):
  <phk@FreeBSD.ORG> wrote this file. As long as you retain this notice you
  can do whatever you want with this stuff. If we meet some day, and you think
  this stuff is worth it, you can buy me a beer in return to Hotboards crew 
  (Beer-ware license created by Poul-Henning Kamp)
--------------------------------------------------------------------------------*/
```

Directorios
-----------

El código que encontraras dentro de este repositorio esta subdividido en la siguientes capas:

- **bsp.-** Todas la piezas de código que se comuniquen directamente con la maquina y sean dependientes de la misma (puertos, adc, serial, etc.)  

- **middleware.-** Piezas de código que no son dependientes de ningún hardware en especifico y que solo estén programadas usando el estándar ANSI C

- **modules.-** Drivers para comunicar con periféricos externos, tales como memorias, sensores, modems, etc. No son dependientes de ninguna maquina en especifico.


Lista de Códigos
----------------

Consulta las **[lista de código](https://github.com/Hotboards/CodeBank8/blob/master/codigos.md)** que encontraras en el repositorio



Configuración
-------------

Para que no tengas ningún problema al utilizar las piezas de código que encontraras en este repositorio deberás indicarle a tu compilador las rutas de los siguientes directorios.

```
<path_to>/CodeBank8
<path_to>/CodeBank8/middleware
<path_to>/CodeBank8/modules
<path_to>/CodeBank8/bsp/<fabricante> (ejemplo: CodeBank8/bsp/Microchip )
```
Lo anterior lo podrás hacer a través del ambiente de desarrollo que estés usando o directamente en un archivo Makefile

