#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<string.h>
#include<sys/stat.h>

int main()
{
    umask(0);
    int fd = open("./test2.txt",O_RDWR | O_CREAT,0777);
    //O_EXCEL(和O_CREAT同时存在),文件不存在创建，存在报错
    //O_TRUNC 打开文件时清空文件
    //O_APPEND 追加在末尾    w+ ==  O_RDWR | O_CREAT | O_TRUNC   a+ == O_RDWR | O_CREAT | O_APPEND
    if(fd < 0)
    {
        perror("open fail");
        return -1;
    }

    char* ptr = "天黑了~~~\n";
    int ret = write(fd,ptr,strlen(ptr));
    if(ret < 0)
    {
        perror("write fail");
        return -1;
    }
    printf("write size = %d\n",ret);


    lseek(fd,0,SEEK_SET);//成功：返回跳转后的位置到起始位置的偏移量(跳转到文件末尾，获取返回值就可以知道文件的长度)
                         //失败：返回-1

    char buf[1024]={0};
    ret = read(fd,buf,1023);
    if(ret < 0)
    {
        perror("read fail");
        return -1;
    }

    else if(ret == 0)
    {
        printf("at end of file\n");
    }
    printf("read buf:[%s] [%d]\n",buf,ret);

    close(fd);
    return 0;    
}
