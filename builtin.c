#include "builtin.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

bool builtin_is_internal(scommand cmd) {
    assert(cmd != NULL);
    char *command = scommand_front(cmd);
    bool result = false;

    bool is_cd = strcmp("cd", command);
    bool is_help = strcmp("help", command);
    bool is_exit = strcmp("exit", command);
    return is_cd || is_help || is_exit;
}

bool builtin_alone(pipeline p) {
    assert(p != NULL);
    bool is_alone = pipeline_length(p) == 1;
    bool is_internal = builtin_is_internal(pipeline_front(p));
    return is_alone && is_internal;
}
