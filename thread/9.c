#include<pthread.h>
#include<stdio.h>
#include<stdlib.h>
void* write_thread(void *arg);
void* read_thread(void* arg);

pthread_rwlock_t      rwlock;
long                 f_count = 0;


int main(int argc, char *argv[])
{
    int         i, err, time;
    pthread_t   wtid[3];
    void        *tret;

    if (argc != 2) {
        exit(-1);
    }

    time = atoi(argv[1]);

    printf("time = %d\n", time);
    pthread_t* rtid = (pthread_t*)malloc(sizeof(pthread_t) * time);
    if (rtid == NULL) {
        printf("allocl error\n");
        exit(-1);
    }
    if (pthread_rwlock_init(&rwlock, NULL) != 0) {
        exit(-1);
    }
    // create write threads
    for (i = 0; i < 3; i++) {
        err = pthread_create(&wtid[i], NULL, write_thread, &time);
        if (err != 0) {
            exit(-1);
        }
    }
    // create read threads
    for (i = 0; i < time; i++) {
        err = pthread_create(&rtid[i], NULL, read_thread, NULL);
        if (err != 0) {
            exit(-1);
        }
    }

    for (i = 0; i < 3; i++) {
        err = pthread_join(wtid[i], (void*)&tret);
        printf("wthread[%d] exit code %ld\n", i, (long)tret);
        if (err != 0) {
            exit(-1);
        }
    }
    for (i = 0; i < time; i++) {
        err = pthread_join(rtid[i], (void*)&tret);
        printf("thread[%d] fcount = %ld\n", i, (long)tret);
        if (err != 0) {
            exit(-1);
        }
    }
    free(rtid);
    printf("f_count = %ld\n", f_count);
    exit(0);
}

void* write_thread(void *arg)
{
    int i, times = *(int*)arg;
    
    for (i = 0; i < times; i++) {
        pthread_rwlock_wrlock(&rwlock);
        f_count++;
        pthread_rwlock_unlock(&rwlock);
    }
    return ((void*)0);
}

void* read_thread(void* arg)
{
    long res;
    pthread_rwlock_rdlock(&rwlock);
    res = f_count;
    // if (f_count % 10000 == 0) {
    //     printf("count = %ld\n", f_count);
    // }
    pthread_rwlock_unlock(&rwlock);
    return ((void*)res);
}
