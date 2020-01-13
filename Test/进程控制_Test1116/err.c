#include<stdio.h>
#include<errno.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>

int main()
{
    int i = 0;
    for(;i<256;i++)
    {
        printf("err:[%d]=[%s]\n",i,strerror(i));
    }
    return 0;
}

