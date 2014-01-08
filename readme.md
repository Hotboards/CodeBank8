CodeBank8 (low cost machines)
-----------------------------

------

En este repositorio se encuentra la librería oficial de **Hotboards** para maquinas de 8 bits. 

El código que encontrarás en esta página te permitirá desarrollar más rápido tus proyectos, fue diseñado para ser utilizado incluso por software de producción. 

El código es perfectamente compatible con la técnica de programación de "Máquinas de Estado" la cual te permitirá programar aplicaciones multitarea. 

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

####Directorios

- **bsp.-** Todas la piezas de código que se comuniquen directamente con la maquina y sean dependientes de la misma  

- **middleware.-** Piezas de código que no son dependientes de ningún hardware en especifico y que solo estén programadas usando el estandar ANSI C

- **modules.-** Drivers para comunicar con periféricos externos, tales como memorias, sensores, modems, etc. No son dependientes de ninguna maquina en especifico.

####Configuracion

Para que no tengas ningun problema al utilizar las piezas de codigo que encontraras en este repositorio deberas indicarle a tu compilador las rutas de los siguientes directorios.

```
<path_to>/CodeBank8
<path_to>/CodeBank8/middleware
<path_to>/CodeBank8/modules
<path_to>/CodeBank8/bsp/<fabricante> (ejemplo: CodeBank8/bsp/Microchip )
```
Lo anterior lo podras haser atravez del ambiente de desarrollo que estes usando o directamente en un archivo Makefile

