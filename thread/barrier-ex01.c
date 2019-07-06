#include<pthread.h>
#define NTHR 10
void* task(void *arg);

pthread_barrier_t barrier;

int main()
{
    int err, i;
    pthread_t tid;

    pthread_barrier_init(&barrier, NULL, NTHR + 1);
    for (i = 0; i < NTHR; i++) {
        err = pthread_create(&tid, NULL, task, (void*)(i));
        if (err != 0) {
            printf("create thread error!\n");
            return -1;
        }
    }
    pthread_barrier_wait(&barrier);
    printf("mission accomplished!\n");
    return 0;
}

void* task(void *arg)
{
    int i, t;

    i = (int)arg;
    t = rand() % 10 + 1;
    sleep(t);
    printf("task %d accomplished in %d minutes\n", i, t);
    pthread_barrier_wait(&barrier);
    return ((void*)0);
}
