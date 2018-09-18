#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <stdio.h>

void sigHandler (int);
void sigHandler2 (int);

int main()
{

  int parentID = getpid();
  signal (SIGINT, sigHandler);
    int pid;
    if((pid = fork()) < 0) {

    }

// Child process handling
    if (!pid){
      while(1){
        int randomTime = rand() % 6;

        sleep(randomTime);

        int randomSignal = rand() % 2;

        if(randomSignal)
          kill(parentID, SIGUSR2);
        else
          kill(parentID, SIGUSR1);
      }

    }

// Parent process handling
    else{
      printf("Spawned child PID# %d\n",pid);
      while(1) {
        printf("waiting...\t");
        signal (SIGUSR1, sigHandler2);
        signal (SIGUSR2, sigHandler2);
        pause();
    }
  }


    return 0;
}

// This function handles user signals SIGUSR1 and SIGUSR2
void sigHandler2 (int sigNum)
{
    if (sigNum == SIGUSR1)
    printf ("received a SIGUSR1 signal.\n");

    if (sigNum == SIGUSR2)
    printf ("received a SIGUSR2 signal.\n");
    // this is where shutdown code would be inserted


}

// This function handles signal SIGINT and terminates the processes
void sigHandler (int sigNum)
{
    printf ("^C received. Thats it, I'm shutting you down...\n");
    // this is where shutdown code would be inserted
    sleep (1);
    printf ("outta here.\n");
    exit(0);
}
