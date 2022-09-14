#include <assert.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#include "tests/syscall_mock.h"

#include "builtin.h"
#include "command.h"
#include "execute.h"
#include "files_descriptors.h"
#define DBG(message, ...) // printf("%s(): " message, __func__, ##__VA_ARGS__);

static void do_an_execute_single_command(pipeline apipe, int fd_read, int fd_write) {
    scommand cmd = pipeline_front(apipe);

    if (builtin_alone(apipe)) {
        builtin_run(pipeline_front(apipe));
        DBG("=== TERMINO el %s\n", scommand_front(cmd));
        // Finish the command execute
    } else {
        int pid = fork();
        SYS_ERROR(pid == -1, "ForkError");

        if (pid == 0) { // Child process
            // General execute of the inscruction in scommand
            DBG("=== Estoy viendo la instruccion %s\n", scommand_to_string(cmd));

            if (fd_read != -1) {
                DBG("=== %s hace el cambio de INPUT a fd %d\n", scommand_front(cmd), fd_read);
                // Change process' input to a file descriptor fd_read
                change_file_descriptor_input_from_fd(fd_read);
            } else {
                // if no valid read end has been passed close fork pipe read end
                close(3);
            }

            if (fd_write != -1) {
                DBG("=== %s hace el cambio de OUTPUT a fd %d\n", scommand_front(cmd), fd_write);
                // Changes process' output to file descriptor fd_write
                change_file_descriptor_output_from_fd(fd_write);
            }

            printf("=== fd_read:%d, fd_write:%d\n", fd_read, fd_write);
            // Changes the file descriptors if there is a redir in/out to a file
            DBG("=== Hago los cambios de fds en caso que redirija explicitamente a files\n");

            change_file_descriptor_input(scommand_get_redir_in(cmd));
            change_file_descriptor_output(scommand_get_redir_out(cmd));

            // Execute of the command with the corresponding files in input/output
            DBG("=== Me voy a ejecutar la instruccion %s\n", scommand_to_string(cmd));

            char **argv = scommand_to_char_list(cmd);
            execvp(argv[0], argv);
            SYS_ERROR(true, argv[0]);
        } else {
            // Waits for child to finish
            DBG("=== MI HIJO ES %d\n", pid);
            waitpid(pid, NULL, 0u);
            DBG("=== TERMINO el %s\n", scommand_front(cmd));
            // End all the command execute
        }
    }
}

static void do_an_execute_pipeline(pipeline apipe, int fd_read) {
    assert(apipe != NULL);

    DBG("=== SOY %s con input de Pipe en %d\n", scommand_front(pipeline_front(apipe)), fd_read);

    if (pipeline_length(apipe) == 1) { // BASE CASE
        do_an_execute_single_command(apipe, fd_read, -1);
        pipeline_pop_front(apipe);

    } else { // RECURSIVE CASE
        // Create the in and out pipe buffers
        int fd_pipe[2];
        int syscall_result = pipe(fd_pipe); // Output of the current command
        SYS_ERROR(syscall_result == -1, "PipeError");

        DBG("=== Creo la pipe %d de IN y %d de OUT\n", fd_pipe[0], fd_pipe[1]);

        do_an_execute_single_command(apipe, fd_read, fd_pipe[1]);
        DBG("=== Vuelvo\n");

        // We close the pipe's write file descriptor
        DBG("=== Cierro el fd's write de la pipe %d\n", fd_pipe[1]);
        close(fd_pipe[1]);

        pipeline_pop_front(apipe);

        do_an_execute_pipeline(apipe, fd_pipe[0]); // fd_pipe[0] is the input of the next command
        DBG("=== Volvere?\n");

        // We close the pipe's read file descriptor
        DBG("=== Cierro el fd's read de la pipe %d\n", fd_pipe[0]);
        close(fd_pipe[0]);
    }
}

void execute_pipeline(pipeline apipe) {
    // REQUIRES
    assert(apipe != NULL);

    // DBG("=== SOY la PIPELINE %s\n",pipeline_to_string(apipe));

    if (!pipeline_is_empty(apipe)) {

        int syscall_result;

        // ¡¡¡ All internal commands are ignored in a pipe !!!

        if (pipeline_get_wait(apipe)) {
            // Execution of pipeline in FOREGROUND mode
            // DBG("=== El pipeline entra en modo FOREGROUND\n");
            do_an_execute_pipeline(apipe, -1);
        } else { // Background &
            // Execution of pipeline in BACKGROUND mode
            // DBG("=== El pipeline entra en modo BACKGROUND\n");
            int pid = fork();
            SYS_ERROR(pid == -1, "ForkError");

            if (pid == 0) { // A child process
                // We close all the writing syscalls and we redirect the input to STDIN.
                int fd_act[2];
                syscall_result = pipe(fd_act);
                SYS_ERROR(syscall_result == -1, "PipeError");

                close(fd_act[1]);
                close(STDOUT_FILENO);
                change_file_descriptor_input_from_fd(fd_act[0]);

                do_an_execute_pipeline(apipe, -1);

                close(fd_act[0]);
            }
        }
    }
}
