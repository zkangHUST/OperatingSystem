// from apue
#include<pthread.h>
#include<stdio.h>
#include<stdlib.h>
void printThreadId(const char *s);
void *test(void* arg);
pthread_t ntid;

int main()
{
    int err;
    err = pthread_create(&ntid, NULL, test, NULL);
    if (err != 0) {
        printf("cann't create new thread!\n");
        exit(-1);
    }
    printThreadId("main thread:");
    sleep(1);
    return 0;
}

void printThreadId(const char *s)
{
    pid_t pid;
    pthread_t tid;

    pid = getpid();
    tid = pthread_self();
    printf("%s pid %lu tid %lu (0x%lx)\n", s, (unsigned long)pid, (unsigned long)tid, (unsigned long)tid);
}   

void *test(void* arg)
{
    printThreadId("new thread: ");
    return ((void*)0);
}