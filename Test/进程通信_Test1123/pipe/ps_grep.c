#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int main()
{
    int pipefd[2];
    int ret = pipe(pipefd);//创建子进程之前创建管道
    if (ret < 0) 
    {
        perror("pipe error");
        return -1;
    }

    int pid1 = fork();//ps进程
  
    if(pid1 <  0)
    {
        perror("fork1 error:");
        return -1;
    }

    else if (pid1 == 0) 
    {
        dup2(pipefd[1], 1);//将ps -ef命令打印在标准输出的内容重定向到管道的写入端
        execl("/usr/bin/ps", "ps", "-ef", NULL);//替换程序ps -ef
        exit(0);
    }

    int pid2 = fork();//grep进程
    if (pid2 == 0)
    {
        close(pipefd[1]);//进入grep进程时,grep要从管道读取端读取数据,先要关闭管道的写入端,read才不会阻塞
        dup2(pipefd[0], 0);//grep要从标准输入读取数据,这是就得将标准输入重定向到管道的读取端
        execl("/usr/bin/grep", "grep", "sshd", NULL);//替换程序grep sssh
        exit(0);

    }

    //父进程
    close(pipefd[0]);//父进程既不读也不写,将写端和读端关闭。
    close(pipefd[1]);
    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);
    return 0;

}

