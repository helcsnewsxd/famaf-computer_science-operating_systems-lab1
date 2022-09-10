#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include <unistd.h>
#include <sys/types.h> 
#include <time.h>
#include <sys/wait.h>
#include <fcntl.h>

#include "execute.h"
#include "builtin.h"
#include "command.h"



void execute_pipeline(pipeline apipe){
    // REQUIRES
    assert(apipe != NULL);

    scommand fst_command = pipeline_front(apipe);
    
    char *redir_in = scommand_get_redir_in(fst_command);
    char *redir_out = scommand_get_redir_out(fst_command);

    if(redir_in != NULL){
        close(STDIN_FILENO);
    }

    if(redir_out != NULL){
        close(STDOUT_FILENO);
        
    }
    

    if(builtin_alone(apipe)){
        // run internal command
        builtin_run(fst_command); 
    } else {
        // one external command
        int pid_fork = fork();
        if(pid_fork < 0){
            perror("System error with fork: ");
            exit(EXIT_FAILURE);
        } else if (pid_fork == 0) {
            char **argv = scommand_to_char_list(fst_command); // auxiliar function that saves args in argv (list of pointers)
            execvp(argv[0], argv);
            printf("Esto no tendria que aparecer\n");
        } else {
            wait(NULL);
        }
    }
}

// "ls"  					execvp() fork() wait()
// 
// "ls &"  				    execvp() fork()
// 
// "ls -l"  				execvp() fork() wait()
// 
// "ls -l > salida.txt" 	execvp() fork() wait() open() dup2()
// 
// "sort -l < entrada.txt"  execvp() fork() wait() open() dup2()

// char *redir_out = scommand_get_redir_out(fst_command);

// if (redir_out != NULL) {
//     int fd = open(redir_out, O_WRONLY|O_CREAT); // opens file in write mode, if the file doesn't exist, it's created.
//     dup2(fd,1); // change where the output will be shown. In this case, it'll be shown in the file fd is storing. (i.e. stdout now points to redir_out (1 == stdout))
//     builtin_run(fst_command);
//     close(fd); // closes stdout.
// }