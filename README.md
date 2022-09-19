***Laboratorio número 1 de Sistemas Operativos 2022 - Grupo 12 | Famaf UNC*** 

*Integrantes del grupo:* 

- Lautaro Bachmann (lautaro.bachmann@mi.unc.edu.ar)
- Juan Bratti (juanbratti@mi.unc.edu.ar)
- Gonzalo Canavesio (gonzalo.canavesio@mi.unc.edu.ar)
- Emanuel Herrador (emanuel.nicolas.herrador@unc.edu.ar)


**INDICE**

- [Introducción al informe](#introducci-n-al-informe)
- [Como correr el codigo](#como-correr-el-codigo)
    * [Instalación](#instalaci-n)
    * [Compilación y ejecución](#compilaci-n-y-ejecuci-n)
- [Introducción](#introducci-n)
- [Implementación](#implementaci-n)
    * [Modularización](#modularizaci-n)
    * [mybash](#mybash)
    * [command](#command)
        + [TAD scommand](#tad-scommand)
        + [TAD pipeline](#tad-pipeline)
    * [parsing](#parsing)
    * [parser](#parser)
    * [execute](#execute)
    * [files_descriptors](#files-descriptors)
    * [builtin](#builtin)
    * [internal_commands](#internal-commands)
    * [prompt](#prompt)
    * [strextra](#strextra)
- [Herramientas de Programación](#herramientas-de-programaci-n)
    * [Desarrollo](#desarrollo)
        + [Comentarios](#comentarios)
    * [Compilacion](#compilacion)
    * [Debugging](#debugging)
- [Desarrollo del proyecto](#desarrollo-del-proyecto)
    * [Organizacion del trabajo](#organizacion-del-trabajo)
    * [Comunicación](#comunicaci-n)
    * [Nuestro workflow de desarrollo](#nuestro-workflow-de-desarrollo)
        + [Branches](#branches)
        + [Live share](#live-share)
    * [Proceso de implementación](#proceso-de-implementaci-n)
        + [Modulo Command](#modulo-command)
        + [Modulos builtin y parsing](#modulos-builtin-y-parsing)
        + [Modulo execute](#modulo-execute)
        + [Mybash y prompt](#mybash-y-prompt)
        + [Correcciones de estilo y formato](#correcciones-de-estilo-y-formato)
        + [Informes detallados](#informes-detallados)
        + [Informe final](#informe-final)
    * [Pruebas utilizadas](#pruebas-utilizadas)
    * [Problemas y soluciones durante el desarrollo](#problemas-y-soluciones-durante-el-desarrollo)
        + [Tests que fallaban](#tests-que-fallaban)
        + [Diferencias entre Mybash y Bash](#diferencias-entre-mybash-y-bash)
        + [Desconocimiento y aprendizaje](#desconocimiento-y-aprendizaje)
        + [Soluciones](#soluciones)
- [Conclusiones](#conclusiones)
- [Referencias y bibliografía](#referencias-y-bibliograf-a)


# Introducción al informe
Con respecto al estilo del informe, para evitar alargarlo innecesariamente hemos decidido dar solo una breve explicación sobre la implementación de cada modulo del proyecto, la cual es ampliada en un archivo aparte por si el lector desea profundizar más en los detalles de algún modulo.

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

# Caracteristicas implementadas
## Funcionalidad basica
Al momento de entrega el proyecto cumple con todos las funcionalidades basicas pedidas por la cátedra.

## Puntos estrella
- Generalizar el comando pipeline “|” a una cantidad arbitraria de comandos simples
- Imprimir un prompt con información relevante.
    - En concreto, la prompt imprime el nombre de la shell, el camino relativo y el nombre de usuario.


## Modularización
Los modulos implementados son los siguientes:

- mybash: Módulo principal
- command: Implementación de los TADs para representar los comandos
- parsing: Procesamiento de la entrada del usuario usando el parser
- parser: Implementación del TAD parser 
- execute: Ejecución de comandos y administración de las llamadas al sistema operativo
- files_descriptors: Implementación de funciones para el manejo de redirecciones para las salidas/entradas de los comandos.
- builtin: Implementación de interfaces para interactuar con comandos internos e identificarlos
- internal\_commands: Implementación de los comandos internos
- prompt: Implementación de una prompt con informacion relevante para el usuario
- strextra: Implementación de función auxiliar para el manejo de strings.

La mayor parte de las modularizaciones fueron las dadas por la cátedra, pero agregamos otras como **internal\_commands**, **prompt** y **files_descriptors** con el objetivo de hacer más ordenado el código del programa y que cada modulo se centre en solo un objetivo en específico.

## mybash
Código de ejecución de mybash implementado utilizando un *while* loop que hace un print de la prompt con los respectivos datos relevantes y la lectura constante de los inputs del usuario hasta el fin de lectura o cierre de terminal. 

## command
Implementación del TAD scommand y TAD pipeline utilizados para la representación de los comandos y pipelines dentro de mybash. 

Para ambos TAD se utilizó la estructura GQueue de la librería externa **GLib**.

### TAD scommand

Tipo abstracto utilizado para representar los comandos ingresados por consola junto con sus respectivos argumentos y redireccionamientos en el caso de haber sido utilizados.

Para más detalle leer [implementación scommand](/scommand).

### TAD pipeline
Tipo abstracto utilizado para la implementación del uso de comandos múltiples a través del uso de pipes |.

Para más detalle leer [implementación pipeline](/pipeline).

## parsing
Módulo de procesamiento de la entrada del usuario usando el tipo Parser.

Para más detalle leer [implementación parsing](/parsing).

## parser 
Módulo implementado por la cátedra.

## execute
Módulo ejecutor de comandos, administra las llamadas al sistema operativo. 

Para más detalle leer [implementación execute](/execute).

## files_descriptors
Módulo que se encarga del comportamiento de la entrada y salida de cada comando en una pipeline a través de los respectivos redireccionamientos. Este módulo es usado en execute.

Para más detalle leer [implementación execute](/execute).

## builtin
Módulo que administra los comandos internos del intérprete de comandos.

Para más detalle leer [implementación builtin](/builtin).

## internal_commands
Módulo que implementa los comandos internos del intérprete de comandos.

Para más detalle leer [implementación builtin](/builtin).

## prompt
Módulo que se encarga de mostrar por la terminal la respectiva prompt de mybash con información relevante a través de la función show_prompt.

En colors.h definimos los códigos para los colores ANSI_COLOR_RED, ANSI_COLOR_BLUE, ANSI_COLOR_GREEN y el color para el reset ANSI_COLOR_RESET.

## strextra
Se declara la función *strmerge*, utilizada para implementar las funciones *scommand_to_string* y *pipeline_to_string* del modulo command.


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
## Organizacion del trabajo
Los 4 integrantes del grupo al inicio estuvimos divididos en 2 subgrupos: Lauti y Gonza, Ema y Juan. Cada grupo se encargaba de implemenar un modulo diferente.
Una vez implementados todos los modulos, los grupos se disolvieron y cada integrante ayudaba cuando podia en solucionar errores, mejorar el formato/comentarios del código y realizar el informe.

## Comunicación
La mayor parte del proyecto utilizamos Discord y Telegram para organizarnos, pero probamos varias herramienta, como jira y trello, hasta encontrar las que mejor se adaptaban al flujo del grupo.

## Nuestro workflow de desarrollo
### Branches
Nos basamos fuertemente en el uso de branchs (ramas) dentro del repositorio en bitbucket. Una vez el modulo correspondiente a la rama estaba desarrollado, pasaba los tests y no presentaba errores de memoria, lo fusionábamos con la rama principal y creábamos una nueva rama para el próximo modulo a desarrollar.

### Live share
También se usó mucho live share. Por esto mismo, en el grupo de Ema y Juan, la mayoría de los commits realizados por el subgrupo de Emanuel y Juan son por parte de Emanuel, porque se trabajaba de a dos pero se codeaba en la computadora de Emanuel.

## Proceso de implementación
### Modulo Command
Comenzamos desarrollando el modulo command, donde Emanuel y Juan se encargaron del TAD scommand y Lauty y Gonza del TAD pipeline.

### Modulos builtin y parsing
Luego de tener los TADs correctamente desarrollados y funcionando, nos concentramos en implementar el modulo builtin para la ejecución de comandos internos (Gonza y Lauty) y el parsing para procesar el input del usuario (Emanuel y Juan).

### Modulo execute
Finalmente Lauty y Gonza se pusieron con el execute, donde se encuentra la implementación para ejecutar los comandos ingresados por el usuario, tanto comandos multiples como simples y tanto en foreground como en background.

Pero como con la implementación realizada no se podian utilizar comandos internos adentro de una pipe y se habia interpretado que no se deberia de poder hacer redirecciones en la parte del medio de una pipe, Juan y Ema decidieron refactorizar el código del execute para cambiar esto. 

Una vez finalizada la refactorizacion estuvimos trabajando en conjunto un par de de días para lograr que todas las tests pasen hasta que eventualmente pudimos encontrarle la solución a todo.

### Mybash y prompt
Mientras Lauty y Gonza trabajaban en el modulo execute, Juan y Ema se encargaron de implementar el modulo principal de mybash y la personalización para el prompt.

### Correcciones de estilo y formato
Una vez todo el código fue desarrollado, compilado y satisfacía los tests dados por la cátedra y los propios hechos por nosotros, confirmando que todo funcionara correctamente, empezamos a realizar las correcciones de estilo y formato del código. 

Las correcciones y ajustes que tuvimos que hacer fueron centralmente en nombre de variables o funciones y en el agregado de comentarios en el código para facilitar la lectura y el poder seguir la linea de implmentación del código.

### Informes detallados
Durante todo el desarrollo del proyecto intentamos ir realizando reportes/informes periódicos de nuestras implementaciones, para al final poder realizar un informe lo más completo posible. 

### Informe final
El desarrollo del informe final comenzó de manera paralela a la corrección de errores del execute y estuvimos hasta el último día mejorandolo y agregandole información.

## Pruebas utilizadas
Se realizaron pruebas unitarias caseras para cada módulo, además de las pruebas provistas por la catedra para las cuales necesitamos utilizar la librería Check para C. Para aquellos módulos donde se realizo una prueba casera, se creó un archivo de prueba, con la extensión .c, en la carpeta de **custom_tests**.

Primero nos asegurábamos de que el código diera los resultados esperados según los tests, y luego revisábamos que no existieran memory leaks en el código, utilizando **valgrind** como principal herramienta para encontrar y verificar estos errores de memoria.

## Problemas y soluciones durante el desarrollo
### Tests que fallaban
Algunos de los problemas en el desarrollo fueron algunos failed tests que nos costó poder pasar, principalmente en el módulo del execute. Había algunos tests que no entendíamos muy bien qué hacían y qué era lo que fallaba porque en la implementación prácticamente todo funcionaba.

### Diferencias entre Mybash y Bash
También hubo algunos detalles relacionados a qué tanta diferencia debería haber entre mybash y la bash de linux, ya que había algunas implementaciones de linux que eran muy difíciles de llevar a cabo en mybash y, que según la cátedra, no hacía falta implementar.

### Desconocimiento y aprendizaje
En varios modulos no sabiamos como encarar su implementación o que funciones utilizar para lograr lo solicitado, por lo que se necesito hacer una investigación previa utilizando la [Referencias y bibliografía](#referencias-y-bibliograf-a)

### Soluciones
Estos problemas los fuimos resolviendo siempre gracias al trabajo en conjunto. Realizamos varias reuniones entre todos para compartir posibles ideas y soluciones y retroalimentar lo aprendido. También, consultamos muchas dudas con la cátedra, que nos ayudó a esclarecer las cuestiones relacionadas a las diferencias entre mybash y la bash de linux, a entender mejor el comportamiento de los tests, saber donde buscar la información y a considerar distintos casos bordes relacionados a la implementación de la shell.

# Conclusiones 
Se logró implementar un shell básico, con la ejecución de comandos tanto en modo foreground y en modo background, con la redirección de entrada y salida estándar, y con la posibilidad de realizar multiples pipes (más de 2) entre comandos. 

En este proyecto aprendimos a como trabajar en equipo y organizar las tareas para generar una división equitativa. Esto nos sirvió para mejorar la coordinación dentro del grupo y para comprender como funciona un shell y ayudarnos en las dudas que tenia cada uno. 

También pudimos implementar bastantes conceptos vistos en el teórico, como la utilización de **fork** y **execvp**, la redirección de entrada y salida estándar, y la utilización de señales. 

En este proyecto también pudimos aprender y mejorar nuestro uso de git. 

# Referencias y bibliografía

- https://man7.org/linux/man-pages/ 
- https://pages.cs.wisc.edu/~remzi/OSTEP/
