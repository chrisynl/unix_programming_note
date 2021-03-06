# 线程同步 - POSIX互斥锁
***

### 概括

本文讲解POSIX中互斥量的基本用法，从而能达到简单的线程同步。`互斥量`是一种特殊的变量，它有两种状态：锁定以及解锁。如果互斥量是锁定的，就有一个特定的线程持有或者拥有这个互斥量；如果没有线程持有这个互斥量，我们就说这个互斥量是解锁的、可用的。同时，互斥量还有一个等待持有该互斥量的线程队列。互斥队列中的线程获得互斥量的顺序由线程调度所决定，但POSIX没有要求实现任何特定的策略。

### 程序描述
现在我们尝试写一个程序来体会互斥量的基本应用，用程序来模拟验证：$$\int_0^1 sinxdx = 1.0 - cos(1)$$ 利用多个子线程来产生[0,1]之间的随机数，每产生一次`count`则增加一，并且将产生的数加入`sum`，然后用`sum/count`来模拟等式左边，最后计算等式右边作为标准值，然后计算误差。
程序用法：从命令行参数中接受要创建的线程数目以及运行时间，即`$ ./程序名 线程数 等待时间`。

全局变量设计：
```c
static int doneflag = 0;
static int count = 0;
static double sum = 0;
static pthread_mutex_t flaglock = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t sumlock = PTHREAD_MUTEX_INITIALIZER;
```
用`sumlock`来守护`count`和`sum`，用`flaglock`来守护`doneflag`。

`main`函数里创建线程数组，然后用`pthread_create`来创建线程，用`pthread_join`来等待线程。关键语句：
```c
num_threads = atoi(argv[1]);
tids = (pthread_t *)calloc(num_threads, sizeof(pthread_t);
for (i = 0; i < num_threads; ++i)        /* 创建num_threads个compute_thread线程 */
    pthread_create(tids + i, NULL, compute_thread, NULL);
```

##### 线程函数
注意创建的线程处理函数的形式：`void *(*start_routine) (void *)`, 将函数指针（函数名）传递给`pthread_create`即可。子进程循环计算，直到`doneflag == 1`。

##### 其它函数
`randsafe()`：用一个互斥量来保护`rand()`，要确保不会有两个线程同时调用rand，因为它在多线程中是不安全的。其次，rand不是一个特别好的伪随机数生成器，所以应该在实际程序中避免使用它，这里只是用作demo示范。
`set_done`和`get_done`分别用于设置flag和获得flag。
最后`show_results`来整理数据并进行输出。


### 运行情况
编译：`gcc -o demo mutex_demo.c -lm -lpthread`
```bash
$ gcc -o demo mutex_demo.c -lm -lpthread
$ ./demo 5 1
The sum is 232.913662 and the count is 500
The average is 0.465827 and error is 0.006130 or 1.333405%
$ ./demo 10 1
The sum is 467.382538 and the count is 1000
The average is 0.467383 and error is 0.007685 or 1.671717%
$ ./demo 10 2
The sum is 919.177364 and the count is 1990
The average is 0.461898 and error is 0.002200 or 0.478680%
```
