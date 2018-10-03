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
    
    printf("THIS IS A READER PROGRAM\n\n");

    shmId = shmget (key, FOO, IPC_CREAT|S_IRUSR|S_IWUSR);
    printf("Shared Memory ID = %d\n",shmId);
    
    if ((shmPtr = shmat (shmId, 0, 0)) == (void*) -1) {
        perror ("can't attach\n");
        exit (1);
    }
    m = (struct memory*)shmPtr;
    
    while(1){
        if(m->flag > 0){
            int flag = m->flag;
            printf("Received %s", m->message);
            m->flag= m->flag-1;
            while(m->flag == flag-1);
        }
        
    }
    
    return 0;
}

