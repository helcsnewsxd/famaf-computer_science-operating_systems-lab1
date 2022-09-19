# **EXECUTE**
## Explicación general

Éste módulo esta encargado de invocar a las syscalls necesarias para ejecutar los comandos ingresados por consola, teniendo en cuenta los detalles de implementación relacionados a la ejecución de comandos en un pipeline haciendo uso de tuberías y las puntas de escrituras y lecturas.

## Funciones implementadas

### execute_pipeline
Función principal que ejecuta un pipeline. Toma como argumento un pipeline y modulariza la ejecución del mismo dependiendo de si se ejecuta en modo background/foreground.

Para determinar su modo de ejecución, se divide en casos utilizando un condicional. En el modo foreground, se guarda en *cnt_child_process* la cantidad de comandos a ejecutar para que cuando se haga el llamado de *do_an_execute_pipeline* luego se espere a que cada comando termine su ejecución adecuadamente utilizando el while loop. En el llamado a *do_an_execute_pipeline* se pone como segundo argumento -1 ya que el input viene en la pipeline, es el comienzo de ejecución y por lo tanto no hago uso de pipes/tuberías.

```c
// MODO FOREGROUND
 int cnt_child_process; // CANTIDAD DE PROCESOS HIJOS NECESARIOS
            if (builtin_alone(apipe)) {
                cnt_child_process = 0;
            } else {
                cnt_child_process = pipeline_length(apipe);
            }

            // EJECUTA PIPELINE
            do_an_execute_pipeline(apipe, -1); 

            // SE ESPERA A CADA CHILD PROCESS QUE TERMINE.
            while (cnt_child_process > 0) {
                int exit_signal = -1;
                wait(&exit_signal);
                if (exit_signal != EXIT_BACKGROUND) {
                    cnt_child_process--;
                }
            }
```

En el modo background, como los comandos corren en el fondo y la terminal no debe esperar a que éstos terminen su ejecución para ejecutar el siguiente, se ejecutan los comandos haciendo la llamada de *do_an_execute_pipeline* en un ambiente aislado usando la syscall fork. Aquí también se usa la variable *cnt_child_process* para contabilizar los procesos hijos creados.

En este caso, se declaran los file descriptors actuales en fd_act y también se hace la llamada a la syscall pipe para generar el buffer en donde los comandos van a intercambiar los outputs e inputs de ser necesario. Notar que aquí no se usa la syscall execvp porque la terminal no espera a que el comando termine de correr para porder correr otro.

```c
int pid = fork();
            SYS_ERROR(pid == -1, "ForkError");

            if (pid == 0) { 
                int fd_act[2]; // DECLARO FILE DESCRIPTORS ACTUALES
                syscall_result = pipe(fd_act); // CREO TUBERÍA
                SYS_ERROR(syscall_result == -1, "PipeError");

                int cnt_child_process;
                if (builtin_alone(apipe)) {
                    cnt_child_process = 0;
                } else {
                    cnt_child_process = pipeline_length(apipe);
                }

                // fd_act[0] PARA LEER DEL INPUT DEL PRIMER COMANDO EN LA TUBERÍA
                do_an_execute_pipeline(apipe, fd_act[0]);

                while (cnt_child_process > 0) {
                    wait(NULL);
                    cnt_child_process--;
                }

                // CIERRO LECTURA Y ESCRITURA
                close(fd_act[0]);
                close(fd_act[1]);

                printf("=== Process with PID %d finished === \n", getpid());

                exit(EXIT_BACKGROUND);
            } else {
                printf("=== Process with PID %d initialized === \n", pid);
            }
```

Las llamadas a SYS_ERROR se realizan para verificar que las llamadas a las syscalls hayan sido exitosas.

### do_an_execute_pipeline
Función estática utilizada para la ejecución de los comandos en un pipeline. Toma dos argumentos, una pipeline con los comandos faltantes por ejecutar y el file descriptor de lectura. Implementación pensada de forma recursiva, dividiendo así en caso base (ejecución de un solo comando) o caso recursivo (pipeline con tamaño mayor a uno).

En el caso base, el tamaño de la pipeline es igual a 1. Aquí se llama a la función *do_an_execute_single_command* y se hace un *pipeline_pop_front* para el único elemento de la pipeline. En éste caso, los dos últimos argumentos de *do_an_execute_single_command* son -1 porque se trata del último y único comando que queda en la pipeline.

```c
// CASO BASE
do_an_execute_single_command(apipe, fd_read, -1, -1);
pipeline_pop_front(apipe);
```

En el caso recursivo, se crean los file descriptors y la tubería entre los dos comandos usando nuevamente la syscall pipe. Luego se usa la función *do_an_execute_single_command* para ejecutar el primer comando en la pipe, pasando como argumentos los file descriptors correspondientes para redigir correctamente los outputs e inputs. fd_read es la punta de lectura, fd_pipe[1] es la punta de escritura y fd_pipe[0] es la punta de lectura del siguiente comando.
 
Luego, se cierra el file descriptor de escritura y se hace el *pipeline_pop_front* para sacar el comando ejecutado y se hace la llamada recursiva con el comando siguiente y los restantes llamando a *do_an_execute_pipeline*. Así se seguirá hasta llegar al último comando y caer en el caso base.

```c
        int fd_pipe[2];
        int syscall_result = pipe(fd_pipe); // SE CREA TUBERÍA
        SYS_ERROR(syscall_result == -1, "PipeError");

        do_an_execute_single_command(apipe, fd_read, fd_pipe[1], fd_pipe[0]); // SE EJECUTA COMANDO

        // SE CIERRA ESCRITURA
        close(fd_pipe[1]);

        // SE ELIMINA EL COMANDO EJECUTADO
        pipeline_pop_front(apipe);

        // SE HACE LA LLAMADA RECURSIVA CON INPUT DEL SIGUIENTE COMANDO
        do_an_execute_pipeline(apipe, fd_pipe[0]);

        // CIERRO LECTURA
        close(fd_pipe[0]);
```

### do_an_execute_single_command
Función estática utilizada para la ejecución de un sólo comando presente en la pipeline. Implementación útil para la modularización del caso base en la función *do_an_execute_pipeline*. Recibe como argumentos la pipeline con el comando a ejecutar, el file descriptor de lectura, el de escritura, y el de lectura que todavía no se ha utilizado, es decir, el de lectura del siguiente comando.

Esta función utiliza un condicional para correr el comando con *builtin_run* en el caso de que el single command sea uno interno. Si no es interno, ejecuta el comando en un ambiente aislado usando la syscall fork(), en la cual primero cambiara los redireccionamientos y las lecturas de input y output llamando a las funciones correspondientes del módulo *file_descriptors*.

```c
 int pid = fork();
        SYS_ERROR(pid == -1, "ForkError");

        if (pid == 0) { // Child process
            // General execute of the inscruction in scommand

            if (fd_read_not_used != -1) {
                // Close the file descriptor
                close(fd_read_not_used);
            }

            if (fd_read != -1) {
                // Change process' input to a file descriptor fd_read
                change_file_descriptor_input_from_fd(fd_read);
            }

            if (fd_write != -1) {
                // Changes process' output to file descriptor fd_write
                change_file_descriptor_output_from_fd(fd_write);
            }

            // Changes the file descriptors if there is a redir in/out to a file

            change_file_descriptor_input(scommand_get_redir_in(cmd));
            change_file_descriptor_output(scommand_get_redir_out(cmd));

            // Execute of the command with the corresponding files in input/output

            char **argv = scommand_to_char_list(cmd);
            execvp(argv[0], argv);
            SYS_ERROR(true, argv[0]);
```
- Si fd_read_not_used es distinto de -1, lo cierro para que el programa no espere un input y luego tenga problemas para cerrar en el execvp.
- Si fd_read es distinto de -1, entonces debo leer el input de la pipe, por lo tanto llamo a *change_file_descriptor_input_from_fd*.
- Si fd_write es distinto de -1, entonces debo escribir en el input del comando siguiente en la pipe, por lo tanto llamo a *change_file_descriptor_output_from_fd*.

Finalmente hago la llamada a la syscall execvp para ejecutar el comando.

## Funciones módulo *file_descriptors*
Las funciones de este módulo las usamos de forma auxiliar para realizar los correspondientes cambios en las puntas de lectura y escrituras de cada comando a ejecutar en la pipeline.

### change_file_descriptor_input(char *redir_in)
Esta función, si *redir_in* != NULL, cambia qué toma el comando como input, para ello, el input del comando pasa a ser lo que apunte *redir_in*. Para lograr esto, la syscall *open* obtiene la punta de lectura correspondiente a *redir_in* en fd_act, y se utiliza *dup2* para que STDIN pase a apuntar a fd_act, y así el input en vez de tomarse por la entrada estándar, se toma por *redir_in*.

```c
 if (redir_in != NULL) {
        // SE CAMBIA LA PUNTA DE LECTURA
        int fd_act = open(redir_in, O_RDONLY, 0u);
        SYS_ERROR(fd_act == -1, "OpenError");

        int syscall_result = dup2(fd_act, STDIN_FILENO);
        SYS_ERROR(syscall_result == -1, "Dup2Error");

        syscall_result = close(fd_act);
        SYS_ERROR(syscall_result == -1, "CloseError");
    }
```

Aquí, O_RDONLY es la flag necesaria para indicar que se trata de una punta de lectura. Luego se cierra la lectura usando la syscall close().

### change_file_descriptor_output(char *redir_out)
Esta función, si *redir_out* != NULL, cambia a dónde se escribe el output del comando, para ello, el output se escribirá a lo que apunte *redir_out*. Para lograr esto, la syscall *open* obtiene la punta de escritura correspondiente a *redir_out* en fd_act, y se utiliza *dup2* para que STDOUT pase a apuntar a fd_act, y así el output en vez de salir por pantalla se guarda en *redir_out*.
```c
    if (redir_out != NULL) {
        // We've to change the writer's file descriptor
        int fd_act = open(redir_out, O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
        SYS_ERROR(fd_act == -1, "OpenError");

        int syscall_result = dup2(fd_act, STDOUT_FILENO);
        SYS_ERROR(syscall_result == -1, "Dup2Error");

        syscall_result = close(fd_act);
        SYS_ERROR(syscall_result == -1, "CloseError");
    }
```
Aquí O_WRONLY es la flag que indica que se trata de una punta de escritura. O_CREATE sirve para indicar que si el salida no existe, entonces debe crearse. S_IRUSR da permisos de lectura y S_IWUSR da permisos de escritura.

### change_file_descriptor_input_from_fd(int fd_act)
Esta función cambia el input de la pipe/tubería. Sirve para indicar qué input de qué comando se lee en la tubería. Aquí, dup2 cambia a dónde apunta STDIN para que apunte a fd_act y cambie el input del comando. Luego se cierra para que el programa pueda concluir sin problemas.
```c
    int syscall_result = dup2(fd_act, STDIN_FILENO);
    SYS_ERROR(syscall_result == -1, "Dup2Error");

    syscall_result = close(fd_act);
    SYS_ERROR(syscall_result == -1, "CloseError");
```

### change_file_descriptor_output_from_fd(int fd_act)
Esta función cambia para dónde va el output de la pipe/tubería. Sirve para indicar qué comando leerá el output. Aquí, dup2 cambia a dónde apunta STDOUT para que apunte a fd_act y cambie a dónde se escribe el output del comando. Luego se cierra para que el programa pueda concluir sin problemas.
```c
    int syscall_result = dup2(fd_act, STDOUT_FILENO);
    SYS_ERROR(syscall_result == -1, "Dup2Error");

    syscall_result = close(fd_act);
    SYS_ERROR(syscall_result == -1, "CloseError");
```