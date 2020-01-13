#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/wait.h>

int main()
{
    pid_t pid = fork();

    if(pid < 0)
    {
        perror("fork error");
        exit(-1);
    }

    else if(pid == 0)
    {
        sleep(5);//子进程退出
        exit(0);
    }

    int retval = 0;//接收退出返回值
    int Pid = wait(&retval);//等待子进程退出后再继续
    printf("Pid = %d\n  retval = %d\n",Pid,retval);
  
    while(1)
    {
        printf("打麻将\n");
        sleep(1);
    }       

    return 0;
}
