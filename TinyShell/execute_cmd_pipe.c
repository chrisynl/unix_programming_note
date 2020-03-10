/**
 * @Filename: execute_cmd_pipe.c
 * @Description: 处理流水线的execute_cmd函数，为了直观，省略了错误检查。
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void execute_redirect(char *s, int in, int out);
int make_argv(const char *, const char *delimiters, char ***argvp);

void execute_cmd(char *cmds) {
    int child;
    int count;
    int fds[2];
    int i;
    char **pipelist;

    count = make_argv(cmds, "|", &pipelist);
    if (count <= 0) {
        fprintf(stderr, "Failed to find any commands\n");
        exit(1);
    }

    /* 处理流水线, 比如"cmd1 | cmd 2 | cmd3" */ 
    for (i = 0; i < count - 1; i++) {
        pipe(fds);
        child = fork();
        if (child) {
            dup2(fds[1], STDOUT_FILENO);
            close(fds[0]);
            close(fds[1]);
            /* 只有第一条命令才可能有输入重定向 */ 
            execute_redirect(pipelist[i], i == 0, 0);
            exit(1);
        }
        dup2(fds[0], STDIN_FILENO);
        close(fds[0]);
        close(fds[1]);
    }

    /**
     * 处理最后一条命令, 只有最后一条命令才可能有输出的重定向
     * 当只有一条命令时且有输入输出重定向时, 上面的for循环不执行, 且in,out都为1
     */ 
    execute_redirect(pipelist[i], i == 0, 1);
    exit(1);
}
