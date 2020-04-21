/**
 * @Filename: tsh3.c
 * @Description: 在tsh2的基础上进行提升，按<C-c>会刷新行，显示提示符
 *     利用了siglongjmp和sigsetjump
 */
 
#include <limits.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <setjmp.h>

#define PROMPT_STRING "tsh3>> "
#define QUIT_STRING "q"  /* 按q退出 */ 


void execute_cmd(char *incmd);
int signal_setup(struct sigaction *def, sigset_t *mask, void (*handler)(int));

static sigjmp_buf jump_to_prompt;
static volatile sig_atomic_t jp = 0;

/* 信号处理函数 handler */ 
static void jump_handler(int signalnum) {
    if (!jp) return;
    jp = 0;
    siglongjmp(jump_to_prompt, 1);
}

int main (void) {
     pid_t childpid;
     char inbuf[MAX_CANON];
     int len;
     sigset_t blockmask;
     struct sigaction defaction;

     if (signal_setup(&defaction, &blockmask, jump_handler) == -1) {
         perror("Failed to setup shell signal handler...");
         return 1;
     }

     for( ; ; ) {
         if ((sigsetjmp(jump_to_prompt, 1)) &&  /* 如果从信号中返回则换行 */ 
             (fputs("\n", stdout) == EOF))
             continue;
         wait(NULL);
         jp = 1;
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
         if (sigprocmask(SIG_BLOCK, &blockmask, NULL) == -1)
             perror("Failed to block signals...");
         if ((childpid = fork()) == -1)
             perror("Failed to fork child");
         else if (childpid == 0) {
             /* 子进程解除信号忽略 */ 
             if ((sigaction(SIGINT, &defaction, NULL) == -1) || 
                 (sigaction(SIGQUIT, &defaction, NULL) == -1) ||
                 (sigprocmask(SIG_UNBLOCK, &blockmask, NULL) == -1)) {
                 perror("Failed to set signal handling for command...");
                 return 1;
             }
             execute_cmd(inbuf);
             return 1; 
            }  
         if (sigprocmask(SIG_UNBLOCK, &blockmask, NULL) == -1)
             perror("Failed to unblock signals...");
     }
     return 0; 
}
