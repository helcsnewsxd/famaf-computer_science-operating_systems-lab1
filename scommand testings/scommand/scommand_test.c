#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <glib.h>
#include "../../command.h"

// Compilation and execution options:
// - To view program output and fails
//      gcc scommand_test.c ../../command.c ../../strextra.c -o ejecutable_scommand_test `pkg-config --cflags --libs glib-2.0` && ./ejecutable_scommand_test < scommand_test_input.in
// - To view memory leaks and heaps (only for us)
//      gcc scommand_test.c ../../command.c ../../strextra.c -o ejecutable_scommand_test `pkg-config --cflags --libs glib-2.0` && valgrind --leak-check=full ./ejecutable_scommand_test < scommand_test_input.in
// - To view memory leaks and all heaps information
//      gcc scommand_test.c ../../command.c ../../strextra.c -o ejecutable_scommand_test `pkg-config --cflags --libs glib-2.0` && valgrind --leak-check=full -v --track-origins=yes ./ejecutable_scommand_test < scommand_test_input.in

int main(void){
    char * a = malloc(sizeof(char*));
    char * b = malloc(sizeof(char*));
    char * c = malloc(sizeof(char*));
    char * d = malloc(sizeof(char*));

    scommand test = scommand_new();
    scanf("%s %s",&(*a),&(*b));
    scommand_push_back(test,a);
    scommand_push_back(test,b);
    scanf("%s %s",&(*c),&(*d));
    scommand_set_redir_in(test,c);
    scommand_set_redir_out(test,d);

    // char * representation = scommand_to_string(test);
    // printf("%s\n",representation);
    // free(representation);
    // representation = NULL;

    printf("%s\n",scommand_front(test));
    scommand_pop_front(test);
    printf("%s\n",scommand_front(test));
    scommand_pop_front(test);
    printf("%u\n",scommand_length(test));
    printf("%u\n",(unsigned int)scommand_is_empty(test));
    printf("%s\n",scommand_get_redir_in(test));
    printf("%s\n",scommand_get_redir_out(test));

    test = scommand_destroy(test);
    printf("%u\n",test == NULL);
    return 0;
}