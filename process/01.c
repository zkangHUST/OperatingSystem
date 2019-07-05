#include<stdlib.h>
#include<stdio.h>
// 环境变量表
extern char **environ;
int main(int argc, char* argv[])
{
    int i = 0;
    while(environ[i] != NULL) {
        printf("%s\n", environ[i]);
        i++;
    }
    return 0;
}