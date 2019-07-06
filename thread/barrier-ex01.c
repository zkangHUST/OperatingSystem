#include<pthread.h>
#include<stdio.h>
#include<limits.h>
#include<stdlib.h>
#include<time.h>
#include "heapsort.h"
#define NTHR 100
#define NUMNUM (800 * 10000)
#define TNUM (NUMNUM / NTHR)

void* sort(void *arg);
void merge();

int nums[NUMNUM];
pthread_barrier_t barrier;

int main()
{
    int err, i;
    pthread_t tid;
    printf("INT_MAX = %d\n", INT_MAX);

    srand(time(NULL));  
    for (i = 0; i < NUMNUM; i++) {
        // sleep_ms(1);
        nums[i] = rand() % INT_MAX;
    }
    pthread_barrier_init(&barrier, NULL, NTHR + 1);
    for (i = 0; i < NTHR; i++) {
        err = pthread_create(&tid, NULL, sort, (void*)(i * TNUM));
        if (err != 0) {
            printf("create thread error!\n");
            return -1;
        }
    }
    
    pthread_barrier_wait(&barrier);

    merge();
    printf("-------------------------------------\n");
    for (i = 0; i < NUMNUM; i++) {
        // nums[i] = random();
        printf("%10d \n", nums[i]);
        if ((i + 1) % TNUM == 0) {
            printf("-------------------------------------\n");
        }
    }
    return 0;
}

void merge()
{
    int idx[NTHR];
    int num, i, sidx, minidx;

    for (i = 0; i < NTHR; i++) {
        idx[i] = i * TNUM;
    }
    
    for (sidx = 0; sidx < NUMNUM; sidx++) {
        num = INT_MAX;
        for (i = 0; i < NTHR; i++) {
            if ((idx[i] < (i + 1) * TNUM) && nums[idx[i]] < num) {
                num = nums[idx[i]];
                minidx = i;
            }
        }    
        nums[sidx] = nums[idx[minidx]];
        idx[minidx]++;
    }
}

void* sort(void *arg)
{
    int idx;
    idx = (int)arg;
    heap_sort(&nums[idx], TNUM);
    printf("%d - %d have been sorted.\n", idx, idx + TNUM - 1);
    pthread_barrier_wait(&barrier);
    return ((void*)0);
}
