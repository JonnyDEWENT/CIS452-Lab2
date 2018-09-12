#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <sys/resource.h>



int main(void) {
    char *argv[64];
    char *buffer = malloc(sizeof(char)*1000);
    pid_t  pid, child;
    int    status;
    struct rusage* pusage = malloc(sizeof(struct rusage*));
    int* parentpid = malloc(sizeof(int*));
    printf("\nParent PID: %ld\n", (long)getpid());
    while(1){
        printf("[Shell] -> ");
        if(fgets(buffer,sizeof(buffer),stdin) == NULL){
            perror("Failed to get user input");
            exit(1);
        }
        
        // Exiting program when user types quit
        if(!strncmp("quit",buffer,4)){
            exit(1);
        }
        // Tokenizing the user input
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
            printf("\n\nPROCESS PID: %i\nUSER CPU TIME: %ld.%d\nINVOLUNTARY CONTEXT SWITCHES: %ld\n\n\n", *parentpid, pusage->ru_utime.tv_sec,pusage->ru_utime.tv_usec, pusage->ru_nivcsw);
           
            if (execvp(*argv, argv) < 0) {
                perror("exec failed");
                exit(1);
            }
            
        }
        
        else{                               /* The parent waits for child to finish. */
            child = wait(&status);

        }
    }
    return 1;
}
