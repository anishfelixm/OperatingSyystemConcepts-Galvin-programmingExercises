/*
 * ProcessCreationAndTermination.c
 * 
 * A simple c program to create a child process
 * 
 * Fork() to create a child process -> execlp to run a new prog (here ls command)
 */
#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char * argv[]) {
    pid_t pid;
    pid = fork();
   
    if  (pid < 0) {
        printf("Error creating process");
        return 1;
    }
    else if (pid == 0) {
        printf("Inside child process\n");
        execlp("/bin/ls", "ls", NULL);
    }
    else {
        printf("Inside parent process\n");
        wait(NULL);
        printf("Child process execution completed\n");
    }
    return 0;
}