#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <stdio.h>

/***********************************************************************
 * This program is a simple Communicating Processes, it will communicate
 * between the parent and child regarding signals send, and quit
 * elegantly when ^C is send.
 *
 * @author Dewent, Jon
 * @author Li, Cheng
 * @version September 19, 2018
 **********************************************************************/

void sigHandler (int);
pid_t *childPid;

int main()
{
    childPid = malloc(sizeof(int*));
    int parentID = getpid();
    int pid;
    if((pid = fork()) < 0) {
        perror ("fork error");
        exit(1);
    }

    // Child process handling
    if (!pid){
        while(1){
            int randomTime = (rand() % 5)+1; /* Wait a time of 1-5 secs*/
            sleep(randomTime);
            int randomSignal = rand() % 2;  /* decide to send which signal */
            if(randomSignal)
                kill(parentID, SIGUSR2);
            else
                kill(parentID, SIGUSR1);
        }
    }

    // Parent process handling
    printf("Spawned child PID# %d\n",pid);
    *childPid = pid;
    while(1) {
        printf("waiting...\t");
        fflush(stdout);
        signal (SIGINT, sigHandler);        /* Signal handlers */
        signal (SIGUSR1, sigHandler);
        signal (SIGUSR2, sigHandler);
        pause();
    }

    return 0;
}

// This function handles signal SIGINT and terminates the processes
void sigHandler (int sigNum)
{
    if(sigNum == SIGUSR1){
        printf ("received a SIGUSR1 signal.\n");
    }
    else if(sigNum == SIGUSR2){
        printf ("received a SIGUSR2 signal.\n");
    }
    else{
        printf (" received. Thats it, I'm shutting you down...\n");
        // this is where shutdown code would be inserted
        sleep (1);
        kill(*childPid, SIGINT);
        free(childPid);
        exit(0);
    }
}
