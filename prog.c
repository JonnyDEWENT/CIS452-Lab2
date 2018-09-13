#include <sys/wait.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <sys/resource.h>
/***********************************************************************
 * This program simulate a simple shell, it will take in shell command
 * and execute the command, also the program will print each execution
 * CPU time and any involuntary context switches.
 *
 * @author Dewent, Jon
 * @author Li, Cheng
 * @version September 12, 2018
 **********************************************************************/
int main(void) {
    /* An array of pointers pointing to user enter arguments. */
    char *argv[64];
    
    /* Buffer reads user input line. */
    char buffer[1000];
    
    /* Parent pid and child status. */
    pid_t  pid;
    int status;
    
    /* Variable declared to get resource usage. */
    struct rusage* pusage = malloc(sizeof(struct rusage*));
    int* parentpid = malloc(sizeof(int*));
    printf("\nParent PID: %ld\n", (long)getpid());
    
    while(1){
        printf("[Shell] -> ");
        
        /* Read user input */
        if(fgets(buffer,sizeof(buffer),stdin) == NULL){
            perror("Failed to get user input");
            exit(1);
        }
        
        /* Exiting program when user types quit */
        if(!strncmp("quit",buffer,4)){
            exit(1);
        }
        /* Tokenizing the user input */
        argv[0]=strtok (buffer, " \t\r\n\f\v\0");
        argv[1]=strtok (NULL, " \t\r\n\f\v\0");
        
        pid = fork();                       /* Fork a child. */
        if(pid < 0){                        /* Chek check if the fork fail or not. */
            perror("Failed to fork a child");
            exit(1);
        }
        
        else if(pid == 0){                  /* The child, execute. */
            
            /* getting process resource statistics and printing */
            getrusage(RUSAGE_SELF, pusage);
            *parentpid = getpid();
            printf("\n\nPROCESS PID: %i\nUSER CPU TIME: %ld.%ld\nINVOLUNTARY CONTEXT SWITCHES: %ld\n\n\n", *parentpid, pusage->ru_utime.tv_sec,pusage->ru_utime.tv_usec, pusage->ru_nivcsw);
            
            /* Execute command line input */
            if (execvp(*argv, argv) < 0) {
                perror("exec failed");
                exit(1);
            } 
        }
        
        else{                               /* The parent waits for child to finish. */
            wait(&status);
        }
    }
    return 1;
}
