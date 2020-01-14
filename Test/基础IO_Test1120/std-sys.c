#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>
#include<stdlib.h>

int main()
{
    printf("printf--");
    fwrite("fwrite--",8,1,stdout);
    fprintf(stdout,"%s","fprintf--");
    write(1,"write--",7);
    sleep(3);
    return 0;
}
