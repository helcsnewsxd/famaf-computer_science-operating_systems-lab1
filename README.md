***Laboratorio número 1 de Sistemas Operativos 2022 - Grupo 12 | Famaf UNC*** 

*Integrantes del grupo:* 

- Lautaro Bachmann (INSERTAR MAIL)
- Juan Bratti (INSERTAR MAIL)
- Gonzalo Canavesio (gonzalo.canavesio@mi.unc.edu.ar)
- Emanuel Herrador (INSERTAR MAIL)


**INDICE**

- [Introducción al informe](#introducci-n-al-informe)
- [Como correr el codigo](#como-correr-el-codigo)
  * [Instalación](#instalaci-n)
  * [Compilación y ejecución](#compilaci-n-y-ejecuci-n)
- [Introducción](#introducci-n)
- [Modularización](#modularizaci-n)
  * [command](#command)
    + [TAD scommand](#tad-scommand)
    + [TAD pipeline](#tad-pipeline)
  * [parsing](#parsing)
  * [parser](#parser)
  * [execute](#execute)
  * [builtin](#builtin)
  * [internal_commands](#internal-commands)
  * [strextra](#strextra)
  * [prompt](#prompt)
  * [mybash](#mybash)
- [Herramientas de Programación](#herramientas-de-programaci-n)
  * [Desarrollo](#desarrollo)
    + [Comentarios](#comentarios)
  * [Compilacion](#compilacion)
  * [Debugging](#debugging)
- [Desarrollo del proyecto](#desarrollo-del-proyecto)
  * [Proceso de implementación](#proceso-de-implementaci-n)
  * [Pruebas utilizadas](#pruebas-utilizadas)
  * [Problemas y soluciones durante el desarrollo](#problemas-y-soluciones-durante-el-desarrollo)
- [Conclusiones](#conclusiones)
- [Referencias y bibliografía](#referencias-y-bibliograf-a)

# Introducción al informe
Para evitar generar un informe con el largo de 25 hojas, con nuestro grupo decidimos dar solo una breve explicación sobre la implementación de cada modulo del proyecto, la cual es ampliada en un archivo aparte por si usted, el lector, quiere profundizar más en algún modulo.

# Como correr el codigo
## Instalación
1. Clonar repositorio.
2. Instalar librerias `glib` y `check`.

## Compilación y ejecución 
Para compilar el código, basta con ejecutar el comando `make`. 

Para ejecutar el programa, basta con ejecutar `./mybash` luego de haber compilado el código.

# Introducción
El proyecto trata sobre codificar un shell al estilo de bash (Bourne Again SHell) al que llamaremos mybash. 

El programa debe poder satisfacer la ejecución de comandos en modo foreground y background, la redirección de entrada y salida estándar y realizar pipe entre comandos.

# Modularización
En la implementación de mybash se procesa una lista de órdenes del usuario, cada una de las cuales sse representa mediante una instancia del TAD pipeline. El programa mybash recibe la entrada de la shell desde la standard input y debe llamar al modulo parsing para que este le de formato y genere una instancia de pipeline. 
A continuación debe llamar al modulo execute que se encarga de llamar a **fork** y **execvp** para ejecutar los comandos simples. En caso de ser un comando interno, utiliza la función **builtin_run** y en caso de ser un pipeline con varios comandos, se utiliza la función **do_an_execute_pipeline** para gestionar la ejecución de los diferentes comandos y sus entradas y salidas.

Los modulos implementados son los siguientes:

- mybash: Módulo principal
- command: Implementación de los TADs para representar los comandos
- parsing: Procesamiento de la entrada del usuario usando el parser
- parser: Implementación del TAD parser 
- execute: Ejecución de comandos y administración de las llamadas al sistema operativo
- builtin: Implementación de interfaces para interactuar con comandos internos e identificarlos
- internal\_commands: Implementación de los comandos internos
- prompt: Implementación de una prompt con informacion relevante para el usuario
- strextra: Implementación de función auxiliar para el manejo de strings.

## command
(INSERTAR DESCRIPCIÓN GENERAL DEL COMMAND)

### TAD scommand
(INSERTAR DESCRIPCIÓN GENERAL DEL SCOMMAND)

(INSERTAR EXPLICACIÓN IMPLEMENTACIÓN DEL SCOMMAND)

### TAD pipeline
(INSERTAR DESCRIPCIÓN GENERAL DEL PIPELINE)

(INSERTAR EXPLICACIÓN IMPLEMENTACIÓN DEL PIPELINE)

## parsing
módulo de procesamiento de la entrada del usuario usando un parser



## parser 
módulo que implementa el TAD parser

## execute
módulo ejecutor de comandos, administra las llamadas al sistema operativo

## builtin
módulo que administra los comandos internos del intérprete de comandos.

## internal_commands
módulo que implementa los comandos internos del intérprete de comandos.

## strextra
Se declara la función *strmerge*, utilizada para implementar las funciones *scommand_to_string* y *pipeline_to_string* del modulo command.

## prompt

## mybash
diseño general y particular, diagrama de módulos, descripción general de los módulos, TADs internos a los módulos y los que comunican itermódulo, funciones públicas y privadas, algoritmos destacables.


# Herramientas de Programación
## Desarrollo
- Visual Studio Code, editor de código
- Live Share, extensión para el desarrollo colaborativo en tiempo real
- ClangFormat, formateador de código

### Comentarios
- Para obtener un estilo consistente utilizamos ClangFormat con un formato basado en el estilo de codigo usado por LLVM.
- Generamos una configuracion comun de vscode y la subimos al repositorio para que el editor de texto de todos los integrantes del grupo se comporte de una manera similar.

## Compilacion
- GNU Make

## Debugging
- GDB, depurador estándar para el compilador GNU.
- Valgrind, conjunto de herramientas para la depuración de problemas de memoria y rendimiento de programas.

# Desarrollo del proyecto
Los 4 integrantes del grupo al inicio estuvimos divididos en 2 subgrupos, lo que nos permitió mayor flexibilidad para trabajar en dos módulos en simultaneo y además tener alguien con quien apoyarnos en caso de dudas o errores. Una vez implementados todos los modulos, los grupos se disolvieron y cada integrante ayudaba cuando podia en solucionar errores, mejorar el formato/comentarios del código y realizar el informe.

Nos basamos fuertemente en el uso de branchs (ramas) dentro del repositorio en bitbucket, para no tener conflictos del código a modificar o corregir. Una vez el modulo correspondiente a la rama estaba desarrollado, pasaba los tests y no presentaba errores de memoria, lo fusionábamos con la rama principal y creábamos una nueva rama para el próximo modulo a desarrollar.

Semanalmente los integrantes del grupo se reunían con los profesores para mostrar todo el proceso hecho hasta el momento, compartir los avances y solucionar dudas e inquietudes.

## Proceso de implementación
Comenzamos desarrollando el modulo command, donde Emanuel y Juan se encargaron del TAD scommand y Lauty y Gonza del TAD pipeline. Luego de tener los TADs correctamente desarrollados y funcionando, nos concentramos en implementar el modulo builtin para la ejecución de comandos internos (Gonza y Lauty) y el parsing para procesar el input del usuario (Emanuel y Juan). Finalmente Lauty y Gonza se pusieron con el execute, donde se encuentra la implementación para ejecutar los comandos ingresados por el usuario, tanto comandos multiples como simples y tanto en foreground como en background. Mientras se hacía eso, Juan y Ema se encargaron de implementar el modulo principal de mybash y la personalización para el prompt. Pero como con la implementación realizada aparecían un par de errores en el test del execute, refactorizamos el código del execute para ver si así podían solucionarse (algunos de los errores pudieron arreglarse, pero aún quedaban varios). Necesitamos estar un par de de días trabajando sobre los errores hasta que pudimos encontrarle la solución a todo.

Una vez todo el código fue desarrollado, compilado y satisfacía los tests dados por la cátedra y los propios hechos por nosotros, confirmando que todo funcionara correctamente, empezamos a realizar las correcciones de estilo y formato del código, para tener uniformidad y cumplir con las pautas de la catedra. Esto fue bastante sencillo ya que Lautaro realizó al comienzo del proyecto un archivo de formateado que le daba un formato uniforme a nuestro código, las correcciones y ajustes que tuvimos que hacer fueron centralmente en nombre de variables o funciones y en el agregado de comentarios en el código para facilitar la lectura y el poder seguir la linea de implmentación del código.

Durante todo el desarrollo del proyecto intentamos ir realizando reportes/informes periódicos de nuestras implementaciones, para al final poder realizar un informe lo más completo posible. El desarrollo del informe final comenzó de manera paralela a la corrección de errores del execute y estuvimos hasta el último día mejorandolo y agregandole información.

## Pruebas utilizadas
Se realizaron pruebas unitarias caseras para cada módulo, además de las pruebas provistas por la catedra para las cuales necesitamos utilizar la librería Check para C. Para aquellos módulos donde se realizo una prueba casera, se creó un archivo de prueba, con la extensión .c, en la carpeta de **custom_tests**.

Primero nos asegurábamos de que el código diera los resultados esperados según los tests, y luego revisábamos que no existieran memory leaks en el código, utilizando **valgrind** como principal herramienta para encontrar y verificar estos errores de memoria.

## Problemas y soluciones durante el desarrollo
(AGREGAR)

# Conclusiones 

Se logró implementar un shell básico, con la ejecución de comandos tanto en modo foreground y en modo background, con la redirección de entrada y salida estándar, y con la posibilidad de realizar multiples pipes (más de 2) entre comandos. 

En este proyecto aprendimos a como trabajar en equipo y organizar las tareas para generar una división equitativa. Esto nos sirvió para mejorar la coordinación dentro del grupo y para comprender como funciona un shell y ayudarnos en las dudas que tenia cada uno. 

También pudimos implementar bastantes conceptos vistos en el teórico, como la utilización de **fork** y **execvp**, la redirección de entrada y salida estándar, y la utilización de señales. 

En este proyecto también pudimos aprender y mejorar nuestro uso de git. 

# Referencias y bibliografía

- https://man7.org/linux/man-pages/ 
- https://pages.cs.wisc.edu/~remzi/OSTEP/
