#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h>

int main(int argc, char *argv[])
{
    if(argc < 2){
        printf("Please provide a filename\n");
        exit(1);
    }
    // Symbolic link.
    if(!strcmp(argv[1], "-s")) {
        // Case a filename is not provided
        if(argc < 4){
            char *filename = basename(argv[2]);
            if(symlink(argv[2], filename)){
                perror("Soft Link error");
                exit(1);
            }
            return 1;
        }else{ // Case there is a filename or path provided
            if(symlink(argv[2], argv[3])){
                perror("Soft Link error");
                exit(1);
            }
            return 1;
        }
    }
    // Hard Link
    else{
        // Case a new filename is not provided
        if(argc < 3){
            char *filename = basename(argv[1]);
            if(link(argv[1], filename)){
                perror("Soft Link error");
                exit(1);
            }
            return 1;
            return 1;
        }else{ // Case there is a filename or path provided
            if(link(argv[1], argv[2])){
                perror("Hard Link error");
                exit(1);
            }
            return 1;
        }
    }
}
