#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>

int main()
{
    pid_t pid = fork();
    if(pid < 0)
    {
        perror("fork error\n");
    }

    else if(pid == 0)
    {
        printf("子进程[%d]\n",getpid());
    }

    else if(pid > 0)
    {
        printf("父进程[%d]\n",getpid());
        sleep(5);
        exit(0);//父进程先于子进程退出,子进程变成孤儿进程
    }
          

    while(1)
    {        
        printf("进程号[%d]\n",getpid());
        sleep(1);
    }

    return 0;
}
