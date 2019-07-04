// b.c
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <string.h>

int main() {
  // 根据事先约定后的键值获取内核对象 id，这时候后面两个参数都可以为 0.
  int id = shmget(0x8888, 0, 0); 
  if (id < 0) {
    perror("shmget");
    return -1; 
  }

  printf("id = %d\n", id);

  // 挂接内存
  char *buf = shmat(id, NULL, 0); 

  if (buf == (char*)-1) {
    perror("shmat");
    return -1; 
  }

  // 打印数据
  printf("%s", buf);

  // 卸载
  if (shmdt(buf) < 0) {
    perror("shmdt");
    return -1; 
  }

  // 删除内核对象
  if (shmctl(id, IPC_RMID, NULL) < 0) {
    perror("shmctl");
    return -1; 
  }

  return 0;
}