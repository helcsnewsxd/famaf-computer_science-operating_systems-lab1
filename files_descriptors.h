#ifndef FILE_DESCRIPTORS
#define FILE_DESCRIPTORS

#include <stdio.h>

#define SYS_ERROR(condition, message)                                                              \
    if (condition) {                                                                               \
        perror(message);                                                                           \
        exit(EXIT_FAILURE);                                                                        \
    }

/**
 * @brief Define used to report syscall and exit errors if needed
 *
 */

void change_file_descriptor_input(char *redir_in);
/**
 * @brief Change the actual INPUT of the process to the file's file descriptor
 *
 */

void change_file_descriptor_output(char *redir_out);
/**
 * @brief Change the actual OUTPUT of the process to the file's file descriptor
 *
 */

void change_file_descriptor_input_from_fd(int fd_act);
/**
 * @brief Change the actual INPUT of the process to the file descriptor
 *
 */

void change_file_descriptor_output_from_fd(int fd_act);
/**
 * @brief Change the actual OUTPUT of the process to the file descriptor
 *
 */

#endif