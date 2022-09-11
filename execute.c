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

static void handle_redirection(scommand command) {
    char *redir_in = scommand_get_redir_in(command);
    char *redir_out = scommand_get_redir_out(command);
    int fd_in, fd_out;

    if (redir_in != NULL) {
        fd_in = open(redir_in, O_RDONLY);
        dup2(fd_in, STDIN_FILENO);
        close(fd_in);
    }

    if (redir_out != NULL) {
        fd_out = open(redir_out, O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
        dup2(fd_out, STDOUT_FILENO);
        close(fd_out);
    }
}

static void execute_single_command(pipeline p) {
    bool should_wait = pipeline_get_wait(p);
    scommand command = pipeline_front(p);

    if (builtin_alone(p)) {
        builtin_run(command);
    }

    handle_redirection(command);

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
