/**************************************
*文件说明:threadpool.h
*作者:高小调
*创建时间:2017年08月21日 星期一 12时28分18秒
*开发环境:Kali Linux/g++ v6.3.0
****************************************/
#ifndef __THREADPOOL_H_
#define __THREADPOOL_H_
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <errno.h>

#define DEFAULT_TIME 10                 /*10s检测一次*/
#define MIN_WAIT_TASK_NUM 10            /*如果queue_size > MIN_WAIT_TASK_NUM 添加新的线程到线程池*/ 
#define DEFAULT_THREAD_VARY 10          /*每次创建和销毁线程的个数*/
#define true 1
#define false 0

typedef struct {
    void *(*function)(void *);          /* 函数指针，回调函数 */
    void *arg;                          /* 上面函数的参数 */
} threadpool_task_t;                    /* 各子线程任务结构体 */

/* 描述线程池相关信息 */
struct threadpool_t {
    pthread_mutex_t lock;               /* 用于锁住本结构体 */    
    pthread_mutex_t thread_counter;     /* 记录忙状态线程个数de琐 -- busy_thr_num */
    pthread_cond_t queue_not_full;      /* 当任务队列满时，添加任务的线程阻塞，等待此条件变量 */
    pthread_cond_t queue_not_empty;     /* 任务队列里不为空时，通知等待任务的线程 */

    pthread_t *threads;                 /* 存放线程池中每个线程的tid。数组 */
    pthread_t adjust_tid;               /* 存管理线程tid */
    threadpool_task_t *task_queue;      /* 任务队列 */

    int min_thr_num;                    /* 线程池最小线程数 */
    int max_thr_num;                    /* 线程池最大线程数 */
    int live_thr_num;                   /* 当前存活线程个数 */
    int busy_thr_num;                   /* 忙状态线程个数 */
    int wait_exit_thr_num;              /* 要销毁的线程个数 */

    int queue_front;                    /* task_queue队头下标 */
    int queue_rear;                     /* task_queue队尾下标 */
    int queue_size;                     /* task_queue队中实际任务数 */
    int queue_max_size;                 /* task_queue队列可容纳任务数上限 */

    int shutdown;                       /* 标志位，线程池使用状态，true或false */
};

typedef struct threadpool_t threadpool_t;
//创建线程池
threadpool_t *threadpool_create(int min_thr_num, int max_thr_num, int queue_max_size);
//添加任务
int threadpool_add(threadpool_t *pool, void*(*function)(void *arg), void *arg);
//销毁线程池
int threadpool_destroy(threadpool_t *pool);
//获取线程数目
int threadpool_all_threadnum(threadpool_t *pool);
//获取正在工作的线程数目
int threadpool_busy_threadnum(threadpool_t *pool);
//工作线程
void *threadpool_thread(void *threadpool);
//管理线程
void *adjust_thread(void *threadpool);
//判断某个线程是否“活着”
int is_thread_alive(pthread_t tid);
//释放线程池
int threadpool_free(threadpool_t *pool);
#endif
