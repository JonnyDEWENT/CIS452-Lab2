#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <signal.h>

#define FOO 4096

/****************************************************
 * A writer program that writes to shared memory.
 ***************************************************/

/* Signal handler for interrupt. */
void sigHandler (int sigNum);

/* Memory struct, this will be the shared memory */
struct memory{
    int flag;   /* Indicate condition. */
    char message[FOO-4];    /* Message */
};

/* Global shared memory id and pointer, for detach on exit. */
int shmId;
char *shmPtr;

int main ()
{
    /* Memory struct */
    struct memory* m;
    
    /* Agree on a pass key for reader and writer */
    key_t key = ftok("shmfile",65);
    
    /* Get shared memory id. */
    if ((shmId = shmget (key, FOO, IPC_CREAT|S_IRUSR|S_IWUSR)) < 0) {
        perror ("i can't get no..\n");
        exit (1);
    }
    printf("Writer shared Memory ID = %d\n",shmId);
    
    /* Make a shared memory pointer. */
    if ((shmPtr = shmat (shmId, 0, 0)) == (void*) -1) {
        perror ("can't attach\n");
        exit (1);
    }
    /* Casting the shared memory pointer to memory struct. */
    m = (struct memory*)shmPtr;
    m->flag = 0;
    
    /* Signal for interrupt */
    signal(SIGINT, sigHandler);
    
    /* Loop to take user input and write to shared memory. */
    while(1){
        if(m->flag<=0){
            printf("Enter your message: \n");
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

// This function handles signal SIGINT and terminates the processes
void sigHandler (int sigNum)
{
    if(sigNum == SIGINT){  /* INTERRUPT */
        /* Remove shared memory pointer. */
        if (shmdt (shmPtr) < 0) {
            perror ("just can't let go\n");
            exit (1);
        }
        /* Deallocate shared memory. */
        if (shmctl (shmId, IPC_RMID, 0) < 0) {
            perror ("can't deallocate\n");
            exit(1);
        }
        printf (" received. That’s it, I'm shutting you down...\n");
        // this is where shutdown code would be inserted
        sleep (2);
        exit(0);
    }
}

