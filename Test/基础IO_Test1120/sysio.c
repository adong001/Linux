#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/stat.h>
#include<fcntl.h>

int main()
{
    char* ptr = "这是一个测试系统调用接口的demo~~~\n";
    char buf[1024];
    int ret;
    umask(0);//设置当前调用进程的文件创建权限掩码
    //mode_t umask(mode_t mask);
    //文件权限 = mode & ~mask   ---mode就是open的最后一个参数
    int fd = open("./tmp2.txt",O_RDWR | O_CREAT,0777);
    if(fd < 0)
    {
        perror("open error:");
        return -1;
    }

    ret = write(fd,ptr,strlen(ptr));
    if(ret < 0)
    {
        perror("write error:");
        return -1;
    }
    else
    {
        printf("数据写入成功~~\n");
    }

    lseek(fd,0,SEEK_SET);//改变文件操作位置到文件起始

    ret = read(fd,buf,strlen(buf));
    if(ret < 0)
    {
        perror("read error:");
        return -1;
    }
    else if(ret == 0)
    {
        printf("at end of file\n");
    }

    else
    {
        printf("数据读取成功~~\n");
        printf("buf[%d] = [%s]",ret,buf);
    }

    close(fd);//关闭文件,释放资源
    return 0;
}

