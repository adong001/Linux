#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>

void* start_routine(void* arg)//线程入口函数
{
    char* retval = "这个线程退出了~~\n";
   //pthread_detach(pthread_self());//分离线程方式2,在线程入口函数出分离  
    printf("recive main thread :%s\n",(char*)arg);
    
    sleep(3);
    pthread_exit(retval);//线程退出方式2;
  //pthread_cancel(pthread_self());//线程退出方式3
    
    return NULL;//线程退出方式1,
}

int main()
{
    pthread_t tid;//pthread_t == unsigned long int
    char buf[] = "这是一种个测试线程创建的demo~~\n";
    void *retval = NULL;

    int ret = pthread_create(&tid,NULL,start_routine,(void*) buf);//创建线程
    //pthread_detach(tid);//分离线程方式1,创建完线程后分离

    if(ret != 0)//创建失败返回一个非0值
    {
        printf("thread create error:%d\n",ret);
        return -1;
    }
    
    while(1)
    {
      if(!pthread_join(tid,&retval))//线程等待
      {
        printf("等待线程退出~~\n");
      }

     else
      {
          printf("线程已退出,retval = %s\n",retval);
          break;
      }

    }
    while(1)//主线程
    {
        printf("this is main thread,child thread tid=%p\n",tid);
        sleep(1);
    }

    return 0;
}
