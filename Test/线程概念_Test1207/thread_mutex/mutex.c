#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>

int tickets = 100;//共有100张票

void* thread_grabbing(void* mutex)
{
    while(1)
    {
        pthread_mutex_lock((pthread_mutex_t*) mutex);//阻塞加锁,
        //pthread_mutex_trylock((pthread_mutex_t*)mutex);//非阻塞加锁
        //pthread_mutex_timelock((pthread_mutex_t*)mutex);限时等待阻塞加锁
        if(tickets > 0)
        {
        // usleep(1000);
            printf("scalper:%p- get a tickets:%d\n",pthread_self(),tickets);//抢到一张票
            tickets--;
            pthread_mutex_unlock((pthread_mutex_t*) mutex);//抢完一张票解锁,下次和其他黄牛再次竞争抢票
            usleep(1);
        }

        else//票没了就退出
        {
            printf("scalper:%p exit\n",pthread_self());
            pthread_mutex_unlock((pthread_mutex_t*) mutex);//在任何线程可能退出的地方都要解锁
            pthread_exit(NULL);
        }
    }
    return NULL;    
}

int main()
{
    int i,ret;
    pthread_t scalper[4];//四个线程代表四个黄牛
    pthread_mutex_t mutex;//定义互斥锁
    pthread_mutex_init(&mutex,NULL);
    for(i = 0;i < 4;i++)
    {
        ret = pthread_create(&(scalper[i]),NULL,thread_grabbing,&mutex);//创建四个黄牛线程
        if(ret != 0)
        {
            printf("thread create error\n");
            return -1;
        }
    }

    for(i = 0;i < 4;i++)
    {
         pthread_join(scalper[i],NULL);//等待线程退出,释放资源;
    }
    pthread_mutex_destroy(&mutex);//销毁互斥锁
    return 0;
}
