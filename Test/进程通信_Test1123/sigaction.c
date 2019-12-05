#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<signal.h>

struct sigaction oldact;//``
void sigcb(int signum)
{
    printf("signum = %d\n",signum);
    sigaction(signum,&oldact,NULL);
}

int main()
{
    struct sigaction newact;
    newact.sa_handler = sigcb;
    newact.sa_flags = 0;
    sigemptyset(&newact.sa_mask);

    //sigaction修改信号的处理动作为newact,原来的动作oldact保存
    sigaction(SIGINT,&newact,&oldact);
    while(1)
    {
        printf("hello\n");
        sleep(1);
    }
    return 0;
}
