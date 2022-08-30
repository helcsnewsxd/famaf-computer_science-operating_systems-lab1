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

## Implementación del TAD Scommand

### Estructura de Datos

A partir de una lectura comprensiva del **command.h**, para la implementación del pipeline, decidimos partir desde la librería **GLib** para estructurar la tuberia de comandos, aprovechando así GQueue para representar la lista de comandos simples, y un booleano que indica si hay que esperar o continuar.

La estructura elegida fue la siguiente:
```c

    struct pipeline_s {
        GQueue∗ commands;
        bool should_wait;
    } pipeline_s;
        
```

### Implementaciones 

Consideramos que todas las implementaciones realizadas son intuitivas y faciles de entender. 
Intentamos aprovechar al máximo la libreria GLib para la realización de las funciones y dejamos comentarios explicativos para aclarar su uso.