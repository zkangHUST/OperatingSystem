// from apue
//an error demonstration
#include<pthread.h>
#include<stdio.h>
#include<stdlib.h>
long            f_count;

void* thread(void *arg);

int main(int argc, char *argv[])
{
    int         i, err, time;
    pthread_t   tid[3];
    void        *tret;

    if (argc != 2) {
        exit(-1);
    }

    time = atoi(argv[1]);
    
    for (i = 0; i < 3; i++) {
        err = pthread_create(&tid[i], NULL, thread, &time);
        if (err != 0) {
            exit(-1);
        }
    }
    for (i = 0; i < 3; i++) {
        err = pthread_join(tid[i], (void*)&tret);
        printf("thread[%d] exit code %ld\n", i, (long)tret);
        if (err != 0) {
            exit(-1);
        }
    }
    printf("f_count = %ld\n", f_count);
    exit(0);
}

void* thread(void *arg)
{
    long i, times = *(long*)arg;
    for (i = 0; i < times; i++) {
        f_count++;
    }
    return ((void*)0);
}