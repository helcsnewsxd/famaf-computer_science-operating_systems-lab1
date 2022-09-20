[Ir al informe](../README.md)

# **BUILTIN**

Al hacer la implementación del modulo builtin nos basamos fuertemente en la modularización para hacer más legible y ordenado el código. 
Siguiendo esa linea, dividimos la implementación en 2 archivos: Builtin e Internal Commands

## **Sección de comandos internos**

Creamos un nuevo archivo llamado **internal_commands** donde modularizamos gran parte del código correspondiente al comando **builtin_run**.
Allí generamos un arreglo de tuplas llamado *internal_cmd_list*, donde cada tupla es un struct de tipo *cmd_type*, que está formado por el nombre del comando interno a ejecutar y un puntero a la función que ejecuta ese comando.
```c
struct cmd_type internal_cmd_list[count_internal_cmd] = {
    {"help", help_run}, {"cd", cd_run}, {"exit", exit_run}};
```
Esto nos permite generalizar luego el comando **builtin_run** a una cantidad n de funciones internas, sin necesidad de modificar el archivo **builtin.c**, solo se necesita modificar **internal_commands** para agregar en *internal_cmd_list* la tupla correspondiente a la nueva función y agregar también la nueva función para ejecutar 

### **are_str_equal**

Recibe dos strings y utilizando la funcion **strcmp** de la libreria **string.h** revisa si los dos strings son iguales y devuelve true, en el caso que los dos strings sean distintos devuelve false.
Esta función es muy utilizada durante todo el builtin, por lo que nos parecio muy importante modularizarla para facilitar la lectura del código y hacerlo más corto

### **help_run**

Es una implementación trivial, envia un mensaje por salida estandar indicando el nombre de la shell, el nombre de sus autores y lista los comandos internos implementados con una breve descripcion de cada uno. Se decidió implementarla con el uso de **fork()** para redireccionar correctamente el output a archivos en caso que sea especificado.

### **exit_run**

Se modifica la variable global *quit* definida en **internal_commands.h**, que es la que define si termina o no la ejecución de mybash en la ejecución desde **mybash.c**

#### **cd_run**

Esta funcion recibe el comando **cd** de tipo *scommand* y cambia el directorio donde trabaja el proceso al directorio definido en el comando.

##### **Idea de la implementación**

Primero se revisa si la cantidad de argumentos es correcta para el comando interno *cd* (menor o igual a 2), en caso de que no lo sea se imprime un mensaje de error y termina la ejecución.

En el caso de que la cantidad de argumentos sea 2, se guarda en *input_path* el elemento correspondiente a la ruta donde debe ejecutarse el proceso y se revisa si es "~", ya que en el Bash de linux al colocar ese simbolo te lleva a "HOME", pero si lo ponemos como input de la función **chdir** no funciona así, por lo que lo que hacemos es quitar ese simbolo con la función **memmove** y fusionar el resultado del *input_path* sin el simbolo "~" con la dirección del "HOME" (utilizando la función **getenv("HOME")**).

Una vez solucionado ese pequeño problema generado por el simbolo "~", lo que hacemos es utilizar la función auxiliar **do_cd_syscall** para que ejecute la función **chdir** con el path indicado, que cambia el directorio donde se encuentra el proceso de **mybash**, todo esto para que con el input recibido por **cd_run** se obtenga el mismo resultado que el dado por la Bash de Linux. 

En el caso de que la cantidad de argumentos sea 1, lo que se hace es directamente darle como argumento a **do_cd_syscall** la dirección de "HOME", utilizando la función **getenv("HOME")**.

La implementación de **do_cd_syscall** es para tener el código más ordenado y facilitar la legibilidad.
La función **chdir** viene por la libreria `#include <unistd.h>` y cambia el directorio de trabajo del actual proceso que la ejecuta.

##### **Gestión de errores**

La función **chdir** puede devolver un error y no ejecutarse con normalidad debido a malos argumentos, accesos invalidos o falta de permisos. En esos casos (cuando la función devuelve algo diferente de 0) se utiliza la función **perror** para obtener el código de error dependiendo de cual fue la causa y se lo imprime por salida estandar para que el usuario este informado del porque su comando no pudo ejecutarse correctamente.

##### **Gestion de procesos al hacer exit**
- Al realizar el exit, los procesos zombies que aún queden esperando a que se solicite su estado pasaran de ser hijos del proceso "mybash" a ser hijos del proceso padre de "mybash" (en una bash normal, sería el proceso init), quien se encargará al recibir todos esos procesos zombie de pedirles su estado para permitir su cierre.
- Al realizar el exit, los procesos de fondo que aún se esten ejecutando pasarán a ser hijos del proceso padre de "mybash" (en una bash normal, sería el proceso init). Esto simula el funcionamiento del bash original de linux, donde los procesos de fondo siguen ejecutandose aunque se cierre la terminal utilizando exit()


## **Sección builtin**

Acá implementamos las funciones **builtin_is_internal**, **builtin_alone** y **builtin_run**. La implementación de las dos primeras implementaciones son intuitivas y faciles de entender, pero la implementación de **builtin_run** si que se merece una explicación aparte

#### **builtin_run**

Separamos la ejecución de cada comando interno en su propia función dedicada, en el archivo **internal_commands.c**, para facilitar la implementación de más comandos internos y mantener una estructura ordenada.

```c
void builtin_run(scommand cmd) {
    assert(builtin_is_internal(cmd));

    char *command = scommand_front(cmd);

    unsigned int i = 0u;
    while (!are_str_equal(internal_cmd_list[i].name, command)) {
        ++i;
    }

    (*internal_cmd_list[i].function)(cmd);
}
```

Dentro de la función **builtin_run** lo único que se hace es redirigir el comando interno a su función correspondiente, para ser ejecutado. Esto se realiza mediante un while que itera hasta encontrar en *internal_cmd_list* la tupla correspondiente al comando interno que fue llamado, para luego ejecutarlo utilizando el puntero a la función (que está guardado en la tupla de *internal_cmd_list*)