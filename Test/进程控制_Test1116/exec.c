#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

int main(int argc,char* argv[],char* env[])
{

   char* envg[] = {"MYVAL=1000","PAT","NULL"};
   //execl("/usr/bin/ls","ls","-l",NULL);
   //execlp("ls","ls","-l",NULL);
    execle("./test","./test","-a","-l",NULL,envg);
    printf("hello\n");
    return 0;
}
