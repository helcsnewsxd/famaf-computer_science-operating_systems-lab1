#include <stdlib.h>
#include <stdbool.h>
#include <check.h>

#include "parsing.h"
#include "parser.h"
#include "command.h"

static scommand parse_scommand(Parser p) {
    scommand new_scmd = scommand_new();

    arg_kind_t arg_type = ARG_INPUT;
    char * arg_cmd = parser_next_argument(p,&arg_type);
    
    while(arg_cmd != NULL){
        // We've the cmd argument in *arg_cmd

        if(arg_type == ARG_NORMAL){
            scommand_push_back(new_scmd,arg_cmd);
        }else if(arg_type == ARG_INPUT){
            scommand_set_redir_in(new_scmd,arg_cmd);
        }else if(arg_type == ARG_OUTPUT){
            scommand_set_redir_out(new_scmd,arg_cmd);
        }

        // New argument
        arg_cmd = NULL;
        arg_cmd = parser_next_argument(p,&arg_type);
    }

    // See if there are some errors
    if(scommand_is_empty(new_scmd)){
        new_scmd = scommand_destroy(new_scmd);
        new_scmd = NULL;
    }

    return new_scmd;
}

pipeline parse_pipeline(Parser p) {
    pipeline result = pipeline_new();
    scommand act_scmd = NULL;
    bool error = false, another_pipe = true;

    act_scmd = parse_scommand(p);
    error = (act_scmd == NULL); // Invalid command to begin

    while (another_pipe && !error) {
        // Push into a pipeline
        pipeline_push_back(result,act_scmd);

        // See final options (end line, pipe, background or EOF)
        // Is there a pipe?
        parser_op_pipe(p,&another_pipe);

        if(another_pipe){
            // Parsing the next scommand
            act_scmd = parse_scommand(p);

            error = (act_scmd == NULL); // Invalid command
        }
    }

    if(!error){
        // Is there a background?
        bool there_is_background = false;
        parser_op_background(p,&there_is_background);
        pipeline_set_wait(result,!there_is_background);
        
        // Erase the rest of the parser
        bool there_are_other_symbols = false;
        parser_garbage(p,&there_are_other_symbols);
        error = there_are_other_symbols;
    }else{
        // Erase the rest of the parser
        bool there_are_other_symbols = false;
        parser_garbage(p,&there_are_other_symbols);
    }

    // Erase the pipeline in case of an error
    if(error){
        result = pipeline_destroy(result);
        result = NULL;
    }

    // Return the pipeline or NULL (in case of a syntax error)
    return result;
}
