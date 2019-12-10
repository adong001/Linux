#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<signal.h>
#include<sys/wait.h>

void sigcb(int signum)
{
    while(waitpid(-1,NULL,WNOHANG)> 0)
    {
            printf("子程序退出\n");
    }
        //返回值为子进程的pid，没有子进程退出，返回0，出错返回-1
}
int main()
{
    signal(SIGCHLD,sigcb);
    pid_t pid = fork();
    if(pid == 0)
    {
        sleep(5);
        exit(0);
    }
    
    if(fork() == 0)//再创建一个子进程，不退出
    {
        while(1)
        {
            printf("this is child\n");
            sleep(1);
        }
    }

    while(1)
    {
    
        sleep(1);
        printf("打麻将~~~\n");
    }
    return 0;
}
