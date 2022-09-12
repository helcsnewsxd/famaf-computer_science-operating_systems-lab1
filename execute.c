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

static void throw_error_msg_if(bool condition, char *message) {
    if (condition) {
        perror(message);
        exit(EXIT_FAILURE);
    }
}

static int spawn_subprocess(int in_fd, int out_fd, scommand cmd, bool should_wait) {
    char **argv = scommand_to_char_list(cmd);
    int syscall_result;
    pid_t pid = fork();
    throw_error_msg_if(pid == -1, "ForkError");
    if (pid == 0) {
        if (in_fd != STDIN_FILENO) {
            syscall_result = dup2(in_fd, STDIN_FILENO);
            throw_error_msg_if(syscall_result == -1, "Dup2Error");
            close(in_fd);
        }

        if (out_fd != STDOUT_FILENO) {
            syscall_result = dup2(out_fd, STDOUT_FILENO);
            throw_error_msg_if(syscall_result == -1, "Dup2Error");
            close(out_fd);
        }
        execvp(argv[0], argv);
        throw_error_msg_if(true, "ExecutionError");
    }
    if (should_wait) {
        syscall_result = waitpid(pid, NULL, 0);
        throw_error_msg_if(syscall_result == -1, "WaitError");
    }
    free(argv);
    return pid;
}

static int get_input_fd(char *redir_in) {
    int fd_in = STDIN_FILENO;

    if (redir_in != NULL) {
        fd_in = open(redir_in, O_RDONLY);
        throw_error_msg_if(fd_in == -1, "OpenError");
    }
    return fd_in;
}

static int get_output_fd(char *redir_out) {
    int fd_out = STDOUT_FILENO;
    if (redir_out != NULL) {
        fd_out = open(redir_out, O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
        throw_error_msg_if(fd_out == -1, "OpenError");
    }
    return fd_out;
}

static void execute_external_scommand(scommand command, bool should_wait) {
    char *redir_in = scommand_get_redir_in(command);
    char *redir_out = scommand_get_redir_out(command);

    // set default file descriptors
    int fd_in = get_input_fd(redir_in);
    int fd_out = get_output_fd(redir_out);

    spawn_subprocess(fd_in, fd_out, command, should_wait);
}

static void execute_single_command(pipeline p) {
    bool should_wait = pipeline_get_wait(p);
    scommand command = pipeline_front(p);

    if (builtin_alone(p)) {
        builtin_run(command);
    } else {
        execute_external_scommand(command, should_wait);
    }
}

static void execute_multiple_commands(pipeline p) {
    int syscall_result;
    int fd[2];
    int n = pipeline_length(p);
    bool should_wait = pipeline_get_wait(p);
    scommand cmd = pipeline_front(p);

    char *redir_in = scommand_get_redir_in(cmd);
    int in_fd = get_input_fd(redir_in);
    // The first process should get its input normally
    for (int i = 0; i < n - 1; ++i) {
        syscall_result = pipe(fd);
        throw_error_msg_if(syscall_result == -1, "PipeError");

        cmd = (i == 0) ? cmd : pipeline_front(p);
        // f [1] is the write end of the pipe, we carry `in` from the prev iteration.
        spawn_subprocess(in_fd, fd[1], cmd, should_wait);
        syscall_result = close(fd[1]);
        throw_error_msg_if(syscall_result == -1, "CloseError");
        // set input fd to read end of pipe
        in_fd = fd[0];
        pipeline_pop_front(p);
    }

    cmd = pipeline_front(p);
    char *redir_out = scommand_get_redir_out(cmd);

    // The first process should give its output normally
    int fd_out = get_output_fd(redir_out);

    // read fron previous pipe and output normally
    spawn_subprocess(in_fd, fd_out, cmd, should_wait);
    pipeline_pop_front(p);
}

void execute_pipeline(pipeline apipe) {
    // REQUIRES
    assert(apipe != NULL);

    unsigned int cmd_quantity = pipeline_length(apipe);

    if (cmd_quantity == 1u) {
        execute_single_command(apipe);
    } else if (cmd_quantity > 1u) {
        execute_multiple_commands(apipe);
    }
}
