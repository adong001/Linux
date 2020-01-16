#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>                                                                                                                                                       

int main()
{
    char buf[1024] = {0};
    int pipefd[2] = {0};
    int ret = pipe(pipefd);//在创建子进程之前创建管道,子进程才会复制到创建的管道
    if(ret < 0)
    {
        perror("pipe error:");
        return -1;

    }

    pid_t pid = fork();

    if(pid < 0)
    {
        perror("fork error:");
        return -1;

    }

    else if(pid == 0)//子进程
    {
        int fd = read(pipefd[0],buf,1023);//若管道中没有数据,则read就阻塞
        if(fd < 0)
        {                                                                                                                                       
            perror("read error:");                                                                                                              
            return -1;                                                                                                                          
        }                                                                                                                                       
        printf("子进程读取成功~~\n");    
        printf("buf[%s]-[%d]\n",buf,fd);    

    }                                 

    else//父进程    
    {                                    
        char* ptr = "这是一个测试匿名管道的demo~~";    
        if(write(pipefd[1],ptr,strlen(ptr)) < 0)//若管道写满,则write就阻塞    
        {    
            perror("write error:");                                                                                                                                       
            return -1;                                                        
        }    
        printf("父进程写入成功~~~\n");    
    }                                                                        

    while(1)                             
    {                                                                        
        printf("---------------%d\n",getpid());                                                                                                          
        sleep(1);                                                                                                                                        
    }                                                                                                                                                    
    return 0;                                                                                                                                            

} 
