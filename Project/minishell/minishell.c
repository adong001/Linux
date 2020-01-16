#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<ctype.h>
#include<fcntl.h>
#include<sys/wait.h>

#define CLEAR_REDIRECT 1 //清空重定向
#define APPEND_REDIRECT 2 //追加重定向

char g_buf[1024]={ 0 }; //输入缓冲区
int g_redirect_flag = 0;//解析重定向标志
char* g_filename = NULL;//重定向文件名

void AnalyseFilename();//解析重定向
void AnalyseOrder(char** argv);//解析命令
void GetData(char** argv);//从标准输入获取要解析的命令

void AnalyseFilename()//解析输入字符串,若为重定向命令,获得重定向文件名
{
    char* ptr = g_buf;
    while(*ptr)
    {
        if(*ptr == '>')
        {
            //若找到重定向标志'>'将buf中的这个字符置为结束标志,下面命令解析时就不在解析
            g_redirect_flag++;
            *ptr = '\0';
            ptr++;

            if(*ptr == '>')
            {
                g_redirect_flag++;
                ptr++;
            }

            for(;isspace(*ptr) && *ptr != '\0'; ptr++);//找到文件名开始,出掉中间的不可见字符
            g_filename = ptr;

            for(;!isspace(*ptr) && *ptr != '\0'; ptr++);//文件名结束标志置为'\0'
            *ptr = '\0';
        }
        ptr++;
    }
}

void AnalyseOrder(char**argv)//解析命令
{
    char *ptr = g_buf;
    int i, argc = 0;

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
            if (*ptr == '\0')//解析结束
            {
                break;
            }

            *ptr = '\0';
        }
        ptr++;
    }
}

void GetData(char** argv)
{
    g_redirect_flag = 0;//每次进入解析，先将所有的全局变量初始化,避免解析上次留下数据
    g_filename = NULL;
    memset(g_buf, 0x00, 1024);

    printf("[username@locathost]$ ");
    fflush(stdout);
    
    fgets(g_buf,1023,stdin);//从标准输入获取数据
    g_buf[strlen(g_buf)-1] = '\0';
    
    AnalyseFilename();
    AnalyseOrder(argv);
}

int main()
{
    while(1)
    {
        char* argv[32]={NULL};
        int i;
        GetData(argv);

        pid_t pid = fork();

        if(pid < 0)//创建失败继续创建
        {
            continue;
        }
        else if(pid == 0)//子进程程序替换
        {
            if(g_filename != NULL)
            {
                int fd = -1;
                if(g_redirect_flag == CLEAR_REDIRECT)//清空重定向
                {
                    fd = open(g_filename,O_WRONLY|O_CREAT|O_TRUNC,0664);
                    if(fd < 0)
                    {
                        perror("open error:");
                        return -1;
                    }
                    dup2(fd,1);
                }

                else if(g_redirect_flag == APPEND_REDIRECT)//追加重定向 
                {
                    fd = open(g_filename,O_WRONLY|O_CREAT|O_APPEND,0664);
                    if(fd < 0)
                    {
                        perror("open error:");
                        return -1;
                    }
                    dup2(fd,1);
                }
            }
            execvp(argv[0],argv);
            exit(0);
            //子进程和父进程代码一致，若程序替换失败，
            //子进程就变成了新的shell,一个终端不需要有多个shell
            //因此替换失败，直接退出。
        }
        wait(NULL);//父进程等待,直至minishell退出
    }
    return 0;
}

