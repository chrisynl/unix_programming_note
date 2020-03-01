/**
 * @Filename: execute_cmd_redirect.c
 * @Description: 处理出入输出的重定向，为了直观，省去了错误检查
 */
 
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int make_argv(const char *s, const char *delimiters, char ***argvp);
int parse_redirect_in(char *s);
int parse_redirect_out(char *s);

void execute_cmd(char *incmd) {
   char **chargv;
   parse_redirect_out(incmd);   /* 先处理输出重定向 */ 
   parse_redirect_in(incmd);  /* 处理输入重定向 */ 
   make_argv(incmd, " \t", &chargv);
   execvp(chargv[0], chargv);
   
   exit(1);
}
