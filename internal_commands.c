#include "internal_commands.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
 
struct cmd_type internal_cmd_list[count_internal_cmd] = {{"cd",cd_run},{"help",help_run},{"exit",exit_run}};

bool are_str_equal(char *s1, char *s2){
    return strcmp(s1, s2) == 0;
}

// FUNCTIONS TO RUN COMMANDS

void cd_run(scommand cmd){
    assert(are_str_equal(scommand_front(cmd),"cd"));  
    char * input_path = NULL;
    int cmd_length = scommand_length(cmd);

    if(cmd_length>2){ 
        printf("mybash: cd: demasiados argumentos\n"); // The cd command only has 1 or 2 args
    
    } else {

        scommand_pop_front(cmd); // Remove the "cmd" part of the cmd
        input_path = scommand_front(cmd); // Access to the first arg of the cmd
        int ret_chdir = 0;
        if(cmd_length == 1){
            input_path = getenv("HOME");
        } else {
            if(input_path[0] == '~'){
                ret_chdir = chdir(getenv("HOME"));
                memmove(input_path, input_path+1, strlen(input_path));
            }
            if(ret_chdir == 0){
                ret_chdir = chdir(input_path);
            }
        }
        if(ret_chdir != 0){ // if chdir has a error
            perror("mybash: cd: "); // Print the error message of chdir
        }
    }
}

void help_run(scommand cmd){
    assert(are_str_equal(scommand_front(cmd),"help"));    

    printf("%s",
                            "### MyBASH\n"
                            "- Proyecto para la materia Sistemas Operativos 2022, FAMAF UNC\n"
                            "Integrantes: Lautaro Bachmann, Juan Bratti, Gonzalo Canavesio, Emanuel Herrador\n"
                            "-----------------------------------\n"
                            "### COMANDOS INTERNOS IMPLEMENTADOS\n"
                            "Comando 'cd': Permite moverse entre directorios del sistema. Modo de uso: cd {ruta/_absoluta/_o/_relativa}\n"
                            "Comando 'exit': Finaliza con la ejecucion de la shell y todos los procesos asociados a la misma. Modo de uso: exit\n"
                            "Comando 'help': Envia un mensaje por salida estandar indicando el nombre de la shell, el nombre de sus autores y lista los comandos internos implementados con una breve descripcion de cada uno. Modo de uso: help\n"
                            ); // The text wall that we need to write
}

void exit_run(scommand cmd){
    assert(are_str_equal(scommand_front(cmd),"exit"));  
    quit = true; // Change the value of the global variable in built.h
}