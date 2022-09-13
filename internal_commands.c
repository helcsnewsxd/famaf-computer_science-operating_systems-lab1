#include "internal_commands.h"
#include "files_descriptors.h"
#include "prompt.h"
#include "strextra.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "tests/syscall_mock.h"

struct cmd_type internal_cmd_list[count_internal_cmd] = {
    {"help", help_run}, {"cd", cd_run}, {"exit", exit_run}};

bool are_str_equal(char *s1, char *s2) { return strcmp(s1, s2) == 0; }

// FUNCTIONS TO RUN COMMANDS

static int do_cd_syscall(char *input_path) { return chdir(input_path); }

void cd_run(scommand cmd) {
    assert(are_str_equal(scommand_front(cmd), "cd"));

    char *input_path = NULL;
    int cmd_length = scommand_length(cmd);

    if (cmd_length > 2) {
        printf("mybash: cd: demasiados argumentos\n"); // The cd command only has 1 or 2 args
    } else if (cmd_length == 2) {
        scommand_pop_front(cmd); // Remove the "cmd" part of the cmd

        input_path = strmerge("", scommand_front(cmd));

        if (input_path[0] == '~') {
            memmove(input_path, input_path + 1, strlen(input_path));

            char *auxiliar_to_remove = input_path;
            input_path = strmerge(getenv("HOME"), input_path);
            free(auxiliar_to_remove);
        }

        if (do_cd_syscall(input_path) < 0) {
            perror("mybash: cd: "); // Print the error message of chdir
        }
    } else {
        input_path = strmerge("", getenv("HOME"));
        if (do_cd_syscall(input_path) < 0) {
            perror("mybash: cd: "); // Print the error message of chdir
        }
    }
    if (input_path != NULL) {
        free(input_path);
        input_path = NULL;
    }
}

void help_run(scommand cmd) {
    assert(are_str_equal(scommand_front(cmd), "help"));

    int pid = fork();
    SYS_ERROR(pid == -1, "ForkError");

    if (pid == 0) {
        change_file_descriptor_output(scommand_get_redir_out(cmd));
        prompt_help(); // Function in prompt.h
        exit(EXIT_SUCCESS);
    } else {
        waitpid(pid, NULL, 0u);
    }
}

void exit_run(scommand cmd) {
    assert(are_str_equal(scommand_front(cmd), "exit"));
    quit = true; // Change the value of the global variable in built.h
}