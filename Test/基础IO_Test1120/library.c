#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>

int main()
{

    FILE* fp = fopen("./tmp.txt","w+");//以可读可写方式打开

    if(!fp)
    {
        perror("fopen error:");
    }

    else
    {
        char buf[1024] = {0};
        char* ptr = "这是一个测试标准库函数的demo~~\n";    
        int ret ;

        ret = fwrite(ptr,1,strlen(ptr),fp);//向文件写入数据

        if(ret < 0)
        {
            perror("fwrite error:");
            return -1;
        }

        else
        {
            printf("文件写入数据成功！\n");
        }

        fseek(fp,0,SEEK_SET);//写完文件后，文件指跳转到了文件末尾,
        //需要将其移动到文件开头,再进行读操作
        ret = fread(buf,1,strlen(ptr),fp);

        if(ret < 0)//读文件出错
        {
            perror("fread error:");
            return -1;
        }

        else
        {
            printf("从文件读取成功！\nbuf[%d] =[%s]\n",strlen(buf),buf);
        }
        fclose(fp);//关闭文件，释放资源
    }
    return 0;
}
