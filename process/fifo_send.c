// fifo sender
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
