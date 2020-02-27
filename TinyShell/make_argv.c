/**
 * @Filename: make_argv.c
 * @Description: 处理字符串，形成参数数组
 */
 

#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/** s是传入字符串
 *  delimiters是分隔符
 *  argvp是形成的参数数组
 */ 
int make_argv(const char *s, const char *delimiters, char ***argvp) {
   int error;
   int i;
   int numtokens;  /* 形成的token总数 */ 
   const char *snew;
   char *t;

   if ((s == NULL) || (delimiters == NULL) || (argvp == NULL)) {
      errno = EINVAL;
      return -1;
   }
   *argvp = NULL;                           
   snew = s + strspn(s, delimiters);         /* snew是字符串的真正的开始，去掉前面多余的分隔符*/
   if ((t = malloc(strlen(snew) + 1)) == NULL)    
      return -1; 
   strcpy(t, snew);               
   numtokens = 0;
   if (strtok(t, delimiters) != NULL)     /* 计算token的数目，for从1开始是因为参数数组最后一个空间要给NULL */
      for (numtokens = 1; strtok(NULL, delimiters) != NULL; numtokens++) ; 

   /* 创建参数数组 */
   if ((*argvp = malloc((numtokens + 1)*sizeof(char *))) == NULL) {
      error = errno;
      free(t);
      errno = error;
      return -1; 
   } 
   /* 对每个子串进行拷贝 */
   if (numtokens == 0) 
      free(t);
   else {
      strcpy(t, snew);
      **argvp = strtok(t, delimiters);
      for (i = 1; i < numtokens; i++)
          (*argvp)[i] = strtok(NULL, delimiters);
    } 
    (*argvp)[numtokens] = NULL;             /* 最后放NULL */
    return numtokens;
}    
