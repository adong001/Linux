#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<pthread.h>

//模拟多个黄牛抢票
int tickets = 100;//100张票
pthread_mutex_t mutex;

//pthread_mutex_t ---互斥锁变量
//int pthread_mutex_init(pthread_mutex_t* mutex,pthread_mutexattr_t* attr);---初始化互斥锁
//int pthread_mutex_lock(pthread_mutex_t* mutex);---在临界资源访问之前加锁，阻塞加锁(若临界资源已经加锁，则阻塞，直至临界资源可以加锁为止)
//int pthread_mutex_trylock(pthread_mutex_t* mutex);---非阻塞加锁(若临界资源已经加锁，直接退出)
//int pthread_mutex_timelock(pthread_mutex_t* mutex);---限时阻塞加锁(若临界资源的已经加锁，则阻塞某个时间段，还不能加锁就退出)
//int pthread_mutex_unlock(pthread_mutex_t* mutex);---在访问临界资源完毕后解锁
//int pthread_mutex_destory(pthread_mutex_t* mutex);---销毁互斥锁
void* tickets_tout(void* arg)
{
    while(1)
    {
        pthread_mutex_lock(&mutex);
        if(tickets > 0)
        {
            usleep(10);
            printf("黄牛:%p抢到%d号票\n",pthread_self(),tickets);
            tickets--;
            pthread_mutex_unlock(&mutex);//(解锁一)在某个线程访问完临界资源后解锁
        }

        else
        {
            printf("黄牛%p exit\n",pthread_self());
            pthread_mutex_unlock(&mutex);//(解锁二)在某个线程退出后解锁
            pthread_exit(NULL);
        }
    }
    return NULL;
}


int main()
{
    int i =0,ret;
    pthread_t tid[4];//四个线程代表四个黄牛

    //初始换互斥锁一定要放在创建线程之前
    pthread_mutex_init(&mutex,NULL);

    for(;i < 4;i++)
    {
        ret = pthread_create(&tid[i],NULL,tickets_tout,NULL);//创建四个黄牛线程


        if(ret!=0)//创建失败退出
        {
            printf("thread create errno\n");
            return -1;
        }
    }

    for( i = 0;i < 4;i++ )//等待四个黄牛线程退出后,释放其资源。
    {
        pthread_join(tid[i],NULL);
    }

    //互斥锁的销毁，放在完全不使用互斥锁的地方
    pthread_mutex_destroy(&mutex);
    return 0;

}
