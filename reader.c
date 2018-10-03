#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define FOO 4096

struct memory{
    int flag;
    char message[FOO-4];
};

int main ()
{
    int shmId;
    char *shmPtr;
    
    struct memory* m;
    
    printf("THIS IS A READER PROGRAM\n\n");
    printf("Enter the shared memory ID: ");
    char idInput[100];
    if(fgets(idInput,sizeof(idInput),stdin) == NULL){
        perror("Failed to get user input");
        exit(1);
    }
    shmId = atoi(idInput);
    
    if ((shmPtr = shmat (shmId, 0, 0)) == (void*) -1) {
        perror ("can't attach\n");
        exit (1);
    }
    m = (struct memory*)shmPtr;
    
    while(1){
        if(m->flag){
            printf("Received %s", m->message);
            m->flag=0;
        }
        
    }
    
    return 0;
}
