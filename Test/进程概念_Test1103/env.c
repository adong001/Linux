#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>

int main(int argc, char* argv[],char* env[])
{
    char* ptr = getenv("MYVAL");
    if(ptr == NULL)
    {
        printf("NO MYVAL\n");
    }

    else
    {
        printf("%s\n",ptr);
    }
    int i;

    extern char** environ;
    for(i = 0;environ[i] != NULL;i++)
    {
        printf("environ[%d]=[%s]\n",i,environ[i]);
    }
    /*
    char* ptr = getenv("MYENV");
    if(ptr == NULL)
    {
        printf("no have MYENV\n");
    }

    else 
    {
        printf("%s\n",ptr);
    }*/
    return 0;
}
