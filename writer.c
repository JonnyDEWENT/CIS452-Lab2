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
    
    if ((shmId = shmget (IPC_PRIVATE, FOO, IPC_CREAT|S_IRUSR|S_IWUSR)) < 0) {
        perror ("i can't get no..\n");
        exit (1);
    }
    
    printf("WRITE SHARED MEMORY ID: %d\n", shmId);
    if ((shmPtr = shmat (shmId, 0, 0)) == (void*) -1) {
        perror ("can't attach\n");
        exit (1);
    }

//    if (shmdt (shmPtr) < 0) {
//        perror ("just can't let go\n");
//        exit (1);
//    }
//    if (shmctl (shmId, IPC_RMID, 0) < 0) {
//        perror ("can't deallocate\n");
//        exit(1);
//    }
//    char buffer[4096];
//    if(fgets(shmPtr,sizeof(shmPtr),stdin) == NULL){
//        perror("Failed to get user input");
//        exit(1);
//    }
    m = (struct memory*)shmPtr;
    m->flag = 0;
    while(1){
        if(!m->flag){
            printf("Enter your message: ");
            if(fgets(m->message,sizeof(m->message),stdin) == NULL){
                perror("Failed to get user input");
                exit(1);
            }
            m->flag = 1;
            printf("Writing to shared memory....\n");
        }
    }
    
    return 0;
}
