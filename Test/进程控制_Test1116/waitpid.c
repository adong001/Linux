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
        sleep(5);
        exit(99);
    }

    int status  = -1;
    while(waitpid(pid,&status,WNOHANG)==0)//没有进程退出直接返回,返回值为-1(出错)0(没有子进程退出)>1(返回退出子进程的pid)
    {
        printf("等一等~~~\n");
        sleep(1);
    }
   if(!(status & 0x7f))
   {
    printf("retval = %d\n",(status>>8)&0xff);
   }
   else
   {
       printf("process exit eception\n");
   }
    while(1)
    {
        printf("打麻将\n");
        sleep(1);
    }       

    return 0;
}

