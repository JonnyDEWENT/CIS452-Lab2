#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <signal.h>
#include <unistd.h>

#define FOO 4096

/****************************************************
 * This program is a reader program that reads message
 * from the shared memory.
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
    shmId = shmget (key, FOO, IPC_CREAT|S_IRUSR|S_IWUSR);
    printf("Reader shared Memory ID = %d\n",shmId);
    
    /* Make a shared memory pointer. */
    if ((shmPtr = shmat (shmId, 0, 0)) == (void*) -1) {
        perror ("can't attach\n");
        exit (1);
    }
    /* Casting the shared memory pointer to memory struct. */
    m = (struct memory*)shmPtr;
    
    /* Signal for interrupt */
    signal(SIGINT, sigHandler);
    
    /* Loop to read shared memory. */
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

// This function handles signal SIGINT and terminates the processes
void sigHandler (int sigNum)
{
    if(sigNum == SIGINT){  /* INTERRUPT */
        if (shmdt (shmPtr) < 0) {
            perror ("just can't let go\n");
            exit (1);
        }
        printf (" received. That’s it, I'm shutting you down...\n");
        // this is where shutdown code would be inserted
        sleep (2);
        exit(0);
    }
}
