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
        printf("inode %llu", statBuf.st_ino);
        printf("%u %u %lld\t", statBuf.st_uid, statBuf.st_gid, statBuf.st_size);
        printf("%-20s\n", entryPtr->d_name);
    }
    
    closedir(dirPtr);
    return 0;
}
