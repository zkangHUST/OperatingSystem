# 进程通信

## 1. 命名管道

创建命名管道可以通过指令和函数两种方式

```C
// 1.通过指令创建
mkfifo vincent.fifo

// 2.调用函数创建
#include<sys/types.h>
#include<sys/stat.h>
int mkfifo(const char *pathname, mode_t mode);
mkfifo("vincnet.fifo", 0664);
```

尽管FIFO在文件系统中有名字，但是他的持续性和管道一样是随进程的，最后一个进程关闭FIFO后，FIFO中的数据都会被丢弃。FIFO是一个单向(半双工)数据流。

在下面的例程中，我们先用`mkfifo vincent.fifo`指令创建一个FIFO，然后在发送程序中已只写的方式打开该FIFO，向该FIFO写入数据。在接收程序中，我们从FIFO中读取数据，并打印到标准输出。

```C
// fifo_send.c
#include<unistd.h>
#include<fcntl.h>
#include<stdio.h>
#define MAX_BUF_LEN 64

int main()
{
    int len, fd;
    char buf[MAX_BUF_LEN];
    
    fd = open("vincent.fifo", O_WRONLY);
    if (fd < 0) {
        perror("open fifo");
        return -1;
    }
    while ((len = read(STDIN_FILENO, buf, MAX_BUF_LEN)) > 0) {
        write(fd, buf, len);
    }
    close(fd);
    return 0;
}
```

```C
// fifo_recv.c
#include<unistd.h>
#include<fcntl.h>
#include<stdio.h>
#define MAX_BUF_LEN 64

int main()
{
    int len, fd;
    char buf[MAX_BUF_LEN];
    
    fd = open("vincent.fifo", O_RDONLY);
    if (fd < 0) {
        perror("open fifo");
        return -1;
    }
    while ((len = read(fd, buf, MAX_BUF_LEN)) > 0) {
        write(STDIN_FILENO, buf, len);
    }
    close(fd);
    return 0;
}
```