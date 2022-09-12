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

static int spawn_subprocess(int in_fd, int out_fd, scommand cmd) {
    char **argv = scommand_to_char_list(cmd);
    pid_t pid = fork();

    if (pid == 0) {
        if (in_fd != STDIN_FILENO) {
            dup2(in_fd, STDIN_FILENO);
            close(in_fd);
        }

        if (out_fd != STDOUT_FILENO) {
            dup2(out_fd, STDOUT_FILENO);
            close(out_fd);
        }

        return execvp(argv[0], argv);
    }

    return pid;
}

static void execute_external_scommand(scommand command) {
    char *redir_in = scommand_get_redir_in(command);
    char *redir_out = scommand_get_redir_out(command);

    int fd_in = open(redir_in, O_RDONLY);
    int fd_out = open(redir_out, O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);

    spawn_subprocess(fd_in, fd_out, command);
}

static void execute_single_command(pipeline p) {
    bool should_wait = pipeline_get_wait(p);
    scommand command = pipeline_front(p);

    if (builtin_alone(p)) {
        builtin_run(command);
    }

    execute_external_scommand(command);
    if (should_wait) {
        wait(NULL);
    }
}

static void execute_multiple_commands(pipeline p) {
    int i;
    int in_fd, fd[2];
    int n = 2; // pipeline_length(p);
    scommand cmd;

    // The first process should get its input from stdin
    in_fd = STDIN_FILENO;

    for (i = 0; i < n - 1; ++i) {
        pipe(fd);
        cmd = pipeline_front(p);

        // f [1] is the write end of the pipe, we carry `in` from the prev iteration.
        spawn_subprocess(in_fd, fd[1], cmd);
        close(fd[1]);

        // set input fd to read end of pipe
        in_fd = fd[0];
        pipeline_pop_front(p);
        cmd = NULL;
        wait(NULL);
    }

    // read fron previous pipe and output to stdout
    cmd = pipeline_front(p);
    spawn_subprocess(in_fd, STDOUT_FILENO, cmd);
    pipeline_pop_front(p);
    wait(NULL);
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

        // TODO wait for children
    } else {
        execute_pipeline_on_background(apipe);
    }
}
