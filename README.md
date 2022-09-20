# **Laboratorio número 1 de Sistemas Operativos 2022 - Grupo 12 | FaMAF UNC**

## **Integrantes del grupo:**

- Lautaro Bachmann (lautaro.bachmann@mi.unc.edu.ar)
- Juan Bratti (juanbratti@mi.unc.edu.ar)
- Gonzalo Canavesio (gonzalo.canavesio@mi.unc.edu.ar)
- Emanuel Herrador (emanuel.nicolas.herrador@unc.edu.ar)


## **Índice**

 - [Introducción](#introducción)
 - [¿Cómo correr el código?](#¿cómo-correr-el-código)
 - [Detalles de implementación](#detalles-de-implementacion)
    - [Características implementadas](#caracteristicas-implementadas)
    - [Modularización](#modularización)
 - [Herramientas de Programación](#herramientas-de-programación)
 - [Desarrollo del proyecto](#desarrollo-del-proyecto)
 - [Conclusión](#conclusión)
 - [Webgrafía](#webgrafía)

## **Introducción**
Todo experto o amateur en el *mundo de Linux* tiene un conocimiento, aunque sea mínimo, sobre lo que es una terminal y cuáles son sus principales potenciales. Sin embargo, muy pocas veces nos hemos parado a pensar:
```
¿Por qué anda esto?
¿Cómo hacen para reconocer mi comando y permitir ejecutar varios al mismo tiempo?
¿Cómo se encargan de "linkear" todos los programas?
```
Por ello mismo es que en el presente informe, gracias al laboratorio ofrecido por la cátedra de *Sistemas Operativos 2022* y su posterior realización en el repositorio por parte del **Grupo 12**, se plantearán las respuestas claves a estas preguntas, se mostrará el procedimiento, las ideas, los problemas a enfrentar y el abanico de posibilidades que se produce en el desarollo de una *Shell* en un sistema tipo UNIX (más específicamente, **Linux**).

## **¿Cómo correr el código?**
### **Preparación**
1. Clonar repositorio.
2. Instalar librerias `glib` y `check`.

### **Compilación y ejecución**
Por motivos de comodidad y desarrollo, gracias al Makefile desarrollado por la cátedra, para compilar el código debemos ejecutar el comando
```sh
make
```
en el directorio principal (*so22lab1g12*), mientras que para ejecutarlo (luego de la compilación)
```sh
./mybash
```
Finalmente, para eliminar todos los archivos ejecutables (excepto lexer.o y parser.o), debe tenerse en cuenta el comando
```
make clean
```

## **Detalles de implementacion**
### **Caracteristicas implementadas**
#### **Funcionalidad basica**
Al momento de entrega, el proyecto cumple con todos las funcionalidades basicas pedidas por la cátedra, tales como:
 * Pasar el 100% del *unit-testing* (make test) dado para todo el proyecto
 * Manejar pipelines de, al menos, dos comandos
 * Manejar de manera adecuada la terminación de procesos lanzados en segundo plano sin dejar procesos zombies

#### **Puntos estrella**
Respecto a los puntos y funcionalidades extras de nuestro proyecto, se encuentran:
 * Generalizar el comando pipeline “|” a una cantidad arbitraria de comandos simples
 * Imprimir un prompt con información relevante (nombre de la shell, camino relativo y nombre de usuario).


### **Modularización**
La división del proyecto en módulos que se consideró por decisión del grupo fue la siguiente:

- MyBash: Módulo principal de la Shell
- Prompt: Implementación de una prompt con información relevante para el usuario
- Parsing: Procesamiento de la entrada del usuario utilizando el parser
- Parser: Implementación del TAD parser (dada por la cátedra)
- Execute: Ejecución de comandos y administración de las llamadas al sistema operativo (syscalls)
- Builtin: Implementación de interfaces para identificación e interacción con comandos internos
- Internal Commands: Implementación de los comandos internos
- Files Descriptors: Implementación de funciones para el manejo de redirecciones para las salidas/entradas de los comandos
- Command: Implementación de los TADs para representar los comandos simples y compuestos
- Strextra: Implementación de función auxiliar para el manejo de strings.

y cuya estructura se visualiza gracias al siguiente gráfico:

![diagrama de modulos](https://docs.google.com/drawings/d/e/2PACX-1vRHp-9hzIoFRShrxN-YHieV_GRyaoD86901jnYfCOg-2j05br2PHRQ_qR2JEl_N3SbRAXz0IJh87ZMN/pub?w=960&h=720)

El objetivo de haber desarrollado esta división y agregado más módulos apartes de los ofrecidos por la cátedra, fue hacer mucho más ordenado el código del programa de forma tal que cada parte se centre sólo en un objetivo en específico y, de ese modo, permitir una mayor *legibilidad* y *entendimiento* del proyecto tanto para los desarrolladores de este como para sus lectores.

#### **Explicación detallada de los módulos**

Bajo la idea de proponer un informe general, legible y llamativo del proyecto, la explicación detallada de los módulos más importantes va a encontrarse dentro de los archivos con formato Markdown linkeados a continuación:
 * **Command**
    * [**TAD Scommand**](/detailed_explanations/scommand.md)
    * [**TAD Pipeline**](/detailed_explanations/pipeline.md)
 * [**Parsing**](/detailed_explanations/parsing.md)
 * [**Execute**](/detailed_explanations/execute.md) (junto con **Files Descriptors**)
 * [**Builtin**](/detailed_explanations/builtin.md) (junto con **Internal Commands**)

## **Herramientas de Programación**
Las principales herramientas utilizadas por el grupo en la implementación, división y codificación de las secciones del programa MyBash fueron las siguientes:

### **Material teórico de estudio y preparación**
 - [**Operating Systems: Three Easy Pieces**: Process virtualization](https://pages.cs.wisc.edu/~remzi/OSTEP/), principalmente el capítulo número 5 (*Process API*) junto con su sección de *Homework Simulation* y *Homework Code*
 - [**Introducción a Unit Testing**](https://docs.google.com/document/d/111q867IwnaS6IlFdFEXvJqCIgZR2zaKt9Nq6e-Civd4/edit)
 - [**GLib Docs**](https://docs.gtk.org/glib/index.html)
### **Desarrollo**
- [Visual Studio Code](https://code.visualstudio.com/), editor de código
- [Live Share](https://code.visualstudio.com/learn/collaboration/live-share), extensión para el desarrollo colaborativo en tiempo real
- [ClangFormat](https://clang.llvm.org/docs/ClangFormat.html), formateador de código
    - Para obtener un estilo consistente se utilizó con un formato basado en el estilo de código usado por [LLVM](https://llvm.org/).
    - Se generó una configuración común de VSCode y se subió al repositorio para que el editor de texto de todos los integrantes del grupo se comporte de forma similar.
### **Compilacion**
- [GNU Make](https://www.gnu.org/software/make/)

### **Debugging**
- [GDB](https://sourceware.org/gdb/), depurador estándar para el compilador GNU.
- [Valgrind](https://valgrind.org/), conjunto de herramientas para la depuración de problemas de memoria y rendimiento de programas.

## **Desarrollo del proyecto**

### **Comunicación**
Luego de pasar por varias herramientas de comunicación y organización, tales como [Jira](https://www.atlassian.com/software/jira) y [Trello](https://trello.com/), se decidió por parte del grupo coordinar mediante la utilización de plataformas como [Discord](https://discord.com/) y [Telegram](https://telegram.org/) dada la afinidad y comodidad que nos presenta respecto a las tareas que debemos realizar.

### **Organización del trabajo**
Inicialmente, y por gran parte del desarrollo de MyBash, se realizó una división en dos subgrupos conformados del siguiente modo:
 - Grupo 1: Lautaro Bachmann y Gonzalo Canavesio
 - Grupo 2: Juan Bratti y Emanuel Nicolás Herrador

para facilitar el correcto y ordenado desarrollo de la implementación de los módulos evitando todo tipo de colisión de tareas. Esta idea se mantuvo durante la implementación de los módulos básicos hasta la *segunda versión del Execute*, desde la cual cada integrante ayudaba en proponer ideas, solucionar errores, mejorar el formato o comentarios del código, y realizar el informe.

#### **Live share**
A motivo de aclaración por los commits presentados en el proyecto, el Grupo 2 se basó en el uso de la herramienta Live Share por motivos de comodidad y para poder compilar y correr MyBash en la computadora (dado que en Windows se complicaba). Por ello mismo, los commits tienen aclarado, en su descripción o en los comentarios, quiénes fueron los coautores de los cambios realizados e implementados.

### **Workflow de desarrollo**
#### **Branches**
Bajo los mismos objetivos anteriormente mencionados, para lograr una mayor organización del código durante la implementación y desarrollo de MyBash, nuestro Workflow de trabajo se basó fuertemente en el uso de Branchs (ramas) dentro del repositorio en Bitbucket.

Para cada módulo, primero se creaba una nueva branch para desarrollarlo y cuando ya estaba totalmente implementado, pasaba los tests y no presentaba errores de memoria, era fusionado con la rama principal.

### **Pruebas utilizadas**
Se realizaron pruebas unitarias *caseras* para cada módulo, además de los tests provistos por la cátedra para los cuales necesitamos utilizar la librería [Check](https://libcheck.github.io/check/doc/doxygen/html/check_8h.html) para C. Respecto a aquellos módulos donde se realizó una prueba casera, se creó un archivo de prueba, con la extensión .c, en la carpeta de **Custom Tests**.

De este modo, podía probarse y asegurarse de que el código diera los resultados esperados según todo tipo de test, además de la revisión de memory leaks y heaps mediante la utilización de **Valgrind**.

### **Problemas y soluciones durante el desarrollo**
#### **Tests que fallaban**
El principal test con el que encontramos complicaciones a la hora del desarrollo del módulo de execute fue el encargado de controlar el correcto funcionamiento de las Syscalls en el proceso hijo luego de realizar una llamada a **fork()**.

```
test_execute.c:306:F:Functionality:test_pipe2_child1:0: Assertion 'mock_check_fd(3, KIND_CLOSED, NULL)' failed
```

Para solucionarlo, dado el planteamiento recursivo de la ejecución de los comandos, una vez generado el buffer de in/out mediante **pipe()** había que notar dos cuestiones principales:
 - El input del comando actual va a ser el read buffer de la pipe anterior, o STDIN en caso de ser el primero (salvo declaración explícita de redirección a files)
 - El read buffer creado en la pipe actual no va a ser usado, sino solo su contraparte write.

Como consecuencia, al no ser utilizado el read buffer actual, debía cerrarse en el proceso hijo antes de hacer la llamada a **execvp()** (en caso de no ser interno).

#### **Colisión de comandos Fore y Background**
El principal problema que se presentó en el desarrollo del proyecto pero se logró superar con éxito, fue el de la colisión en la ejecución de comandos Fore y Background en MyBash dado que, en una primera implementación, se bugueaba el prompt y el wait loop de comandos Foreground era "invadido" por procesos en segundo plano que finalizaban justo en ese momento. Un ejemplo es el siguiente:
```
xeyes &
ls -l | wc -l (y justo cuando está realizando la acción, cerrar xeyes)
```
La forma en la que logró superarse este caso fue mediante la utilización de un define para retornos de comandos en segundo plano
```c
#define EXIT_BACKGROUND 13
```
de modo que cada vez que se terminara de ejecutar un proceso en segundo plano, se retornara con exit el estado dado por EXIT_BACKGROUND.

Luego, esto permitió que al llamar a wait se pueda identificar mediante el argumento de *exit_signal* si efectivamente pertenecía a un comando en segundo plano o al actual en modo foreground para evaluar si era contabilizado o no. Además, para evitar los bugueos de la prompt, en el módulo MyBash se utilizó un waitpid loop para terminar efectivamente todos los procesos que, antes de dar el prompt, enviaron una señal de exit:
```c
while (waitpid(-1, NULL, WNOHANG) > 0) {
};
```
*Aclaración*: Aquí se utiliza la opción WNOHANG para que waitpid retorne de forma inmediata si ningún hijo realizó un exit.

## **Conclusión** 
Gracias al proceso de desarrollo e implementación de MyBash, un Shell tipo UNIX con funcionalidades básicas, ofrecido por la cátedra, es que podemos entender mucho más a fondo la complejidad que presenta realizar un programa tan importante como este dentro del contexto de los sistemas operativos. Es gracias a este proyecto que se le ha podido dar una revalorización de lo que nos ha parecido "común" o "usual" dentro del ámbito de Linux ya que, finalmente, sabemos cómo es que funciona, cómo se realizan las redirecciones a files, cuáles son las principales ideas y problemas, y cómo estas se combinan con la ejecución de procesos con Context Switch.

Además, producto de su elaboración en el marco de un grupo de cuatro integrantes, se ha dado una primera aproximación sobre cómo trabajar en equipo y organizar las tareas para generar una división equitativa dentro del ámbito de la *Programación* y *Desarrollo de Software*.

Finalmente, y no menos importante, recalcamos la gran utilidad de este proyecto para poder entender, junto con los *Homeworks* del libro [OSTEP](https://pages.cs.wisc.edu/~remzi/OSTEP/), cómo funcionan y se distribuyen los procesos dentro de un sistema operativo, y cuáles son las principales funciones que tienen las API de procesos tales como **fork()**, **execvp()**, **wait()**, **waitpid** y **pipe()**.

## **Webgrafía**

 - Arpaci-Dusseau, R. & Arpaci-Dusseau, A. (2018, agosto). Operating Systems: Three Easy Pieces. Recuperado 20 de septiembre de 2022, de https://pages.cs.wisc.edu/%7Eremzi/OSTEP/
 - Check: check.h File Reference. (s. f.). Recuperado 20 de septiembre de 2022, de https://libcheck.github.io/check/doc/doxygen/html/check_8h.html
 - GLib-2.0. (s. f.). Recuperado 20 de septiembre de 2022, de https://docs.gtk.org/glib/index.html
 - Linux man pages online. (s. f.). Recuperado 20 de septiembre de 2022, de https://man7.org/linux/man-pages/
 - Rocchietti, M., Teruel, M., Hobborg, L. & Almeida, G. (s. f.). Introducción a Unit Testing. Recuperado 20 de septiembre de 2022, de https://docs.google.com/document/d/111q867IwnaS6IlFdFEXvJqCIgZR2zaKt9Nq6e-Civd4/edit