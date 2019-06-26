// from apue
#include<pthread.h>
#include<stdio.h>
#include<stdlib.h>

void* thread1(void *arg);
void* thread2(void *arg);

int main()
{
    int err;
    pthread_t tid1, tid2;
    void *tret;

    err = pthread_create(&tid1, NULL, thread1, NULL);
    if (err != 0) {
        exit(-1);
    }
    err = pthread_create(&tid2, NULL, thread2, NULL);
    if (err != 0) {
        exit(-1);
    }   
    err = pthread_join(tid1, &tret);
    if (err != 0) {
        exit(-1);
    }   
    printf("thread1 exit code %ld\n", (long)tret);
    err = pthread_join(tid2, &tret);
        if (err != 0) {
        exit(-1);
    }  
    printf("thread2 exit code %ld\n", (long)tret);
    exit(0);
}

void* thread1(void *arg)
{
    printf("thread1 returning\n");
    // sleep(1);
    return ((void*)1);
}

void* thread2(void *arg)
{
    printf("thread2 returning\n");
    return ((void*)2);
}
