#include "../../execute.h"
#include "../../parser.h"
#include "../../parsing.h"
#include <assert.h>
#include <glib.h>
#include <stdio.h>
#include <string.h>

// helper function to create pipeline from string
static pipeline get_pipeline_from_string(char *input_str) {
    pipeline pipe = NULL;
    FILE *temp = tmpfile();
    if (temp) {
        fprintf(temp, "%s", input_str);
    }
    rewind(temp);

    Parser input = parser_new(temp);

    pipe = parse_pipeline(input);
    fclose(temp);
    return pipe;
}

static void test_simple_command() {
    char *input = "ls -l";
    pipeline pipe = get_pipeline_from_string(input);
    execute_pipeline(pipe);
}

static void test_helper(char *cmd) {
    pipeline pipe1 = get_pipeline_from_string(strdup(cmd));
    char *pipe_str = pipeline_to_string(pipe1);
    g_assert_cmpstr(pipe_str, ==, cmd);
}

int main(void) {
    // test if helper function works
    test_helper("ls -l");
    test_simple_command();

    printf("\nAll tests passed!\n");
    return 0;
}
