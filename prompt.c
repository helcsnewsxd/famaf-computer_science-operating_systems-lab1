#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

// COLORS
#define ANSI_COLOR_RED     "\033[0;31m"
#define ANSI_COLOR_RED2     "\033[1;31m"
#define ANSI_COLOR_GREEN   "\x1b[1;32m"
#define ANSI_COLOR_BLUE    "\033[1;34m"
#define ANSI_COLOR_RESET   "\x1b[0m"
#define BLK "\033[0;30m"
#define GRN "\033[0;32m"
#define YEL "\033[0;33m"
#define MAG "\033[0;35m"
#define CYN "\033[0;36m"
#define WHT "\033[1;37m"

// UNDERLINED
#define UWTH "\033[4;37m"

#include "prompt.h"
#include "strextra.h"

void show_prompt(void) {
    // Get username
    char * username = getenv("USER");

    // Get the directory location
    char * buffer = getcwd(NULL,0);
    char * home_path = getenv("HOME");
    
    if(strncmp(buffer,home_path,strlen(home_path)) == 0){
        memmove(buffer, buffer+strlen(home_path), strlen(buffer));
        char * auxiliar_to_remove = buffer;
        buffer = strmerge("~",buffer);
        free(auxiliar_to_remove);
    }
    
    // Print the command line

    printf(ANSI_COLOR_RED "(MyBash) " ANSI_COLOR_RED);
    printf(ANSI_COLOR_BLUE "[%s]" ANSI_COLOR_BLUE,buffer);
    printf(ANSI_COLOR_GREEN " @%s → " ANSI_COLOR_GREEN,username);
    printf(ANSI_COLOR_RESET);

    fflush(stdout);
}

void prompt_help(void) {
    printf("\n===================================================================\n");
    printf(ANSI_COLOR_RED2 "\n(MyBash) -> A bash scripting project\n" ANSI_COLOR_RED2);
    printf(ANSI_COLOR_RESET);
    printf("Proyecto para la materia Sistemas Operativos 2022, FAMAF UNC\n\n");
    printf(WHT "Integrantes:\n" WHT);
    printf(ANSI_COLOR_RESET);
    printf(" - Lautaro Bachmann\n - Juan Bratti\n - Gonzalo Canavesio\n - Emanuel Herrador\n");
    printf(WHT "\nComandos Internos Implementados:\n" WHT);
    printf(ANSI_COLOR_RESET);
    printf(
    " ‣ Comando 'cd':   Permite moverse entre directorios del sistema\n"
    "                   Modo de uso: cd {ruta/_absoluta/_o/_relativa}\n"
    " ‣ Comando 'exit': Finaliza la ejecucion de MyBash y los procesos asociados no backgrounds\n"
    "                   Modo de uso: exit\n"
    " ‣ Comando 'help': Envia un mensaje por salida estandar indicando:\n"
    "                       - Nombre de la shell\n"
    "                       - Nombre de sus autores\n"
    "                       - Lista de comandos internos implementados con breve descripcion\n"
    "                   Modo de uso: help\n"
    );
    printf("\n===================================================================\n");
}