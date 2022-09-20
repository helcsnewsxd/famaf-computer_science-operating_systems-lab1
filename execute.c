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

static void do_an_execute_single_command(pipeline apipe, int fd_read, int fd_write,
                                         int fd_read_not_used) {
    scommand cmd = pipeline_front(apipe);

    if (builtin_alone(apipe)) {
        builtin_run(pipeline_front(apipe));
        // Finish the command execute
    } else {
        int pid = fork();
        SYS_ERROR(pid == -1, "ForkError");

        if (pid == 0) { // Child process
            // General execute of the inscruction in scommand

            if (fd_read_not_used != -1) {
                // Close the file descriptor
                close(fd_read_not_used);
            }

            if (fd_read != -1) {
                // Change process' input to a file descriptor fd_read
                change_file_descriptor_input_from_fd(fd_read);
            }

            if (fd_write != -1) {
                // Changes process' output to file descriptor fd_write
                change_file_descriptor_output_from_fd(fd_write);
            }

            // Changes the file descriptors if there is a redir in/out to a file

            change_file_descriptor_input(scommand_get_redir_in(cmd));
            change_file_descriptor_output(scommand_get_redir_out(cmd));

            // Execute of the command with the corresponding files in input/output

            char **argv = scommand_to_char_list(cmd);
            execvp(argv[0], argv);
            SYS_ERROR(true, argv[0]);
        }
    }
}

static void do_an_execute_pipeline(pipeline apipe, int fd_read) {
    assert(apipe != NULL);

    if (pipeline_length(apipe) == 1) { // BASE CASE
        do_an_execute_single_command(apipe, fd_read, -1, -1);
        pipeline_pop_front(apipe);

    } else { // RECURSIVE CASE
        // Create the in and out pipe buffers
        int fd_pipe[2];
        int syscall_result = pipe(fd_pipe); // Output of the current command
        SYS_ERROR(syscall_result == -1, "PipeError");

        do_an_execute_single_command(apipe, fd_read, fd_pipe[1], fd_pipe[0]);

        // We close the pipe's write file descriptor
        close(fd_pipe[1]);

        pipeline_pop_front(apipe);

        do_an_execute_pipeline(apipe, fd_pipe[0]); // fd_pipe[0] is the input of the next command

        // We close the pipe's read file descriptor
        close(fd_pipe[0]);
    }
}

void execute_pipeline(pipeline apipe) {
    // REQUIRES
    assert(apipe != NULL);

    if (!pipeline_is_empty(apipe)) {

        int syscall_result;

        // ¡¡¡ All internal commands are ignored in a pipe !!!

        if (pipeline_get_wait(apipe)) {
            // Execution of pipeline in FOREGROUND mode

            int cnt_child_process;
            if (builtin_alone(apipe)) {
                cnt_child_process = 0;
            } else {
                cnt_child_process = pipeline_length(apipe);
            }

            do_an_execute_pipeline(apipe, -1);

            // In this block of waiting, we have to prevent with fore and background command
            // collapse
            while (cnt_child_process > 0) {
                int exit_signal = -1;
                wait(&exit_signal);
                if (exit_signal != EXIT_BACKGROUND) {
                    cnt_child_process--;
                }
            }

        } else { // Background &
            // Execution of pipeline in BACKGROUND mode
            int pid = fork();
            SYS_ERROR(pid == -1, "ForkError");

            if (pid == 0) { // A child process
                // We close all the writing syscalls and we redirect the input to STDIN.
                int fd_act[2];
                syscall_result = pipe(fd_act);
                SYS_ERROR(syscall_result == -1, "PipeError");

                int cnt_child_process;
                if (builtin_alone(apipe)) {
                    cnt_child_process = 0;
                } else {
                    cnt_child_process = pipeline_length(apipe);
                }

                do_an_execute_pipeline(apipe, fd_act[0]);

                while (cnt_child_process > 0) {
                    wait(NULL);
                    cnt_child_process--;
                }

                close(fd_act[0]);
                close(fd_act[1]);

                printf("=== Process with PID %d finished === \n", getpid());

                exit(EXIT_BACKGROUND);
            } else {
                printf("=== Process with PID %d initialized === \n", pid);
            }
        }
    }
}
