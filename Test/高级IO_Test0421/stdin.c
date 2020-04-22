#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/select.h>

int main()
{
    //对标准输入进行监控
    fd_set rfds;
    FD_ZERO(&rfds);

    while(1)
    {
        struct timeval tv;
        tv.tv_sec = 3;//阻塞等待时间设置为3秒
        tv.tv_usec = 0;

        printf("将标准输入添加到监控集合中\n");
        FD_SET(0,&rfds);

        printf("开始监控.......\n");
        
        int nfds = select(1, &rfds,NULL, NULL,&tv);//标准输入描述符为0,,所以最大描述符为0+1
        if(nfds < 0)
        {
            printf("监控出错\n");
            return -1;
        }  
        
        else if(nfds == 0)
        {
            printf("监控超时...\n");
            continue;
        }

        printf("标准输入事件就绪...正在进行读操作...\n");
        char buf[1024] = {0};
        int ret = read(0,buf,1023);

        if(ret <= 0)
        {
            perror("read erro:\n");
            return -1;
        }

        printf("read data from stdin: %s\n",buf);
    }
    return 0;
}
