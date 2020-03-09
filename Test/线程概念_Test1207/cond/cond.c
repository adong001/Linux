#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>

pthread_cond_t cond_customer;//顾客条件变量
pthread_cond_t cond_chief;//厨师条件变量
pthread_mutex_t mutex;//互斥锁

void* MakeFoods(void* foods)//做食物
{
    while(1)
    {
        pthread_mutex_lock(&mutex);//要做饭了,先加锁,防止顾客进来
        while((*(int*)foods) >= 5)
        {
            //若做好食物,但没人吃,则陷入等待
            pthread_cond_wait(&cond_chief,&mutex);
        }
        (*(int*)foods)++;//做了一碗饭,饭的数量++
        printf("make a bowl of foods:%d\n",*(int*)foods);
        pthread_mutex_unlock(&mutex);//做完饭解锁,
        pthread_cond_signal(&cond_customer);//做好食物唤醒等待的顾客
    }
    return NULL;
}

void* EatFoods(void* foods)//吃食物
{
    while(1)
    {
        pthread_mutex_lock(&mutex);
        while(*((int*)foods) == 0)//没有食物则customer等待,但是需要在等待之前先解锁
        {   
            //并且解锁操作和陷入休眠操作必须是原子操作
            pthread_cond_wait(&cond_customer,&mutex);
        }
        (*(int*)foods)--;
        printf("吃饭~~~,还有%d碗饭\n",*(int*)foods);
        pthread_mutex_unlock(&mutex);
        pthread_cond_signal(&cond_chief);//吃完一碗饭，就可以唤醒厨师再做一碗了
    }
    return NULL;
}

int main()
{
    int ret ,i;
    int foods = 0;//食物,1---有食物customer可以吃;0---没有食物,customer等待
    pthread_t thread_chief,thread_customer;//厨师线程和顾客线程

    pthread_cond_init(&cond_chief,NULL);//初始化条件变量
    pthread_cond_init(&cond_customer,NULL);
    pthread_mutex_init(&mutex,NULL);//初始化互斥锁

    for(i = 0;i < 4;i++)
    {
        ret = pthread_create(&thread_chief, NULL, MakeFoods,(void*)(&foods));//创建厨师线程
        if(ret != 0)
        {
            printf("chief thread create error\n");
            return -1;
        }
    }

    for(i = 0;i < 4;i++)
    {
        ret = pthread_create(&thread_customer,NULL, EatFoods,(void*)&foods);//创建顾客线程·
        if(ret != 0)
        {
            printf("customer thread create error\n");
            return -1;
        }
    }

    pthread_join(thread_chief,NULL);//等待两个线程退出释放其资源
    pthread_join(thread_customer,NULL);

    pthread_cond_destroy(&cond_chief);//销毁条件变量
    pthread_cond_destroy(&cond_customer);
    return 0;    
}
