#include "builtin.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

bool builtin_is_internal(scommand cmd) {
    assert(cmd != NULL);
    char *command = scommand_front(cmd);
    bool result = false;

    bool is_cd = strcmp("cd", command);
    bool is_help = strcmp("help", command);
    bool is_exit = strcmp("exit", command);
    return is_cd || is_help || is_exit;
}

bool builtin_alone(pipeline p) {
    assert(p != NULL);
    bool is_alone = pipeline_length(p) == 1;
    bool is_internal = builtin_is_internal(pipeline_front(p));
    return is_alone && is_internal;
}

static void cd_run(scommand cmd){
    char * input_path = NULL;
    char * real_path = NULL;
    if(scommand_length(cmd)>2){ 
        printf("cd: demasiados argumentos\n"); // The cd command only has 1 or 2 args
    } else {
        scommand_pop_front(cmd); // Remove the "cmd" part of the cmd
        input_path = scommand_front(cmd); // Access to the first arg of the cmd

        int ret_chdir, ret_real_path;
        real_path = realpath(input_path,NULL); // Return the canonicalized absolute pathname

        if(real_path != NULL){ // if realpath dont has error
            ret_chdir = chdir(real_path); // Changes the current working directory of the calling process
            if(ret_chdir != 0){ // if chdir has a error
                perror("cd: "); // Print the error message
            }
        } else {
            perror("realpath: "); // Print the error message
        }
        free(real_path);
    }
}

static void help_run(scommand cmd){
    char shell_name = "MyBASH \n- Proyecto para la materia Sistemas Operativos 2022, FAMAF UNC";
    char authors_names = "Lautaro Bachmann, Juan Bratti, Gonzalo Canavesio, Emanuel Herrador";
    char cd_explanation = "Comando 'cd': Permite moverse entre directorios del sistema.\n - Modo de uso: cd {ruta/_absoluta/_o/_relativa}";
    char exit_explanation = "Comando 'exit': Finaliza con la ejecucion de la shell y todos los procesos asociados a la misma\n - Modo de uso: exit";
    char help_explanation = "Comando 'help': Envia un mensaje por salida estandar indicando el nombre de la shell, el nombre de sus autores y lista los comandos internos implementados con una breve descripcion de cada uno\n - Modo de uso: help";

    printf("%s\n%s\n%s\n%s\n%s\n",shell_name,authors_names,cd_explanation,exit_explanation,help_explanation);
}

static void exit_run(scommand cmd){
    quit = true; // Change the value of the global variable in built.h
}


void builtin_run(scommand cmd) {
    assert(builtin_is_internal(cmd));
    char *command = scommand_front(cmd);

    if(strcmp("cd", command)){
        cd_run(cmd);
    } else if (strcmp("help", command)) {
        help_run(cmd);
    } else if (strcmp("exit", command)){
        exit_run(cmd);
    }
}