#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<signal.h>

void sigcb(int signum)
{
    printf("signum = %d\n",signum);
}

int main()
{
    signal(2,sigcb);
    signal(40,sigcb);

    sigset_t set,old;
    sigemptyset(&set);//清空信号集合
    sigemptyset(&old);

    
    //sigaddset(int signum,sigset_t *set);将指定自信号添加到集合
    sigfillset(&set);//将所有的信号添加到set集合中来

    sigprocmask(SIG_BLOCK,&set,&old);//阻塞所有信号

    printf("press enter to continue\n");
    getchar();//按下回车之前，程序卡在这里

    sigprocmask(SIG_UNBLOCK,&set,NULL);//解除阻塞
    return 0;
}
