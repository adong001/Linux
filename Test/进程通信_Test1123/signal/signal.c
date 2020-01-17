#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<signal.h>

void sigcb(int signum)
{
    printf("signum = %d\n",signum);
}

int main()
{
    //sighandle_t signal(int signum,sighandler_t handler);
  //  signal(SIGINT,sigcb);
    signal(SIGINT,SIG_IGN);//忽略处理

    while(1)
    {
        printf("hello\n");
        sleep(1);
    }
    return 0;
}
