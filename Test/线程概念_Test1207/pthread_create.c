#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

void *thread_start(void *arg)
{
    while(1)
    {
        //int pthread_detach(pthread_t tid);
        //---分离指定线程，等待一个被分离的进程，pthread_join会返回错误:”这不是一个joinable线程，
        //因为在获取返回值的时候获取不到，detach属性的线程退出后已经自动释放了资源。
        pthread_detach(pthread_self());   //方法2；在线程入口函数中将自己分离，线程属性设置为detach
        char* buf = "hello world\n";
        sleep(3);
        pthread_exit(buf);
        //buf作为线程推迟的返回值,不是pthread_exit的返回值,它没有返回值,buf使用常量是因为这个栈退出,
        //里面的数据也被释放,而常量放在正文段,不会随着栈的销毁而销毁
        while(1)
        {
            printf("主线程传递了一个参数:%s\n", (char*)arg);
            sleep(1);

        }
        return NULL;

    }
}

int main()
{
    pthread_t tid;
    char buf[] = "你好呀~\n";
    int ret = pthread_create(&tid, NULL, thread_start,(void*)buf);
    if  (ret != 0)
    {
        printf("thread create error:%d\n", ret);
        return -1;
    }
    

    //pthead_exit(void* retval); //----终止一个线程，无返回值
    //pthrea_cancel(pthread_t tid);//---取消一个指定tid的线程,成功返回0，失败返回errno
    //int pthrea_join(pthread_t tid,void** pthread); ---等待指定tid的线程，pthread指向这个线程的地址空间。成功返回0，失败返回errno
    //int pthread_detach(pthread_t tid);---将一个指定tid的线程分离，使其在结束后自动释放资源。一个线程的默认属性为joinable,线程退出
    //要对其进行pthread_join(等待)操作,从而释放其资源。但是将这个线程的属性设为detach，就不用等待了，detach和joinable是冲突的。
    //方法1：在创建线程之后分离自己，将自己设为detach属性
    //pthread_detach(tid);

    void* retval = NULL;//获取指向线程的地址空间的地址，回收这个线程占用的资源
    pthread_join(tid,&retval);
    printf("retval is %s\n",retval);

    while(1) 
    {
        printf("i am main thread,child tid:%p\n", tid);
        sleep(1);

    }

    return 0;
}

