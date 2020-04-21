#include <signal.h>
#include <stdio.h>

/* 参数def保留原有的信号结构信息，参数mask存放信号集合，
 * 参数handler是信号处理的函数
 **/ 
int signal_setup(struct sigaction *def, sigset_t *mask, void (*handler)(int)) {
    /* default sigaction */ 
    def->sa_handler = SIG_DFL;
    def->sa_flags = 0;

    /* 创建信号结构，对于捕捉到的信号进行处理 */ 
    struct sigaction catch;
    catch.sa_handler = handler;
    catch.sa_flags = 0;

    if (sigemptyset(&(def->sa_mask)) == -1 ||
       (sigemptyset(&(catch.sa_mask)) == -1) ||
       (sigaddset(&(catch.sa_mask), SIGINT) == -1) || 
       (sigaddset(&(catch.sa_mask), SIGQUIT) == -1) || 
       /* change the action */ 
       (sigaction(SIGINT, &catch, NULL) == -1) || 
       (sigaction(SIGQUIT, &catch, NULL) == -1) || 
       (sigemptyset(mask) == -1) ||
       (sigaddset(mask, SIGINT) == -1) ||
       (sigaddset(mask, SIGQUIT) == -1))
        return -1;
    
    return 0;
}
