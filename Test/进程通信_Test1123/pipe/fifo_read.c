#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<errno.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<string.h>
int main()
{
    umask(0);
    char* file = "./test.fifo";
    int ret = mkfifo(file,0664);

    if(ret < 0 && errno != EEXIST) 
    {
        perror("mkfifo error:");
        return -1;
    }

    printf("creat fifo success\n");

    int fd = open(file,O_RDONLY);
    if(fd < 0)
    {
        perror("open error:");
        return -1;
    }

    printf("open success\n");

    while(1)
    {
        char buf[1024] = {0};
        ret = read(fd,buf,1023);
        if(ret < 0)
        {
            perror("read error:");
            return -1;
        }

        else if(ret == 0)//read函数没有读到数据,返回0,说明写端被关闭
        {
            printf("all write close\n");
            return -1;
        }
        printf("读取数据buf[%s]\n",buf);
    }
    close(fd);

    return 0;
}
