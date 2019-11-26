#include<sys/stat.h>
#include<unistd.h>
#include<fcntl.h>
#include<stdio.h>

void run_pipe(char **args){
    int ps[2];
    pipe(ps);

    pid_t pid = fork();
    pid_t child_pid;
    int child_status;

    if(pid == 0){ // child process

        close(1);
        close(ps[0]);
        dup2(ps[1], 1);

        //e.g. cmd[0] = "ls", cmd[1] = "-l"
        char ** cmd = split(args[index], " \t");   

        //if fork here, program cannot continue with infinite loop somewhere
        if(fork()==0){
            if (execvp(cmd[0],cmd)==-1){
                printf("%s: Command not found.\n", args[0]);
            }
        }
        wait(0);
    }
    else{ // parent process

        close(0);
        close(ps[1]);
        dup2(ps[0],0);

        //e.g. cmd[0] = "wc", cmd[1] = "-l"
        char ** cmd = split(args[index+1], " \t");

        //if fork here, program cannot continue with infinite loop somewhere
        if(fork()==0){
            if (execvp(cmd[0],cmd)==-1){
                printf("%s: Command not found.\n", args[0]);
            }
        }
        wait(0);
        waitpid(pid, &child_status, 0);
    }    
}
