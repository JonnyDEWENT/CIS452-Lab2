#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <stdio.h>

void sigHandler (int sigNum, siginfo_t *siginfo, void *context);

int main()
{
    struct sigaction sigact;
    
    sigact.sa_flags = SA_SIGINFO;;
    sigact.sa_sigaction = sigHandler;
    
    int parentID = getpid();
    int child1, child2;
    
    if((child1 = fork()) < 0) {
        perror ("fork child1 error");
        exit(1);
    }
    
    if (!child1){
        while(1){
            int randomTime = (rand() % 5)+1;
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
        
        if (!child2){
            while(1){
                int randomTime = (rand() % 5)+1;
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
    if(sigNum == SIGUSR1){
        printf ("received a SIGUSR1 signal from process: %d.\n", siginfo->si_pid);
    }
    else if(sigNum == SIGUSR2){
        printf ("received a SIGUSR2 signal from process: %d.\n", siginfo->si_pid);
    }
    
    else{
        printf (" received. Thats it, I'm shutting you down...\n");
        // this is where shutdown code would be inserted
        sleep (1);
        exit(0);
    }
}

