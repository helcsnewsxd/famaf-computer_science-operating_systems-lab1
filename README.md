***Laboratorio número 1 de Sistemas Operativos 2022 - Grupo 12 | Famaf UNC*** 

*Integrantes del grupo:* 

- Lautaro Bachmann (INSERTAR MAIL)
- Juan Bratti (INSERTAR MAIL)
- Gonzalo Canavesio (gonzalo.canavesio@mi.unc.edu.ar)
- Emanuel Herrador (INSERTAR MAIL)

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

En la implementación de mybash se procesa una lista de órdenes del usuario, cada una de las cuales sse representa mediante una instancia del TAD pipeline. El programa mybash recibe la entrada de la shell desde la standard input y debe llamar al modulo parsing para que este le de formato y genere una instancia de pipeline. 
A continuación debe llamar al modulo execute que se encarga de llamar a **fork** y **execvp** para ejecutar el comando. En caso de ser un comando interno, utiliza la función **builtin_run**


# Modularización
(INSERTAR DESCRIPCIÓN GENERAL DE LA MODULARIZACIÓN)

(EXPLICAR TODO EL SISTEMA DE MODULOS)

## Modulos implementados
- mybash: módulo principal
- command: módulo con las definiciones de los TADs para representar comandos
- parsing: módulo de procesamiento de la entrada del usuario usando un parser
- parser: módulo que implementa el TAD parser
- execute: módulo ejecutor de comandos, administra las llamadas al sistema operativo
- builtin: módulo que implementa interfaces para interactuar con comandos internos e identificarlos
- internal\_commands: modulo que implementa los comandos internos usados en builtin
- prompt: modulo que implementa una prompt con informacion relevante para el usuario

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


## mybash
diseño general y particular, diagrama de módulos, descripción general de los módulos, TADs internos a los módulos y los que comunican itermódulo, funciones públicas y privadas, algoritmos destacables.


# Herramientas de Programación
## Desarrollo
- Visual Studio Code
- Live Share
- ClangFormat

### Comentarios
- Para obtener un estilo consistente utilizamos ClangFormat con un formato basado en el estilo de codigo usado por LLVM.
- Generamos una configuracion comun de vscode y la subimos al repositorio para que el editor de texto de todos los integrantes del grupo se comporte de una manera similar.


## Compilacion
- GNU Make

## Debugging
- GDB
- Valgrind

# Desarrollo del proyecto
Para desarrollar el proyecto, los 4 integrantes del grupo la mayor parte del tiempo estuvimos divididos en 2 subgrupos, lo que nos permitió mayor flexibilidad para trabajar en dos módulos en simultaneo y además tener alguien con quien apoyarnos en caso de dudas o errores.

Nos basamos fuertemente en el uso de branchs (ramas) dentro del repositorio en bitbucket, para no tener conflictos del código a modificar o corregir, además que nos permitía tener una versión de código con las nuevas actualizaciones que se podían corregir cómodamente y no generar cambios en la rama principal. Una vez el modulo correspondiente a la rama estaba correctamente desarrollado, pasaba los tests y no presentaba errores, lo fusionábamos con la rama principal y creábamos una nueva rama para el próximo modulo a desarrollar.

Semanalmente los integrantes del grupo se reunían con los profesores para mostrar todo el proceso hecho hasta el momento, compartir los avances y solucionar dudas o inquietudes.

## Proceso de implementación
Comenzamos desarrollando el modulo command, donde Emanuel y Juan se encargaron del TAD scommand y Lauty y Gonza del TAD pipeline. Luego de tener los TADs correctamente desarrollados y funcionando, nos concentramos en implementar el modulo builtin para la ejecución de comandos internos (Gonza y Lauty) y el parsing para procesar el input del usuario (Emanuel y Juan). Finalmente Lauty y Gonza se pusieron con el execute, donde se encuentra la implementación para ejecutar los comandos ingresados por el usuario, tanto comandos multiples como simples y tanto en foreground como en background. Mientras se hacía eso, Juan y Ema se encargaron de implementar el modulo principal de mybash y la personalización para el prompt.

Una vez todo el código fue desarrollado, compilado y satisfacía los tests dados por la cátedra y los propios hechos por nosotros, confirmando que todo funcionara correctamente, empezamos a realizar las correcciones de estilo y formato del código, para tener uniformidad y cumplir con las pautas de la catedra. Esto fue bastante sencillo ya que Lautaro realizó al comienzo del proyecto un archivo de formateado que le daba un formato uniforme a nuestro código, las correcciones y ajustes que tuvimos que hacer fueron centralmente en nombre de variables o funciones y en el agregado de comentarios en el código para facilitar la lectura y el poder seguir la linea de implmentación del código.

## Pruebas utilizadas
Se realizaron pruebas unitarias caseras para cada módulo, además de las pruebas provistas por la catedra para las cuales necesitamos utilizar la librería Check para C. Para aquellos módulos donde se realizo una prueba casera, se creó un archivo de prueba, con la extensión .c, en la carpeta de tests personalizados.

Primero nos asegurábamos de que el código diera los resultados esperados según los tests, y luego revisábamos que no existieran memory leaks en el código, utilizando valgrind como principal herramienta para encontrar y verificar estos errores de memoria.

## Problemas y soluciones durante el desarrollo
(AGREGAR)

# Conclusiones 

Se logró implementar un shell básico, con la ejecución de comandos tanto en modo foreground y en modo background, con la redirección de entrada y salida estándar, y con la posibilidad de realizar multiples pipes (más de 2) entre comandos. 

En este proyecto aprendimos a como trabajar en equipo y organizar las tareas para generar una división equitativa. Esto nos sirvió para mejorar la coordinación dentro del grupo y para comprender como funciona un shell y ayudarnos en las dudas que tenia cada uno. 

También pudimos implementar bastantes conceptos vistos en el teórico, como la utilización de **fork** y **execvp**, la redirección de entrada y salida estándar, y la utilización de señales. 

En este proyecto también pudimos aprender y mejorar nuestro uso de git. 

# Referencias y bibliografía utilizada 

- https://man7.org/linux/man-pages/ 
- https://pages.cs.wisc.edu/~remzi/OSTEP/
