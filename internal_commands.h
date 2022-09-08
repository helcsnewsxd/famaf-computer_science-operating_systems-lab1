#ifndef INTERNAL_COMMANDS_H
#define INTERNAL_COMMANDS_H

#include "command.h"

struct cmd_type{
    char * name;
    void (*function)(scommand);
};
/**
 * @brief CMD_Type
 * There is a type of internal cmd's to approach the modularization
 */

bool are_str_equal(char *s1, char *s2);
// Function to compare strings of cmds

void cd_run(scommand cmd);

void help_run(scommand cmd);

void exit_run(scommand cmd);

bool quit; // global variable to determine if the shell should close

#define count_internal_cmd 3
extern struct cmd_type internal_cmd_list[count_internal_cmd];

// A list of internal commands and pointers to corresponding functions

#endif