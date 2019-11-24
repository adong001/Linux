#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

int main(int argc,char* argv[])
{
    char *env[3];
    env[0]="MYENV = 1000";
    env[1]=NULL;
    int i;
    for(i = 0;i < argc;i++)
    {
        printf("argv[%d]:[%s]\n",i,argv[i]);
    }
    //execl("/usr/bin/ls","ls","-l",NULL);
    //execlp("myenv","myenv","-l",NULL);
    execle("./myenv","myenv","-l",NULL,env);
    printf("hello\n");
    return 0;
}
