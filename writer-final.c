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

    key_t key = ftok("shmfile",65); 
    
    if ((shmId = shmget (key, FOO, IPC_CREAT|S_IRUSR|S_IWUSR)) < 0) {
        perror ("i can't get no..\n");
        exit (1);
    }
    printf("Shared Memory ID = %d\n",shmId);

    printf("WRITE SHARED MEMORY ID: %d\n", shmId);
    if ((shmPtr = shmat (shmId, 0, 0)) == (void*) -1) {
        perror ("can't attach\n");
        exit (1);
    }

    m = (struct memory*)shmPtr;
    m->flag = 0;
    while(1){
        if(m->flag<=0){
            printf("Enter your message: ");
            if(fgets(m->message,sizeof(m->message),stdin) == NULL){
                perror("Failed to get user input");
                exit(1);
            }
            m->flag = 2;
            printf("Writing to shared memory....\n");
        }
    }
    
    return 0;
}

