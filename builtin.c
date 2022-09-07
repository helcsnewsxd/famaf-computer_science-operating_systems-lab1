#include "builtin.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

static bool are_str_equal(char *s1, char *s2) { return strcmp(s1, s2) == 0; }

bool builtin_is_internal(scommand cmd) {
    assert(cmd != NULL);
    char *command = scommand_front(cmd);

    bool is_cd = are_str_equal("cd", command);
    bool is_help = are_str_equal("help", command);
    bool is_exit = are_str_equal("exit", command);
    return is_cd || is_help || is_exit;
}

bool builtin_alone(pipeline p) {
    assert(p != NULL);
    bool is_alone = pipeline_length(p) == 1;
    bool is_internal = builtin_is_internal(pipeline_front(p));
    return is_alone && is_internal;
}
