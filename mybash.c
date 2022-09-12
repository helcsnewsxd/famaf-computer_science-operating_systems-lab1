#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

#include "builtin.h"
#include "command.h"
#include "execute.h"
#include "parser.h"
#include "parsing.h"
#include "prompt.h"

int main(int argc, char *argv[]) {
    pipeline pipe = NULL;
    Parser input = parser_new(stdin);
    quit = false; // initialize variable (in builtin.h)

    while (!quit) {
        while ((waitpid(-1, NULL, WNOHANG)) > 0)
            ; // Code to clean zombie process of the process in the background

        show_prompt();
        pipe = parse_pipeline(input);

        if (pipe != NULL) {
            execute_pipeline(pipe);
            pipe = pipeline_destroy(pipe);
        }

        /* ¿Hay que salir luego de ejecutar? */
        if (parser_at_eof(input)) {
            quit = true;
            printf("\n");
        }
    }
    parser_destroy(input);
    input = NULL;
    return EXIT_SUCCESS;
}
