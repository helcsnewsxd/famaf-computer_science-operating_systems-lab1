#include "builtin.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

bool builtin_is_internal(scommand cmd) {
    assert(cmd != NULL);
    char *command = scommand_front(cmd);

    bool cmd_is_internal = false;
    for(unsigned int i = 0; i < count_internal_cmd; i++){
        cmd_is_internal = cmd_is_internal || are_str_equal(command,internal_cmd_list[i].name);
    }

    return cmd_is_internal;
}

bool builtin_alone(pipeline p) {
    assert(p != NULL);

    bool is_alone = pipeline_length(p) == 1;
    bool is_internal = builtin_is_internal(pipeline_front(p));

    return is_alone && is_internal;
}

void builtin_run(scommand cmd) {
    assert(builtin_is_internal(cmd));

    char *command = scommand_front(cmd);

    unsigned int i = 0u;
    while(! are_str_equal(internal_cmd_list[i].name,command)){
        ++i;
    }
    
    (*internal_cmd_list[i].function)(cmd);
    
}