#include "../../builtin.h"
#include "../../command.h"
#include <assert.h>
#include <glib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool test_internal(char *cmd) {
    scommand test = scommand_new();
    scommand_push_back(test, cmd);
    bool result = builtin_is_internal(test);
    scommand_destroy(test);
    return result;
}

bool test_alone_when_alone(char *cmd) {
    scommand test_sc = scommand_new();
    scommand_push_back(test_sc, cmd);
    pipeline test_p = pipeline_new();
    pipeline_push_back(test_p, test_sc);

    bool result = builtin_alone(test_p);
    pipeline_destroy(test_p);
    return result;
}

bool test_alone_when_not_alone(char *cmd) {
    scommand test_sc = scommand_new();
    scommand_push_back(test_sc, cmd);

    scommand test_sc2 = scommand_new();
    scommand_push_back(test_sc2, strdup(cmd));

    pipeline test_p = pipeline_new();
    pipeline_push_back(test_p, test_sc);
    pipeline_push_back(test_p, test_sc2);

    bool result = builtin_alone(test_p);
    pipeline_destroy(test_p);
    return result;
}

int main(void) {
    // Test internal
    assert(test_internal(strdup("cd")) == true);
    assert(test_internal(strdup("help")) == true);
    assert(test_internal(strdup("exit")) == true);
    assert(test_internal(strdup("NoSoyUnComandoInterno")) == false);

    // Test alone when pipeline has only one element
    assert(test_alone_when_alone(strdup("cd")) == true);
    assert(test_alone_when_alone(strdup("help")) == true);
    assert(test_alone_when_alone(strdup("exit")) == true);
    assert(test_alone_when_alone(strdup("NoSoyUnComandoInterno")) == false);

    // Test alone when pipeline has more than 1 element
    assert(test_alone_when_not_alone(strdup("cd")) == false);
    assert(test_alone_when_not_alone(strdup("help")) == false);
    assert(test_alone_when_not_alone(strdup("exit")) == false);
    assert(test_alone_when_not_alone(strdup("NoSoyUnComandoInterno")) == false);
    return 0;
}
