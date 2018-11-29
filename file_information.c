#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

int compare (const void * a, const void * b)
{
    return strcmp (*(const char **) a, *(const char **) b);
}

void sort(const char *arr[], int n)
{
    qsort (arr, n, sizeof (const char *), compare);
}

int main(int argc, char** argv)
{
    
    struct stat status;
    DIR *dirPtr;
    struct dirent *entryPtr;
    /* Count the number of files in the current directory. */
    int file_count;
    
    char argumentStr[100];
    strcpy(argumentStr, " ");
    for(int i = 1; i < argc; i++){
        strcat(argumentStr, argv[i]);
        strcat(argumentStr, " ");
    }
    
    if(argc > 1 && argv[argc-1][0] != '-'){
        if(stat(argv[argc-1], &status)){
            perror("Issue getting status!\n");
            exit(1);
        }
        if(S_ISDIR(status.st_mode)){
            dirPtr = opendir (argv[argc-1]);
            while ((entryPtr = readdir(dirPtr))) {
                file_count++;
            }
            closedir (dirPtr);
            dirPtr = opendir (argv[argc-1]);
        }
    }else{
        dirPtr = opendir (".");
        while ((entryPtr = readdir(dirPtr))) {
            file_count++;
        }
        closedir (dirPtr);
        dirPtr = opendir (".");
    }
    
    const char* fileArr[file_count];
    int currentFile = 0;
    
    /* Store all files in the current directory for sorting. */
    while ((entryPtr = readdir(dirPtr))) {
        fileArr[currentFile++] = entryPtr->d_name;
    }
    
    sort(fileArr, file_count);
    
    for (int i = 0; i < file_count; i ++){
        
        struct stat statBuf;
        if(argv[argc-1][0] != '-' && argc > 1){
            char path[50];
            strcpy(path, argv[argc-1]);
            strcat(path, "/");
            strcat(path, fileArr[i]);
            stat(path, &statBuf);
        }else{
            stat(fileArr[i], &statBuf);
        }
        
        if(strstr(argumentStr, " -a") == NULL) {
            if(fileArr[i][0] == '.'){
                continue;
            }
        }
        
        if(strstr(argumentStr, " -i") != NULL) {
            printf("%llu ", statBuf.st_ino);
        }
        
        if(strstr(argumentStr, " -n") != NULL) {
            printf("%u %u\t", statBuf.st_uid, statBuf.st_gid);
        }
        
        printf("%-20s\n", fileArr[i]);
    }
    closedir (dirPtr);
    return 0;
}


