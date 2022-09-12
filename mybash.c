#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

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
        show_prompt();
        pipe = parse_pipeline(input);

        if (pipe != NULL) {
            execute_pipeline(pipe);
            pipe = pipeline_destroy(pipe);
        }

        /* ¿Hay que salir luego de ejecutar? */
        if(parser_at_eof(input)){
            quit = true;
            printf("\n");
        }
    }
    parser_destroy(input);
    input = NULL;
    return EXIT_SUCCESS;
}
