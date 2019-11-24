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
        exit(0);
    }

   // wait(NULL);//等待子进程退出后再继续
   //int ret = waitpid(-1,NULL,0);//等待任意子进程退出
   //int ret = waitpid(pid,NULL,0);//等待指定(pid)子进程退出
   int ret = waitpid(-1,NULL,WNOHANG);//没有进程退出直接返回,返回值为-1(出错)0(没有子进程退出)>1(返回退出子进程的pid)
   if(ret < 0)
   {
       perror("wait error");
       exit(-1);
   }

   else if(ret == 0)
   {
       printf("have no child exit\n");
   }

   else
   {
       printf("pid = %d\n",ret);
   }

    while(1)
    {
        printf("打麻将\n");
        sleep(1);
    }       

    return 0;
}
