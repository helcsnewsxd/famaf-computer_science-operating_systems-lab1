# **PIPELINE**

## Estructura del TAD Pipeline (vista externa)

Pipeline es una estructura que permite representar una secuencia de comandos simples de la forma
```     
    | scmd1 | scmd2 | ... | scmdn | 

```
pudiendo actualizarlos a medida que sea necesario.

Por ello mismo, en la librería command.h se proveen las siguientes funciones públicas:

### Constructores

1. **pipeline_new**    
2. **pipeline_push_back**

### Destructores

1. **pipeline_destroy**

### Operaciones

* Actualización del Pipeline
    1. **pipeline_pop_front**
    2. **pipeline_set_wait**
* Información del Pipeline
    1. **pipeline_front**
    2. **pipeline_is_empty**
    3. **pipeline_length**
    4. **pipeline_get_wait**
* Debugging
    1. **pipeline_to_string**

## Implementación del TAD Pipeline

### Estructura de Datos

A partir de una lectura comprensiva del **command.h**, para la implementación del pipeline, decidimos partir desde la librería **GLib** para estructurar la tuberia de comandos, aprovechando así GQueue para representar la lista de comandos simples, y un booleano que indica si hay que esperar o continuar.

La estructura elegida fue la siguiente:
```c

    struct pipeline_s {
        GQueue∗ commands;
        bool should_wait;
    } pipeline_s;
        
```

### Implementaciones interesantes

Casi todas las implementaciones realizadas son intuitivas y faciles de entender. 
A continuación se dan más detalles sobre las implementaciones más complejas

#### pipeline_to_string

Esta función, que provee la estructura de datos, requiere como argumento un pipeline (*tipo pipeline*), y devuelve un string (```char *```) que refiere a su representación en consola (bash).

##### Librerías requeridas

Para poder unir strings, usamos la librería **strextra.h**.

##### Idea de la implementación

Primero, para la implementación de la función, nos manejamos con una variable principal *result* (de tipo ```char *```), en la cual se iban a realizar las fusiones de la forma:
```c
        
        result = strmerge(result, ...);
        
```
Sin embargo, para no tener errores de memory heap, luego de cada merge se propuso liberar la memoria que correspondía al puntero anterior de *result*. Para ello, se utilizó una variable auxiliar *auxiliar_to_remove*, de modo que el esquema de mergeo sea el siguiente:
```c
        
        auxiliar_to_remove = result;
        auxiliar_to_add = scommand_to_string(g_queue_peek_nth(self->commands, i));
        result = strmerge(result, auxiliar_to_add);
        
```

Teniendo esto en cuenta, el problema se dividió en dos partes

* Argumentos: En este caso, se utilizó un for loop para concatenar el contenido de *result* con cada argumento perteneciente a la GQueue respecto al orden de entrada. Eliminando, en cada instancia, la memoria que correspondía al puntero anterior de esta variable principal. Luego se agrega un espacio para mejorar la legibilidad.

* Should_wait: Por último, en el caso que el proceso del pipeline se vaya a ejecutar en el background (no hace falta esperar la ejecución), simplemente agregamos en *result* el operador **&**.