/**
 * @Filename: redirect_demo.c
 * @Description: 执行"ls -l | head -5"
 */
 
#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main(int argc, char *argv[])
{
    pid_t child;
    int fd[2];

    if ((pipe(fd) == -1) || ((child = fork()) == -1)) {
        perror("Failed to fork.");
        return 1;
    }

    if (child == 0) {     /* 子进程 */ 
        if (dup2(fd[1], STDOUT_FILENO) == -1)
            perror("Failed to redirect stdout of ls");
        else if ((close(fd[0]) == -1) || (close(fd[1]) == -1))
            perror("Failed to close extra pipe descriptors on ls");
        else {
            execl("/bin/ls", "ls", "-l", NULL);
            perror("Failed to execute ls");
        }
    }

    /* 父进程 */ 
    if (dup2(fd[0], STDIN_FILENO) == -1)
        perror("Failed to redirect stdin of head");
    else if ((close(fd[0]) == -1) || (close(fd[1]) == -1))
        perror("Failed to close extra pipe descriptors on head");
    else {
        execl("/usr/bin/head", "head", "-5", NULL);
        perror("Failed to execute head");
    }
    return 0;
}
