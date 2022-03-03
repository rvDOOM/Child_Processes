#include <stdio.h>
#include <unistd.h>
#include<sys/wait.h>

int main() {

    pid_t pid1, pid2;
    int fd[2];
    //fd[0] - read
    //fd[1] - write

    //Create pipe. Check if pipe has error upon creation.
    if (pipe(fd) == -1) {
        fprintf(stderr, "An Error occurred with pipe!");
        return 1;
    }

    pid1 = fork();                  //Create a child process

    if (pid1 == 0) {      //First Child Process "ls -F"
        dup2(fd[1], 1);                     //Write the output of the child process to the read end of pipe
        close(fd[0]);
        close(fd[1]);
        execlp("ls", "ls", "-F", NULL);             //Execute ls -F
    } else if (pid1 < 0)
        fprintf(stderr, "First child failed!");

    pid2 = fork();              //Create the second child process

    if (pid2 == 0) {              //Second child Process "nl"
        dup2(fd[0], 0);                 //Read the output of the first child process using pipe
        close(fd[0]);
        close(fd[1]);
        execlp("nl", "nl", NULL);       //Execute nl
    } else if (pid1 < 0)
        fprintf(stderr, "Second child failed!");

    wait(NULL);             //wait for a child to finish

    return 0;
}
