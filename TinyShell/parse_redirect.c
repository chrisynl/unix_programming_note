/**
 * @Filename: parse_redirect.c
 * @Description: 处理输入输出的重定向，按特定顺序调用，为了直观，忽略了错误检查。
 */
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/stat.h>
#define FFLAG (O_WRONLY | O_CREAT | O_TRUNC)
#define FMODE (S_IRUSR | S_IWUSR)

int parse_redirect_in(char *cmd) {    /* 遇到'<'重定向输入 */
   int infd;
   char *infile;

   infile = strchr(cmd, '<');
   if (infile == NULL)
       return 0;                   /* 不用处理即返回 */
   *infile = 0;                  /* 把'<'后面的字符串都去掉，因为处理了 */
   infile = strtok(infile + 1, " \t");
   infd = open(infile, O_RDONLY);
   dup2(infd, STDIN_FILENO);
   return close(infd);
}

int parse_redirect_out(char *cmd) {  /* 遇到'>'重定输出 */
   int outfd;
   char *outfile;

   outfile = strchr(cmd, '>');
   if (outfile == NULL)           /* 不用处理即返回 */
       return 0;
   *outfile = 0;                  /* 将'>'后面的字符串都去掉，因为处理了 */
   outfile = strtok(outfile + 1, " \t");  
   outfd = open(outfile, FFLAG, FMODE);
   dup2(outfd, STDOUT_FILENO);
   return close(outfd);
} 
