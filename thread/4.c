// from apue
//an error demonstration
#include<pthread.h>
#include<stdio.h>
#include<stdlib.h>

void cleanup(void *arg);
void* thread1(void *arg);
void* thread2(void* arg);

int main()
{
    int         err;
    pthread_t   tid1, tid2;
    void        *tret;

    err = pthread_create(&tid1, NULL, thread1, (void*)1);
    if (err != 0) {
        exit(-1);
    }
    err = pthread_create(&tid2, NULL, thread2, (void*)1);
    if (err != 0) {
        exit(-1);
    }
    err = pthread_join(tid1, (void*)&tret);
    if (err != 0) {
        exit(-1);
    }
    printf("thread1 exit code %ld\n", (long)tret);
    err = pthread_join(tid2, (void*)&tret);
    if (err != 0) {
        exit(-1);
    }
    printf("thread2 exit code %ld\n", (long)tret);  
    exit(0);
}
void cleanup(void *arg)
{
    printf("cleanup: %s\n", (char*)arg);
}

void* thread1(void *arg)
{
    // the two handlers will not be execute because thread1 call return to exit.
    printf("thread1 start\n");
    pthread_cleanup_push(cleanup, "thread1 first handler");
    pthread_cleanup_push(cleanup, "thread1 second handler");
    printf("thread1 push complete\n");
    if (arg) {
        return ((void*)1);
    }
    pthread_cleanup_pop(0);
    pthread_cleanup_pop(0);
    return ((void*)1);
}

void* thread2(void *arg)
{
    // the two handlers will be execute because thread2 call pthread_exit.
    printf("thread2 start\n");
    pthread_cleanup_push(cleanup, "thread2 first handler");
    pthread_cleanup_push(cleanup, "thread2 second handler");
    printf("thread2 push complete\n");
    if (arg) {
        pthread_exit((void*)2);
    }
    pthread_cleanup_pop(0);
    pthread_cleanup_pop(0);
    pthread_exit((void*)2);
}

