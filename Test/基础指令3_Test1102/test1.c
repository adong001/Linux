#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

void Init(char* src,int size)
{
    int i;
    for(i = 0;i < size;i++)
    {
        src[i] = ' '; 
    }
}


int main()
{
    char tag[] = "/-\\|";
    char src[51] = "";
    Init(src,50);
    int i,j;
    for(i = 0;i <= 100;i++)
    {
        src[i/2] = '=';
        printf("[%s][%c][%-3d%%]\r",src,tag[i%3],i);
        fflush(stdout);
        usleep(100000);
    }
    sleep(2);
    return 0;
}
