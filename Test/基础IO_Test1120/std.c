#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<unistd.h>

int main()
{
    int fd = open("./test.txt",O_RDWR |O_CREAT,0777);
    if(fd < 0)
    {
        perror("open error:");
        return -1;
    }

    printf("fd = %d\n",fd);
    return 0;
}

