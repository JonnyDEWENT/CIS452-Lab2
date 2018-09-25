#include <pthread.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

//using namespace std;
void* fileGet(void* arg);
void sigHandler (int sigNum);

int filesAccessed = 0;
char *filename;

int main()
{
    filename = malloc(sizeof(char)*100);
    pthread_t thread;
    signal (SIGINT, sigHandler);
    while (fgets(filename, 100, stdin) != NULL) {
        int status;
        
        if ((status = pthread_create (&thread, NULL,  fileGet, filename) != 0)) {
            fprintf (stderr, "thread create error %d: %s\n", status, strerror(status));
            exit (1);
        }

        filesAccessed++;
    }
}

// This function handles signal SIGINT and terminates the processes
void sigHandler (int sigNum)
{
    if(sigNum == SIGINT){
        printf (" received.\nNumber Files Accessed: %d\n", filesAccessed);
        printf("Shutting down...\n");
        free(filename);
        // Prepare to shutdown.
        sleep (1);
        exit(0);
    }
}

void* fileGet(void* arg){
    int wait = rand() % 5;
    if(wait > 0){
        sleep(1);
    }
    else {
        int waitTime = rand() % 4 + 7;
        sleep(waitTime);
    }
    printf("File Accessed: %s \n", arg);
    return arg;
    
}
