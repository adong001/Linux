#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<ctype.h>
#include<sys/wait.h>

void GetData(char**argv)//从标准输入获取命令
{
    char cmd_buf[1024]={0};//输入缓冲区
    char *ptr = cmd_buf;
    int i, argc = 0;
  
    printf("[username@locathost]$ ");
    fflush(stdout);
    
    fgets(cmd_buf,1023,stdin);//从标准输入获取数据
    cmd_buf[strlen(cmd_buf)-1] = '\0';
    printf("cmd_buf:[%s]\n", cmd_buf);
   
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
            if (*ptr == '\0')
            {
                break;
            }
            *ptr = '\0';
        }
        ptr++;
    }

    argv[argc] = NULL;//参数的最后一位以NULL为结束标志
    
    for(i=0;i<argc;i++)
    {
        printf("argv[%d]=[%s]\n",i,argv[i]);
    }
}

int main()
{

    while(1)
    {
        char* argv[32]={NULL};
        GetData(argv);//拿到标准输入字符串，并解析
        int i;
        for(i=0;argv[i] != NULL;i++)
        {
            printf("child argv[%d]=[%s]\n",i,argv[i]);
        }

        pid_t pid = fork();
        
        if(pid < 0)//创建失败继续创建
        {
            perror("fork error");
            continue;
        }

        else if(pid == 0)//子进程程序替换
        {
            for(i=0;argv[i] != NULL;i++)
            {
                printf("child argv[%d]=[%s]\n",i,argv[i]);
            }
            argv[i] =NULL;
            int ret = execvp(argv[0],argv);
            if(ret > 0)
            {
            printf("程序替换成功！\n");
            }
            else
            {
                printf("程序替换失败！\n");
            }
            exit(0);
            //子进程和父进程代码一致，若程序替换失败，
            //子进程就变成了新的shell,一个终端不需要有多个shell
            //因此替换失败，直接退出。
        }

        else
        {
            printf("父进程等待！！！\n");
            wait(NULL);//父进程等待,直至minishell退出
        }
    }
    return 0;
}

