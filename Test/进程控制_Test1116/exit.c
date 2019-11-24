#include<stdio.h>
#include<errno.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>

int main()
{
    printf("nihao");//没有\n写入缓冲区中(exit和return都会刷新缓冲区)
    //exit(0);
    _exit(0);
    return 0;
}
