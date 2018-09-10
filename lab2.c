#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[])
{
	while(1){
		char buffer[20];
		fgets(buffer,20,stdin);
		if(!strncmp("quit",buffer,4))
	  		exit(1);
    if (execvp(argv[1], &argv[1]) < 0) {
        perror("exec failed");
        exit(1);
    }
    puts("After the exec");

    return 0;
}
} 
