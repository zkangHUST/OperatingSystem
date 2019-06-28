#include<pthread.h>
#include<stdio.h>
#include<stdlib.h>
void *do_download(void * arg);
void *check_download_status(void * arg);

int process_bar = 0;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
pthread_mutex_t qlock = PTHREAD_MUTEX_INITIALIZER;

int main(int argc, char* argv[])
{
	pthread_t 	*tid;
	int err, i, cnt;
	if (argc != 2) {
		printf("usage:./a.out threadnum(threadnum >= 2)\n");
		exit(0);
	}
	cnt = atoi(argv[1]);
	if (cnt < 2) {
		printf("thread num must be no less than 2\n");
		exit(0);
	}

	tid = (pthread_t*)malloc(sizeof(pthread_t) * cnt);
	
	for (i = 0; i < cnt - 1; i++) {
		err = pthread_create(&tid[i], NULL, do_download, NULL);
		if (err != 0) {
			printf("create thread error\n");
			exit(-1); 
		}
	}
	err = pthread_create(&tid[cnt - 1], NULL, check_download_status, NULL);
	if (err != 0) {
		printf("create thread error\n");
		exit(-1);
	}
	for (i = 0; i < cnt; i++) {
		pthread_join(tid[i], NULL);
	}
	free(tid);
	exit(0);
}

void *do_download(void * arg)
{
	int i = 0, tmp;

	for (i = 0; ; i++) {
		pthread_mutex_lock(&qlock);
		if (process_bar > 100) {
			pthread_mutex_unlock(&qlock);
			pthread_cond_signal(&cond);
			return ((void*)0);
		}
		printf("downloading......%d%%\n", process_bar);
		tmp = process_bar++;
		pthread_mutex_unlock(&qlock);
		sleep(1);
	}
}

void *check_download_status(void* arg)
{
	pthread_mutex_lock(&qlock);
	printf("is download complete?\n");
	while (process_bar < 100) {
		printf("%d%%......\n", process_bar);
		pthread_cond_wait(&cond, &qlock);
		printf("is download complete?\n");
	}
	printf("download success!\n");
	pthread_mutex_unlock(&qlock);
	return ((void*)0);
}