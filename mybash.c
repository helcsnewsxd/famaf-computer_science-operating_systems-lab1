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
    quit = false;

    while (!quit) {
        // WNOHANG is an option to return immediately if no child has exited
        while (waitpid(-1, NULL, WNOHANG) > 0) {
        };
        // Preventing fore and background command collapse

        show_prompt();
        pipe = parse_pipeline(input);

        if (pipe != NULL) {
            execute_pipeline(pipe);
            pipe = pipeline_destroy(pipe);
        }

        quit = quit || parser_at_eof(input);
    }

    printf("\n");

    parser_destroy(input);
    input = NULL;
    return EXIT_SUCCESS;
}
