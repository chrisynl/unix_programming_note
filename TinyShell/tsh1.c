/**
 * @Filename: tsh1.c
 * @Description: tiny shell 的main函数
 */
 
#include <limits.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define PROMPT_STRING "tsh1>> "
#define QUIT_STRING "q"  /* 按q退出 */ 


void execute_cmd(char *incmd);

int main (void) {
   pid_t childpid;
   char inbuf[MAX_CANON];
   int len;

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
          execute_cmd(inbuf);
          return 1; 
      } else 
         wait(NULL);
   }
   return 0; 
}
