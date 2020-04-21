/**
 * @Filename: tsh2.c
 * @Description: 在遇到SIGINT和SIGQUIT信号时不会退出的shell
 */
 
#include <limits.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

#define PROMPT_STRING "tsh2>> "
#define QUIT_STRING "q"  /* 按q退出 */ 


void execute_cmd(char *incmd);
int signal_setup(struct sigaction *def, sigset_t *mask, void (*handler)(int));

int main (void) {
     pid_t childpid;
     char inbuf[MAX_CANON];
     int len;
     sigset_t blockmask;
     struct sigaction defaction;

     /* 最后一个参数为处理函数，这里是忽略到捕捉到的特定信号 */ 
     if (signal_setup(&defaction, &blockmask, SIG_IGN) == -1) {
         perror("Failed to setup shell signal handler...");
         return 1;
     }
//     if (sigprocmask(SIG_BLOCK, &blockmask, NULL) == -1) {
//         perror("Failed to set blockmask...");
//         return 1;
//     }

     for( ; ; ) {
        if (fputs(PROMPT_STRING, stdout) == EOF)
            continue;
        if (fgets(inbuf, MAX_CANON, stdin) == NULL)
            continue; 
        len = strlen(inbuf);
        if (len == 1 && inbuf[len - 1] == '\n')   /* 若直接按回车就忽略 */ 
            continue;
        if (inbuf[len - 1] == '\n')
            inbuf[len - 1] = 0;
        if (strcmp(inbuf, QUIT_STRING) == 0)
            break;
        if ((childpid = fork()) == -1)
            perror("Failed to fork child");
        else if (childpid == 0) {
            /* 子进程接触信号忽略 */ 
            if ((sigaction(SIGINT, &defaction, NULL) == -1) || 
                (sigaction(SIGQUIT, &defaction, NULL) == -1) ||
                (sigprocmask(SIG_UNBLOCK, &blockmask, NULL) == -1)) {
                perror("Failed to set signal handling for command...");
                return 1;
            }
            execute_cmd(inbuf);
            return 1; 
        }  
        wait(NULL);
     }
     return 0; 
}
