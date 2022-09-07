#include "builtin.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static bool are_str_equal(char *s1, char *s2) { return strcmp(s1, s2) == 0; }

bool builtin_is_internal(scommand cmd) {
    assert(cmd != NULL);
    char *command = scommand_front(cmd);

    bool is_cd = are_str_equal("cd", command);
    bool is_help = are_str_equal("help", command);
    bool is_exit = are_str_equal("exit", command);
    return is_cd || is_help || is_exit;
}

bool builtin_alone(pipeline p) {
    assert(p != NULL);
    bool is_alone = pipeline_length(p) == 1;
    bool is_internal = builtin_is_internal(pipeline_front(p));
    return is_alone && is_internal;
}

static void cd_run(scommand cmd){
    assert(are_str_equal(scommand_front(cmd),"cd"));  
    char * input_path = NULL;
    char * real_path = NULL;
    if(scommand_length(cmd)>2){ 
        printf("cd: demasiados argumentos\n"); // The cd command only has 1 or 2 args
    } else {
        scommand_pop_front(cmd); // Remove the "cmd" part of the cmd
        input_path = scommand_front(cmd); // Access to the first arg of the cmd

        real_path = realpath(input_path,NULL); // Return the canonicalized absolute pathname

        if(real_path == NULL){ // if realpath dont has error
            perror("mybash: cd: "); // Print the error message of realpath   
        } else {     
            int ret_chdir = chdir(real_path); // Changes the current working directory of the calling process
            if(ret_chdir != 0){ // if chdir has a error
                perror("cd: "); // Print the error message of chdir
            }
        }
        free(real_path);
    }
}

static void help_run(scommand cmd){
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

static void exit_run(scommand cmd){
    assert(are_str_equal(scommand_front(cmd),"exit"));  
    quit = true; // Change the value of the global variable in built.h
}


void builtin_run(scommand cmd) {
    assert(builtin_is_internal(cmd));
    char *command = scommand_front(cmd);

    if(are_str_equal("cd", command)){
        cd_run(cmd);
    } else if (are_str_equal("help", command)){
        help_run(cmd);
    } else if (are_str_equal("exit", command)){
        exit_run(cmd);
    }
}