#include <assert.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "files_descriptors.h"
#include "tests/syscall_mock.h"

void change_file_descriptor_input(char *redir_in) {
    if (redir_in != NULL) {
        // We've to change the reader's file descriptor
        int fd_act = open(redir_in, O_RDONLY, 0u);
        SYS_ERROR(fd_act == -1, "OpenError");

        int syscall_result = dup2(fd_act, STDIN_FILENO);
        SYS_ERROR(syscall_result == -1, "Dup2Error");

        syscall_result = close(fd_act);
        SYS_ERROR(syscall_result == -1, "CloseError");
    }
}

void change_file_descriptor_output(char *redir_out) {
    if (redir_out != NULL) {
        // We've to change the writer's file descriptor
        int fd_act = open(redir_out, O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
        SYS_ERROR(fd_act == -1, "OpenError");

        int syscall_result = dup2(fd_act, STDOUT_FILENO);
        SYS_ERROR(syscall_result == -1, "Dup2Error");

        syscall_result = close(fd_act);
        SYS_ERROR(syscall_result == -1, "CloseError");
    }
}

void change_file_descriptor_input_from_fd(int fd_act) {
    int syscall_result = dup2(fd_act, STDIN_FILENO);
    SYS_ERROR(syscall_result == -1, "Dup2Error");

    syscall_result = close(fd_act);
    SYS_ERROR(syscall_result == -1, "CloseError");
}

void change_file_descriptor_output_from_fd(int fd_act) {
    int syscall_result = dup2(fd_act, STDOUT_FILENO);
    SYS_ERROR(syscall_result == -1, "Dup2Error");

    syscall_result = close(fd_act);
    SYS_ERROR(syscall_result == -1, "CloseError");
}