#include <assert.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#include "builtin.h"
#include "command.h"
#include "execute.h"

static void execute_single_command(pipeline p) {
    bool should_wait = pipeline_get_wait(p);
    scommand command = pipeline_front(p);

    if (builtin_alone(p)) {
        builtin_run(command);
    }

    int pid_fork = fork();
    if (pid_fork < 0) {
        perror("System error with fork: ");
        exit(EXIT_FAILURE);
    } else if (pid_fork == 0) {
        char **argv = scommand_to_char_list(command); // return scommand as string array
        char *cmd = argv[0];
        execvp(cmd, argv); // cmd it's supposed to be present on argv
        printf("Esto no tendria que aparecer\n");
    } else if (should_wait) {
        wait(NULL);
    }
}
static void execute_multiple_commands(pipeline p) {
    // TODO
    assert(p == p);
}

static void execute_pipeline_on_foreground(pipeline p) {
    unsigned int cmd_quantity = pipeline_length(p);

    if (cmd_quantity == 1u) {
        execute_single_command(p);
    } else if (cmd_quantity > 1u) {
        execute_multiple_commands(p);
    }
}

static void execute_pipeline_on_background(pipeline p) {
    // TODO
    assert(p == p);
}

void execute_pipeline(pipeline apipe) {
    // REQUIRES
    assert(apipe != NULL);

    bool should_wait = pipeline_get_wait(apipe);

    if (should_wait) {
        execute_pipeline_on_foreground(apipe);
    } else {
        execute_pipeline_on_background(apipe);
    }
}
