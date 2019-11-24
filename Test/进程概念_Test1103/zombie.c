#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>

int main()
{
    printf("**************%d\n",getpid());
    pid_t pid = fork();
    if(pid < 0)
    {
        perror("fork error\n");
        return -1;
    }
    else if(pid == 0)
    {
        //这里是子进程运行，fork返回值为0
        printf("子进程pid = %d\n",getpid());
        sleep(5);
        exit(0);
    }

    else if(pid > 0)
    {

        //这里是父进程运行，fork返回值为子进程的PID
        printf("父进程pid = %d\n",getpid());
    }
    //以下代码父子进程都可以运行得到
    while(1)
    {   
        printf("-------------%d\n",getpid());
        sleep(1);
    }
    return 0;
}
