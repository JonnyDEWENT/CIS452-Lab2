#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>

int main()
{
    DIR *dirPtr;
    struct dirent *entryPtr;
    
    dirPtr = opendir(".");
    
    while((entryPtr = readdir(dirPtr))){
        struct stat statBuf;
        stat(entryPtr->d_name, &statBuf);
        printf()
        printf("%-20s Size %lld bytes\n", entryPtr->d_name, statBuf.st_size);
    }
    
    closedir(dirPtr);
    return 0;
}
