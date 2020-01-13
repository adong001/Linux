#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<ctype.h>
#include<sys/wait.h>


void GetData(char** argv)//从标准输入获取命令
{
    char cmd_buf[1024]={0};//输入缓冲区
    int argc = 0;//记录参数的个数
    char *ptr = cmd_buf;

    printf("[username@locathost]$ ");
    fflush(stdout);

    fgets(cmd_buf,1023,stdin);//从标准输入获取数据
    cmd_buf[strlen(cmd_buf)-1] = '\0';

    while(*ptr)//按照空格对字符串进行切割
    {
        if(!isspace(*ptr))
        {
            argv[argc] = ptr;
            argc++;

            while(!isspace(*ptr) && *ptr != '\0')//到下一个空格且不是字符串结尾位置
            {
                ptr++;
            }
            *ptr = '\0';
        }
        ptr++;
    }

    int i;
    argv[argc] = NULL;
    for(i=0;i<argc;i++)
    {
        printf("argv[%d]=[%s]\n",i,argv[i]);
    }
}


int main()
{

    pid_t pid = fork();
    char* argv[32] = {0};
    while(1)
    {
        GetData(argv);
        printf("pid = %d\n",pid);
        if(pid < 0)//创建失败继续创建
        {
            continue;
        }

        else if(pid == 0)//子进程程序替换
        {
            execvp(argv[0],argv);
            exit(0);//子进程和父进程代码一致，若程序替换失败，子进程就变成了新的shell,一个终端不需要有多个shell
                    //因此替换失败，直接退出。
        }

        else
        {
            wait(NULL);
        }

    }
    return 0;
}

