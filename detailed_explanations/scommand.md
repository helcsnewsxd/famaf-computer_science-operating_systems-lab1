# **SCOMMAND**

## Estructura del TAD Scommand (vista externa)

Scommand es una estructura que permite representar comandos simples de la forma
```     
                cmd arg1 arg2 arg3 ... argn < input > output
```
pudiendo actualizarlos a medida que sea necesario.

Por ello mismo, en la librería command.h se proveen las siguientes funciones públicas:

### Constructores

1. **scommand_new**    
2. **scommand_push_back**
3. **scommand_set_redir_in**
4. **scommand_set_redir_out**

### Destructores

1. **scommand_destroy**

### Operaciones

* Actualización del Scommand
    1. **scommand_pop_front**
* Información del Scommand
    1. **scommand_front**
    2. **scommand_get_redir_in**
    3. **scommand_get_redir_out**
    4. **scommand_is_empty**
    5. **scommand_length**
* Debugging
    1. **scommand_to_string**

## Implementación del TAD Scommand

### Estructura de Datos

A partir de una lectura comprensiva del **command.h**, para la implementación de los comandos simples, decidimos partir desde la librería **GLib** para estructurar cada argumento en una serie de campos, aprovechando así GQueue para representar la lista de argumentos, y punteros a char para representar cada redirección (in & out).

La estructura elegida fue la siguiente:
```c

        struct scommand_s{
            GQueue * sc_arguments;
            char * sc_file_input;
            char * sc_file_output;
        };
        
```

### Implementaciones interesantes

#### scommand_to_string

Esta función, que provee la estructura de datos, requiere como argumento un comando simple (*tipo scommand*), y devuelve un string (```char *```) que refiere a su representación en consola (bash).

##### Librerías requeridas

Para poder mergear strings, usamos la librería **strextra.h**.

##### Idea de la implementación

Primero, para la implementación de la función, nos manejamos con una variable principal *sc_shell_representation* (de tipo ```char *```), en la cual se iban a realizar los merge de la forma:
```c
        
        sc_shell_representation = strmerge(sc_shell_representation, ...);
        
```
Sin embargo, para no tener errores de memory heap, luego de cada merge se propuso liberar la memoria que correspondía al puntero anterior de *sc_shell_representation*. Para ello, se utilizó una variable auxiliar *auxiliar_to_remove*, de modo que el esquema de mergeo sea el siguiente:
```c
        
        auxiliar_to_remove = sc_shell_representation;
        sc_shell_representation = strmerge(sc_shell_representation, ...);
        free(auxiliar_to_remove);
        
```

Teniendo esto en cuenta, el problema se dividió en dos partes

* Argumentos: En este caso, se utilizó un for loop para concatenar el contenido de *sc_shell_representation* con cada argumento perteneciente a la GQueue respecto al orden de entrada. Eliminando, en cada instancia, la memoria que correspondía al puntero anterior de esta variable principal. Además, para dar mayor legibilidad, se mergeó el string principal con un espacio en blanco " ".

* Filenames: Por último, en el caso de que si hubiera redireccionamientos, simplemente mergeamos en *sc_shell_representation* el operador **<** ó **>** con el correspondiente *filename*.