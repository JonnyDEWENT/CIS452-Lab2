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
 * CHAT SERVER program using shared memory.
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

/* Memory struct */
struct memory* m;

int sender;

int main ()
{
    pid_t pid;
    
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
    
    m->flag = 0;
    
    sender = 0;
    
    /* Run into an error when forking */
    if ((pid = fork()) < 0) {
        perror ("fork failed");
        exit(1);
    }
    
    // Parent, writer
    if(pid){
        printf("Enter a message anytime\n");
        while(1){
            if(m->flag==0){
                
                if(fgets(m->message,sizeof(m->message),stdin) == NULL){
                    perror("Failed to get user input");
                    exit(1);
                }
                m->flag = 1;
            }
        }
        
    }
    // Child, reader
    if(!pid){
        /* Loop to read shared memory. */
        while(1){
            pid_t ppid, cpid;
            ppid = getpid();
            
            /* Run into an error when forking */
            if ((cpid = fork()) < 0) {
                perror ("fork failed");
                exit(1);
            }
            /* This child checks for a flag, if a new message is in. */
            if(!cpid){
                while(!m->flag){
                    ;
                }
                kill(ppid, SIGUSR1);
                exit(0);
            }
            /* this process sends signal and wait for message. */
            if(cpid){
                signal(SIGUSR1, sigHandler);
                pause();
            }
            
        }
    }
    return 0;
}

// This function handles signal SIGINT and terminates the processes
void sigHandler (int sigNum)
{
    if(sigNum == SIGUSR1){  /* Read */
        printf("\nReceived: %s\n", m->message);
        m->flag = 0;
    }
    if(sigNum == SIGINT){  /* INTERRUPT */
        if (shmdt (shmPtr) < 0) {
            perror ("just can't let go\n");
            exit (1);
        }
        
        // this is where shutdown code would be inserted
        sleep (2);
        exit(0);
    }
}

