#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>

int main()
{
    FILE* fp = fopen("./test.txt","w+");
    if(fp == NULL)
    {
        perror("fopen fail");
        return -1;
    }

    char ptr[] = "你好啊啊啊啊\n";
    int ret = fwrite(ptr,1,strlen(ptr),fp);
    if(ret <= 0)
    {
        perror("write fail");
        return -1;
    }
    printf("write size = %d\n",ret);

    fseek(fp,0,SEEK_SET);//跳转到文件起始位置(set)往后偏移0，cur(当前位置)，end(末尾)

    char buf[1024] = {0};
    ret = fread(buf,1,19,fp);
    if(ret <= 0)
    {
        perror("read fail");
        return -1;
    }
    printf("read buf[%s]-[%d]\n",buf,ret);

    fclose(fp);
    return 0;
}
