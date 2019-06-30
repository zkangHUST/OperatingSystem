// fifo receiver
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
