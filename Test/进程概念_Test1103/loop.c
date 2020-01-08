#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<stdint.h>

int main()
{
    printf("******************\n");
    pid_t pid = fork();
    uint64_t i = 0;
    while(1)
    {
        printf("---------------\n");
        sleep(1);
    }
    return 0;
}
