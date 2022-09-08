# **BUILTIN**

## Explicación general

Implementamos las funciones **builtin_is_internal**, **builtin_alone** y **builtin_run**. Nos apoyamos fuertemente en la modularización para hacer más legible y ordenado el código.

### Implementaciones interesantes

Las implementaciones realizadas de **builtin_is_internal** y **builtin_alone** son intuitivas y faciles de entender. 
A continuación se da más detalles sobre las implementación de **builtin_run**, que es la más compleja, y después explicaré las modularizaciones/funciones de apoyo que utilizamos.

#### builtin_run

Separamos la ejecución de cada comando interno en su propia función dedicada, para facilitar la implementación de más comandos internos y mantener una estructura ordenada.

Dentro de la función **builtin_run** lo único que se hace es redirigir el comando interno a su función correspondiente, para ser ejecutado.

#### are_str_equal

Recibe dos strings y utilizando la funcion **strcmp** de la libreria **string.h** revisa si los dos strings son iguales y devuelve true, en el caso que los dos strings sean distintos devuelve false.
Esta función es muy utilizada durante todo el builtin, por lo que nos parecio muy importante modularizarla para facilitar la lectura del código y hacerlo más corto

#### cd_run

Esta funcion recibe el comando **cd** de tipo *scommand* y cambia el directorio donde trabaja el proceso al directorio definido en el comando.

##### Idea de la implementación

Primero se revisa si la cantidad de argumentos es correcta para el comando interno *cd* (menor o igual a 2), en caso de que no lo sea se imprime un mensaje de error y termina la ejecución.

En el caso de que la cantidad de argumentos sea correcta, se obtiene el argumento correspondiente al nuevo directorio donde queremos mover el proceso (en el caso de que solo haya 1 argumento en el comando, entonces el segundo argumento es *"."*). Luego utilizamos la función **realpath** de la libreria **#include <unistd.h>** para obtener la ruta absoluta a partir del argumento de la ruta (que puede ser absoluto o relativo) y lo guardamos en la variable *real_path*. Finalmente utilizamos esa variable con la función **chdir** para cambiar el directorio donde se encuentra el proceso de **mybash**.

##### Gestión de errores

Tanto en la función **real_path** como en **chdir** pueden devolver un error y no ejecutarse con normalidad debido a malos argumentos, accesos invalidos o falta de permisos. En esos casos se utilia la función **perror** para obtener el código de error dependiendo de cual fue la casa y poder imprimirlo por salida estandar para que el usuario este informado del porque su comando no pudo ejecutarse correctamente.

#### help_run

Es una implementación trivial, envia un mensaje por salida estandar indicando el nombre de la shell, el nombre de sus autores y lista los comandos internos implementados con una breve descripcion de cada uno

#### exit_run

Se modifica la variable global *quit* definida en **builtin.h**, a la cual se accede desde **mybash.c** y es la que define si termina o no la ejecución de mybash.

##### Gestion de procesos al hacer exit
- Al realizar el exit, los procesos zombies que aún queden esperando a que se solicite su estado pasaran de ser hijos del proceso "mybash" a ser hijos del proceso padre de "mybash" (no estoy seguro si es el init o el bash original de linux el que ahora se convierte en su padre), quien se encargará al recibir todos esos procesos zombie de pedirles su estado para permitir su cierre.
- Al realizar el exit, los procesos de fondo que aún se esten ejecutando seguiran ejecutandose, pero ahora pasarán a ser hijos del proceso padre de "mybash" (no estoy seguro si es el init o el bash original de linux el que ahora se convierte en su padre). Esto simula el funcionamiento del bash original de linux, donde los procesos de fondo siguen ejecutandose aunque se cierre la terminal utilizando exit()


