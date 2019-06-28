// from apue
//an error demonstration
#include<pthread.h>
#include<stdio.h>
#include<stdlib.h>

struct foo {
    int a, b, c, d;
};
void print_foo();
void* thread1(void *arg);
void* thread2(void *arg);

int main()
{
    int         err;
    pthread_t   tid1, tid2;
    struct foo  *fp;
    
    err = pthread_create(&tid1, NULL, thread1, NULL);
    if (err != 0) {
        exit(-1);
    }
    err = pthread_join(tid1, (void*)&fp);
    if (err != 0) {
        exit(-1);
    }
    sleep(1);
    printf("parent starting second thread\n"); 
    err = pthread_create(&tid2, NULL, thread2, NULL);
    if (err != 0) {
        exit(-1);
    }   
    sleep(1);
    print_foo("parent:\n", fp);
    exit(0);
}

void* thread1(void *arg)
{   
    // foo 结构在thread1的栈上分配,thread1执行完成后，foo所占用的内存可能被别的线程改写,因此是不安全的.
    struct foo foo = {1, 2, 3, 4};
    print_foo("thread1:\n", &foo);
    pthread_exit((void*)&foo);
}

void* thread2(void *arg)
{
    pthread_t tid = pthread_self();
    printf("thread2: ID is %lu (0x%lx)\n", (unsigned long)tid, (unsigned long)tid);
    pthread_exit((void*)0);
}

void print_foo(const char* s, const struct foo* fp)
{
    printf("%s", s);
    printf(" structure at 0x%lx\n", (unsigned long)fp);
    printf(" foo.a = %d\n", fp->a);
    printf(" foo.b = %d\n", fp->b);
    printf(" foo.c = %d\n", fp->c);
    printf(" foo.d = %d\n", fp->d);
}