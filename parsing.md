# **PARSING**

## Explicación general

En la implementación del parsing utilizamos las funciones declaradas en el parser.h para poder detectar los argumentos de cada entrada y además categorizarlas según su funcionalidad: comando, redirección de salida/entrada, argumentos, etc.

En particular, la implementación del parsing se dividió en identificar los argumentos de tipo scommand y los argumentos seguidos de las pipelines.

En ambas implementaciones utilizamos ciclos de tal forma que se recorra los inputs dados por la terminal y hacer los push respectivos a cada tipo abstracto, detectando además argumentos inválidos o fin de archivos.

### Funciones implementadas
1. **parse_scommand**: Esta función recibe una variable de tipo Parser con los argumentos obtenidos por terminal y sintetiza la información en el TAD scommand. Esto lo hace usando condicionales y el enumerado *arg_kind_t*. En el condicional se identifica si se trata de un argumento o un archivo de redirección de entrada/salida.

```c
        if(arg_type == ARG_NORMAL){
            scommand_push_back(new_scmd,arg_cmd);
        }else if(arg_type == ARG_INPUT){
            scommand_set_redir_in(new_scmd,arg_cmd);
        }else if(arg_type == ARG_OUTPUT){
            scommand_set_redir_out(new_scmd,arg_cmd);
        }
```

Si por alguna razón el comando ingresado por terminal no es válido, y por ende, no es considerado en *new_scmd*, se elimina la memoria asignada a *new_scmd* para liberar ese espacio que no está en uso y se retorna NULL.

2. **parse_pipeline**: Implementamos esta función con un ciclo, a medida que fuimos identificando la presencia de pipes, pusheamos los comandos respectivos dentro del pipeline. En cada push, se volvía a chequear si el char siguiente era un pipe o no, este resultado lo ibamos guardando en la variable *another_pipe* que, en el caso de que fuera false, terminaba el ciclo ya que no había más pipes que leer. 
También jugó un rol muy importante la variable *error* ya que es la que nos indica si el comando seguido de los pipes es o no un comando válido.

```c
   while (another_pipe && !error) {
        // Push into a pipeline
        pipeline_push_back(result,act_scmd);

        // See final options (end line, pipe, background or EOF)
        // Is there a pipe?
        parser_op_pipe(p,&another_pipe);

        if(another_pipe){
            // Parsing the next scommand
            act_scmd = parse_scommand(p);

            error = (act_scmd == NULL); // Invalid command
        }
    }
```
Luego de haber leído todos los pipes del parser, usamos un condicional para analizar el resultado de la variable *error*:  
1. En el caso de no haber encontrado errores, lo que hicimos fue chequear si en lo que restaba del parser no había backgrounds o basura. Para esto, chequeamos que el carácter siguiente fuera o no un background, para así guardar éste resultado en la respectiva pipeline. Luego, nos fijamos si al final del parser había basura. Para ello, usamos la funcion auxiliar *parser_garbage* para borrar los caracteres basura restantes. Se actualizó la variable *error* con el valor del booleano *there_are_other_symbols* resultante de *parser_garbage*. Si error es true, entonces se encontraron símbolos basura; si error es false, entonces se llegó al eof y no hay símbolos basura.
2. En el caso de que haya habido errores, lo que se hizo fue eliminar el resto del parser para liberar su respectiva memoria.

```c
        if(!error){
        // Is there a background?
        bool there_is_background = false;
        parser_op_background(p,&there_is_background);
        pipeline_set_wait(result,!there_is_background);
        
        // Erase the rest of the parser
        bool there_are_other_symbols = false;
        parser_garbage(p,&there_are_other_symbols);
        error = there_are_other_symbols;
    }else{
        // Erase the rest of the parser
        bool there_are_other_symbols = false;
        parser_garbage(p,&there_are_other_symbols);
    }
```
Finalmente, el último condicional es el que chequea definitivamente si hubo un error en todo el proceso. En el caso de que haya habido un error, se destruye el pipeline ya que se trata de un input inválido. Caso contrario, se devuelve la variable result con el pipeline completo.

```c
if(error){
        result = pipeline_destroy(result);
        result = NULL;
    }
```