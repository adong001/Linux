#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/shm.h>
#include<sys/ipc.h>

#define IPC_KEY 0x123456789 //共享内存key值

int main()
{
    int shmid = shmget(IPC_KEY,32,IPC_CREAT|0664);//write_shm.c中已创建共享内存,判断存在就打开,shmid为这块内存操作句柄
    if(shmid < 0)
    {
        perror("shmget error:");
        return -1;
    }

    void* shmstart = shmat(shmid,NULL,0);//建立映射关系,shmstart为映射的虚拟地址空间首地址
    if(shmstart == (void*)-1)
    {
        perror("shmat error:");
        return -1;
    }

    int i = 0;
    while(1)
    {
        printf("%s",shmstart);//将共享内存中的数据打印到屏幕上
        sleep(1);
    }
    shmdt(shmstart);//解除映射
    shmctl(shmid,IPC_RMID,NULL);//IPC_RMID--删除共享内存

    return 0;
}
