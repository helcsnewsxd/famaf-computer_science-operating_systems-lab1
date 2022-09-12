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
    parser_destroy(input);
    fclose(temp);
    return pipe;
}

static void test_simple_command() {
    char *input = "ip link";
    pipeline pipe = get_pipeline_from_string(input);
    execute_pipeline(pipe);
}

static void test_2_commands() {
    char *input = "ip link | sort ";
    pipeline pipe = get_pipeline_from_string(input);
    execute_pipeline(pipe);
}

static void test_4_commands() {
    char *input = "ip link | sort | head -n 1 | wc -l";
    pipeline pipe = get_pipeline_from_string(input);
    execute_pipeline(pipe);
}

static void test_3_commands() {
    char *input = "ip link | sort | head -n 1";
    pipeline pipe = get_pipeline_from_string(input);
    execute_pipeline(pipe);
}

static void test_helper(char *cmd) {
    pipeline pipe1 = get_pipeline_from_string(strdup(cmd));
    char *pipe_str = pipeline_to_string(pipe1);
    g_assert_cmpstr(pipe_str, ==, cmd);
    pipeline_destroy(pipe1);
    free(pipe_str);
}

int main(void) {
    // test if helper function works
    //test_helper("ls -l");
    //test_simple_command();
    printf("\n");
    //test_2_commands();
    printf("\n");
    //test_3_commands();
    printf("\n");
    test_4_commands();

    printf("\nAll tests passed!\n");
    return 0;
}
