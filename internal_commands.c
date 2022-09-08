#include "builtin.h"
#include "internal_commands.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// INITIALIZATION OF THE ARRAY

internal_cmd_list = {{"cd",cd_run},{"help",help_run},{"exit",exit}};
count_internal_cmd = 3;

bool are_str_equal(char *s1, char *s2){
    return strcmp(s1, s2) == 0;
}

// FUNCTIONS TO RUN COMMANDS

void cd_run(scommand cmd){
    // MAKE THIS
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