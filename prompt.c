#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "colors.h"
#include "prompt.h"
#include "strextra.h"

void show_prompt(void) {
    // Get username
    char *username = getenv("USER");

    // Get the directory location
    char *buffer = getcwd(NULL, 0);
    char *home_path = getenv("HOME");

    if (strncmp(buffer, home_path, strlen(home_path)) == 0) {
        memmove(buffer, buffer + strlen(home_path), strlen(buffer));
        char *auxiliar_to_remove = buffer;
        buffer = strmerge("~", buffer);
        free(auxiliar_to_remove);
    }

    // Print the command line

    printf(ANSI_COLOR_RED "(MyBash) " ANSI_COLOR_RED);
    printf(ANSI_COLOR_BLUE "[%s]" ANSI_COLOR_BLUE, buffer);
    printf(ANSI_COLOR_GREEN " @%s → " ANSI_COLOR_GREEN, username);
    printf(ANSI_COLOR_RESET);

    fflush(stdout);
}