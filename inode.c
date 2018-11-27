#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>

int main(int argc, char** argv)
{
   DIR *dirPtr;
   struct dirent *entryPtr;


   dirPtr = opendir (".");

   if (argc > 1){
   char* userInput = argv[1];


   char* result;
   if(!strcmp(userInput,"-n")){
     while((entryPtr = readdir(dirPtr))){
       struct stat statBuf;
       stat(entryPtr->d_name, &statBuf);
       if(entryPtr->d_name[0] != '.'){
         printf("%u %u\t", statBuf.st_uid, statBuf.st_gid);
         printf("%-20s\n", entryPtr->d_name);
 }
}

   }

   else if(!strcmp(userInput,"-i")){
     while((entryPtr = readdir(dirPtr))){
       struct stat statBuf;
       stat(entryPtr->d_name, &statBuf);
       if(entryPtr->d_name[0] != '.'){
         printf("inode %llu ", statBuf.st_ino);
         printf("%-20s\n", entryPtr->d_name);
}
}
   }
 }

   else {
     while((entryPtr = readdir(dirPtr))){
       struct stat statBuf;
       stat(entryPtr->d_name, &statBuf);
       if(entryPtr->d_name[0] != '.'){
         printf("%-20s\n", entryPtr->d_name);
}
}
   }




   closedir (dirPtr);
   return 0;
}
