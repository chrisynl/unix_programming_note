/**
 * @Filename: mutex_demo.c
 * @Description: 利用子线程计算0-1范围内正弦函数的平均值，并与实际值进行误差比较。
 */
 
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>

#define TEM_MILLION 10000000L

static int doneflag = 0;
static int count = 0;
static double sum = 0;
static pthread_mutex_t flaglock = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t sumlock = PTHREAD_MUTEX_INITIALIZER;


/* 线程函数, 计算随机和 */ 
void *compute_thread(void *arg1);
int set_done(void);
int get_done(int *flag);
int randsafe(double *valp);
int add(double x);
int show_results(void);

int main(int argc, char *argv[])
{
    int i;
    int num_threads;
    int sleep_time;
    pthread_t *tids; 

    if (argc != 3) {
        fprintf(stderr, "Usage: %s num_threads sleep_time\n", argv[0]);
        return 1;
    }
    num_threads = atoi(argv[1]);
    sleep_time = atoi(argv[2]);
    if ((tids = (pthread_t *)calloc(num_threads, sizeof(pthread_t))) == NULL) {
        perror("Failed to allocate space for thread IDs");
        return 1;
    }
    for (i = 0; i < num_threads; ++i)        /* 创建num_threads个compute_thread线程 */ 
        pthread_create(tids + i, NULL, compute_thread, NULL);
    sleep(sleep_time);
    set_done();
    for (i = 0; i < num_threads; ++i)  /* 等待线程完成 */ 
        pthread_join(tids[i], NULL);

    if (show_results())
        return 1;

    return 0;
}


/* 线程函数, 计算随机和 */ 
void *compute_thread(void *arg1) {
    int localdone = 0;
    struct timespec sleep_local;
    double val;    
    
    sleep_local.tv_sec = 0;
    sleep_local.tv_nsec = TEM_MILLION; /* 10ms */ 

    while (!localdone) {
        randsafe(&val);
        add(sin(val));
        get_done(&localdone);
        nanosleep(&sleep_local, NULL); /* 让其他线程进入 */
    }
}

int set_done(void) {
    pthread_mutex_lock(&flaglock);
    doneflag = 1; 
    return  pthread_mutex_unlock(&flaglock);
}

int get_done(int *flag) {
    pthread_mutex_lock(&flaglock);
    *flag = doneflag;
    return pthread_mutex_unlock(&flaglock);
}

int randsafe(double *valp) {
    static pthread_mutex_t randlock = PTHREAD_MUTEX_INITIALIZER;
    *valp = (double)rand() / (double)RAND_MAX;
    return pthread_mutex_unlock(&randlock);
}

int add(double x) {
    pthread_mutex_lock(&sumlock);
    sum += x;
    count++;
    return pthread_mutex_unlock(&sumlock);
}

int show_results(void) {
    int res_count;
    double res_sum;
    double calculated;
    double average;
    double err;
    double perr;

    pthread_mutex_lock(&sumlock);
    res_sum = sum;
    res_count = count;
    pthread_mutex_unlock(&sumlock);

    if (count == 0)
        printf("No values were summed.\n");
    else {
        calculated = 1.0 - cos(1.0);
        average = sum/count;
        err = average - calculated;
        perr = 100.0*err/calculated;
        printf("The sum is %f and the count is %d\n", sum, count);
        printf("The average is %f and error is %f or %f%%\n", average, err, perr);
    }

    return 0;
}
