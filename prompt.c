#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

// COLORS
#define ANSI_COLOR_RED     "\033[0;31m"
#define ANSI_COLOR_GREEN   "\x1b[1;32m"
#define ANSI_COLOR_BLUE    "\033[1;34m"
#define ANSI_COLOR_RESET   "\x1b[0m"

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