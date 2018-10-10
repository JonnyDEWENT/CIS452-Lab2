#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/sem.h>
#include <semaphore.h>

#define SIZE 16

int main (int argc, char* argv[])
{
   int status;
   long int i, loop, temp, *shmPtr;
   int shmId;
   pid_t pid;
   int* semval = malloc(sizeof(int));
   semval[0] = 0;
   int semid = semget(IPC_PRIVATE,1,00600);
   //semctl(semid,0,SETVAL,1);
    struct sembuf waitsem[2],signal2[2];

    waitsem[0].sem_num = 0;
    waitsem[0].sem_op = 0;
    waitsem[0].sem_flg = 0;

    signal2[0].sem_num = 0;
    signal2[0].sem_op = -1;
    signal2[0].sem_flg = 0;

    waitsem[1].sem_num = 0;
    waitsem[1].sem_op = 1;
    waitsem[1].sem_flg = 0;

    signal2[1].sem_num = 0;
    signal2[1].sem_op = 1;
    signal2[1].sem_flg = 0;

      // get value of loop variable (from command-line argument)                

   if ((shmId = shmget (IPC_PRIVATE, SIZE, IPC_CREAT|S_IRUSR|S_IWUSR)) < 0) {
      perror ("i can't get no..\n");
      exit (1);
   }
   if ((shmPtr = shmat (shmId, 0, 0)) == (void*) -1) {
      perror ("can't attach\n");
      exit (1);
   }
    semctl(semid,0,SETVAL,semval);
   loop = atoi(argv[1]);

   shmPtr[0] = 0;
   shmPtr[1] = 1;

   if (!(pid = fork())) {
      for (i=0; i<loop; i++) {
          
        semop(semid,&waitsem[0],2);    
                   
        temp = shmPtr[0];
        shmPtr[0] = shmPtr[1];
        shmPtr[1] = temp;
        
        semop(semid,&signal2[0],1);  
      }
      if (shmdt (shmPtr) < 0) {
         perror ("just can't let go\n");
         exit (1);
      }
      exit(0);
   }
   else
      for (i=0; i<loop; i++) {
               // swap the contents of shmPtr[1] and shmPtr[0]
                
        semop(semid,&waitsem[0],2); 

        temp = shmPtr[1];
        shmPtr[1] = shmPtr[0];
        shmPtr[0] = temp;

        semop(semid,&signal2[0],1);  
      }

   wait (&status);
   printf ("values: %li\t%li\n", shmPtr[0], shmPtr[1]);

   if (shmdt (shmPtr) < 0) {
      perror ("just can't let go\n");
      exit (1);
   }
   if (shmctl (shmId, IPC_RMID, 0) < 0) {
      perror ("can't deallocate\n");
      exit(1);
   }

   semctl (semid, 0, IPC_RMID);
   printf("Disposed of semaphores!\n");

   return 0;
}

