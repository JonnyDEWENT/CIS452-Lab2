#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <stdio.h>

void sigHandler (int);

int main()
{
    int parentID = getpid();
    int pid;
    if((pid = fork()) < 0) {
        perror ("fork error"); 
        exit(1); 
    }
    
    // Child process handling
    if (!pid){
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
    printf("Spawned child PID# %d\n",pid);
    while(1) {
        printf("waiting...\t");
        fflush(stdout);
        signal (SIGINT, sigHandler);
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
        exit(0);
    }
}
