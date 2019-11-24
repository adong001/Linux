#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>

int g_val = 10;
int main()
{
    pid_t pid = fork();
    if(pid < 0)
    {
        perror("fork fail\n");
    }
    else if(pid == 0)
    {
        g_val = 100;
        printf("child--g_val:%d-%p\n",g_val,&g_val);
    }

    else if(pid > 0)
    {
        sleep(2);
        printf("parent--g_val:%d-%p\n",g_val,&g_val);
    }
    return 0;
}
