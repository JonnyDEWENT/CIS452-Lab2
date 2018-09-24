

#include <pthread.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h> 

//using namespace std;
void* fileGet(void* arg);

int main()
{

int filesAccessed = 0;
char * filename;
void* result1;

 pthread_t thread;

    while (fgets(filename, 100, stdin) != NULL) {

    int status;

    if ((status = pthread_create (&thread, NULL,  fileGet, filename) != 0)) {
        fprintf (stderr, "thread create error %d: %s\n", status, strerror(status));
        exit (1);
    }
    
    filesAccessed++;
    
    } 
}

void* fileGet(void* arg){
    int wait = rand() % 5;
    if(wait > 0){
        sleep(1);
    }
    else {
        int wait2 = rand() % 4 + 10;
        sleep(wait2);
    } 
    printf("File Accessed: %s \n", arg);
    //free(arg);
        return arg;
        
}