// from apue
//an error demonstration
#include<pthread.h>
#include<stdio.h>
#include<stdlib.h>

struct foo {
    int             f_count;
    pthread_mutex_t f_lock;
    int             f_id;
};

struct foo* foo_alloc(int id);
void foo_hold(struct foo* fp);
void foo_rele(struct foo* fp);
void* thread1(void *arg);

int main()
{
    int         i, err;
    void        *tret;
    pthread_t   tid[3];

    struct foo* fp = foo_alloc(0);
    for (i = 0; i < 3; i++) {
        err = pthread_create(&tid[i], NULL, thread1, fp);
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
    foo_rele(fp);
    exit(0);
}

void* thread1(void *arg)
{
    struct foo* fp = (struct foo*)arg;
    foo_hold(fp);
    fp->f_id++;
    printf("fp->id = %d\n", fp->f_id);
    foo_rele(fp);
    return ((void*)0);
}

struct foo* foo_alloc(int id)
{
    struct foo* fp;
    fp = malloc(sizeof(struct foo));
    if (fp != NULL) {
        fp->f_count = 1;
        fp->f_id = id;
        if (pthread_mutex_init(&fp->f_lock, NULL) != 0) {
            free(fp);
            return(NULL);
        }
    }
    return fp;
}

void foo_hold(struct foo* fp)
{
    pthread_mutex_lock(&fp->f_lock);
    fp->f_count++;
    // printf("f_count = %d\n", fp->f_count);
    pthread_mutex_unlock(&fp->f_lock);
}

void foo_rele(struct foo* fp)
{
    pthread_mutex_lock(&fp->f_lock);
    if (--fp->f_count == 0) {
        pthread_mutex_unlock(&fp->f_lock);
        pthread_mutex_destroy(&fp->f_lock);
        printf("free fp\n");
        free(fp);
    } else {
        pthread_mutex_unlock(&fp->f_lock);
    }
}