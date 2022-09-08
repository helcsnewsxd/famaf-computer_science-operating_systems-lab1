#ifndef INTERNAL_COMMANDS
#define INTERNAL_COMMANDS

#include "command.h"

typedef struct t_cmd_type{
    char * name;
    void (*function)(scommand);
} cmd_type;
/**
 * @brief CMD_Type
 * There is a type of internal cmd's to approach the modularization
 */

bool are_str_equal(char *s1, char *s2);
// Function to compare strings of cmds

cmd_type internal_cmd_list[];
unsigned int count_internal_cmd;
// A list of internal commands and pointers to corresponding functions

#endif