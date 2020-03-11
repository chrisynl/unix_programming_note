/**
 * @Filename: execute_redirect.c
 * @Description: 对可能带有重定向的单个代码进行处理
 */
 
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int make_argv(const char *s, const char *delimiters, char ***argvp);
int parse_redirect_in(char *s);
int parse_redirect_out(char *s);


/**
 * in为1, 表示需要处理输入重定向
 * out为1, 表示需要处理输出重定向
 */ 
void execute_redirect(char *s, int in, int out) {
    char **chargv;
    char *pin;
    char *pout;

    /**
     * 实现了对于"sort > f2 < f1"这种小于符号在后的命令处理
     * 之前只能处理小于符号在大于符号之前. 
     */ 
    if (in && ((pin = strchr(s, '<')) != NULL) &&
        out && ((pout = strchr(s, '>')) != NULL) && (pin > pout)) {
        /* 先处理输入重定向, 处理完之后就把in置0 */ 
        if (parse_redirect_in(s) == -1) {
            perror("Failed to redirect input");
            return ;
        }
        in = 0;
    }

    /* 一般情况按先处理输出重定向再处理输入重定向 */ 
    if (out && (parse_redirect_out(s) == -1))
        perror("Failed to redirect output");
    else if (in && (parse_redirect_in(s) == -1))
        perror("Failed to redirect input");
    else if (make_argv(s, " \t", &chargv) <= 0)
        fprintf(stderr, "Failed to parse command line\n");
    else {
        execvp(chargv[0], chargv);
        perror("Failed to execute command");
    }
    exit(1);
}
