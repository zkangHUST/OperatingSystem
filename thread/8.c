// from apue
//an error demonstration
#include<pthread.h>
#include<stdio.h>
#include<stdlib.h>
void* thread(void *arg);

pthread_mutex_t f_lock;
long            f_count;

int main(int argc, char *argv[])
{
    int         i, err, time;
    pthread_t   tid[3];
    void        *tret;

    if (argc != 2) {
        exit(-1);
    }

    time = atoi(argv[1]);
    if (pthread_mutex_init(&f_lock, NULL) != 0) {
        exit(-1);
    }
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
        pthread_mutex_lock(&f_lock);
        f_count++;
        pthread_mutex_unlock(&f_lock);
    }
    return ((void*)0);
}