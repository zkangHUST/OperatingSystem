
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
pthread_mutex_t read_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t write_mutex = PTHREAD_MUTEX_INITIALIZER;

void *write(void *temp)
{
    FILE *file1;
    char *str;
    pthread_mutex_lock(&write_mutex);
    sleep(1);
    pthread_mutex_lock(&read_mutex);

    printf("File locked, now you can write:\n");
    str = (char *)malloc(100 * sizeof(char));
    file1 = fopen("temp", "w");
    scanf("%s", str);
    fprintf(file1, "%s", str);
    fclose(file1);

    pthread_mutex_unlock(&read_mutex);
    pthread_mutex_unlock(&write_mutex);

    printf("Write finish!\n");
    return ((void*)0);
}

void *read(void *temp)
{
    FILE *file1;
    char *str;
    
    pthread_mutex_lock(&write_mutex);
    sleep(1);
    pthread_mutex_lock(&read_mutex);

    printf("the file content is:\n");
    file1 = fopen("temp", "r");
    if (file1 == NULL) {
        printf("open file error!");
        exit(-1);
    }
    str = (char *)malloc(100 * sizeof(char));
    fscanf(file1, "%s", str);
    printf("%s\n", str);
    fclose(file1);

    pthread_mutex_unlock(&read_mutex);
    pthread_mutex_unlock(&write_mutex);
    
    return ((void*)0);
}

int main()
{

    pthread_t thread_id, thread_id1;
    pthread_attr_t attr;
    int ret;
    void *res;
    ret = pthread_create(&thread_id, NULL, write, NULL);
    if (ret != 0) {
        exit(-1);
    }
    ret = pthread_create(&thread_id1, NULL, read, NULL);
    if (ret != 0) {
        exit(-1);
    }
    printf("Created thread success!\n");
    pthread_join(thread_id, &res);
    pthread_join(thread_id1, &res);
    exit(0);
}
