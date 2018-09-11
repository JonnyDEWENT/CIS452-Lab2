#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int main(void) {
	char *argv[64];
	char *buffer = malloc(sizeof(char)*1000);
    pid_t  pid;
    int    status;
    
	while(1){
		printf("[Shell] -> ");
        
        if(fgets(buffer,sizeof(buffer),stdin) == NULL){
            perror("Failed to get user input");
            exit(1);
        }
        
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
            if (execvp(*argv, argv) < 0) {
                perror("exec failed");
                exit(1);
            }
        }
        else{                               /* The parent, wait for child to finish. */
            while(wait(&status) != pid)
                printf("Waiting for child to finish");
        }
	}
    return 1;
} 
