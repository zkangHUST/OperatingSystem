#include<pthread.h>
#include<stdio.h>
#include<stdlib.h>
void *print_A(void*arg);
void *print_B(void*arg);
void *print_C(void*arg);

int cnt = 0; 
int state = 0; // 0: print A; 1: print B; 2: print C
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
int main()
{
    int         err, i;
    pthread_t   ntid[3];

    err = pthread_create(&ntid[0], NULL, print_A, NULL);
    if (err != 0) {
        printf("cann't create new thread!\n");
        exit(-1);
    }
    err = pthread_create(&ntid[1], NULL, print_B, NULL);
    if (err != 0) {
        printf("cann't create new thread!\n");
        exit(-1);
    }
   
    err = pthread_create(&ntid[2], NULL, print_C, NULL);
    if (err != 0) {
        printf("cann't create new thread!\n");
        exit(-1);
    }

    pthread_join(ntid[0], NULL);   
    pthread_join(ntid[1], NULL);
    pthread_join(ntid[2], NULL);

    return 0;
}

void *print_A(void*arg) 
{
    while(1) {
        pthread_mutex_lock(&lock);
        if (cnt == 10) {
            pthread_mutex_unlock(&lock);
            return ((void*)0);
        }
        if (state == 0) {
            printf("A");
            state = 1;
        }
        pthread_mutex_unlock(&lock);
    }
}
void *print_B(void*arg) 
{
    while(1) {
        pthread_mutex_lock(&lock);
        if (cnt == 10) {
            pthread_mutex_unlock(&lock);
            return ((void*)0);
        }
        if (state == 1) {
            printf("B");
            state = 2;
        }
        pthread_mutex_unlock(&lock);
    }
}
void *print_C(void*arg) 
{
    while(1) {
        pthread_mutex_lock(&lock);
        if (cnt == 10) {
            pthread_mutex_unlock(&lock);
            return ((void*)0);
        }
        if (state == 2) {
            printf("C");
            cnt++;
            state = 0;
        }
        pthread_mutex_unlock(&lock);
    }
}