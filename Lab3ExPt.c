#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <stdio.h>
/***********************************************************************
 * This program is a Communicating Processes that uses sigaction.
 * Childs will send signals to parents and parent will quit
 * elegantly when ^C is send.
 *
 * @author Dewent, Jon
 * @author Li, Cheng
 * @version September 19, 2018
 **********************************************************************/

/* Our signal handler */
void sigHandler (int sigNum, siginfo_t *siginfo, void *context);

/* Global child pid, so we can kill it */
pid_t child1, child2;

int main()
{
    // initialize sigaction
    struct sigaction sigact;
    sigact.sa_flags = SA_SIGINFO;;
    sigact.sa_sigaction = sigHandler;
    
    // get parent pid
    int parentID = getpid();
    
    // Spawning first child
    if((child1 = fork()) < 0) {
        perror ("fork child1 error");
        exit(1);
    }
    /* First child will wait for random time and sends random signals */
    if (!child1){
        while(1){
            int randomTime = (rand() % 5);
            sleep(randomTime);
            int randomSignal = rand() % 2;
            if(randomSignal)
                kill(parentID, SIGUSR2);
            else
                kill(parentID, SIGUSR1);
        }
    }

    else{
        printf("Spawned child 1 PID# %d\n",child1);
        
        /* Forking child 2 */
        if((child2 = fork()) < 0) {
            perror ("fork child2 error");
            exit(1);
        }
        
        /* Second child also wait for random time and sends random signals */
        if (!child2){
            while(1){
                int randomTime = (rand() % 5);
                sleep(randomTime);
                int randomSignal = rand() % 2;
                if(randomSignal)
                    kill(parentID, SIGUSR2);
                else
                    kill(parentID, SIGUSR1);
            }
        }
        
        // Parent process handling
        printf("Spawned child 2 PID# %d\n",child2);
        while(1) {
            printf("waiting...\t");
            fflush(stdout);
            sigaction(SIGUSR1, &sigact, NULL );
            sigaction(SIGUSR2, &sigact, NULL );
            sigaction(SIGINT, &sigact, NULL );
            pause();
        }
    }
    return 0;
}

// This function handles signal SIGINT and terminates the processes
void sigHandler (int sigNum, siginfo_t *siginfo, void *context)
{
    /* When signal is SIGUSR1, print out signal info and child pid */
    if(sigNum == SIGUSR1 && siginfo->si_pid != 0){
        printf ("received a SIGUSR1 signal from process: %d.\n", siginfo->si_pid);
    }
    /* When signal is SIGUSR2, print out signal info and child pid */
    else if(sigNum == SIGUSR2 && siginfo->si_pid != 0){
        printf ("received a SIGUSR2 signal from process: %d.\n", siginfo->si_pid);
    }
    /* When ^C is performed, quit*/
    else if (sigNum == SIGINT){
        printf (" received. Thats it, I'm shutting you down...\n");
        // this is where shutdown code would be inserted
        kill(child1, SIGINT);
        kill(child2, SIGINT);
        sleep (1);
        exit(0);
    }
}

