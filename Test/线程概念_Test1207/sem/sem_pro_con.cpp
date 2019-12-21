#include<cstdio>
#include<cstdlib>
#include<iostream>
#include<queue>
#include<vector>
#include<pthread.h>
#include<semaphore.h>
using namespace std;

#define MAX_QUEUE 5 //队列默认空间大小

class RingQueue
{
    private:
        vector<int> m_array;//数组
        int m_size;//队列大小
        int m_front;//队尾,消费一个物品，m_front--
        int m_back; //队头,消费一个物品，m_back++
        sem_t m_sem_pro;//空闲空间计数器,有多少个空闲空间,就可以生产多少个物品
        sem_t m_sem_con;//数据资源计数器,有多少个数据资源,就可以消费多少个物品
        sem_t m_sem_lock;//安全锁
    
    public:
        RingQueue(int max_queue = MAX_QUEUE):
            m_array(max_queue), 
            m_size(max_queue),
            m_front(0),
            m_back(0)
    {
        sem_init(&m_sem_pro,0,0);//数据资源计数器
        sem_init(&m_sem_con,0,max_queue);//空闲空间计数器
        sem_init(&m_sem_lock,0,0);//锁的初始化
    }

        bool Push(int& data)
        {

            //有没有空闲空间，没有直接阻塞,并且空闲空间-1
            
            sem_wait(&m_sem_con);//判断是否有空闲空间
            sem_wait(&m_sem_lock);//加锁，保护入队操作

            m_array[m_back] = data;//生产者生产一个物品
            m_back = (m_back + 1) % m_size;//队头坐标(循环后移)

            sem_post(&m_sem_lock);//解锁
            sem_post(&m_sem_pro);//数据资源计数+1，唤醒消费者
            return true;
        }

        bool Pop(int& data)
        {
            //有没有资源数据，没有直接阻塞,空闲空间-1
            sem_wait(&m_sem_pro);
            sem_wait(&m_sem_lock);

            data = m_array[m_front];//消费者拿走一个物品
            m_front = (m_front + 1) % m_size;//队尾坐标(循环后移)

            sem_post(&m_sem_lock);//解锁
            sem_post(&m_sem_pro);//空闲空间计数器+1

            return true;
        }

        ~RingQueuenclude <semaphore.h>#include <semaphore.h>#include <semaphore.h>#include <semaphore.h>#include <semaphore.h>#include <semaphore.h>#include <semaphore.h>#include <semaphore.h>uuu()
        {
            sem_destroy(&m_sem_pro);
            sem_destroy(&m_sem_con);
            sem_destroy(&m_sem_lock);
        }



};

void *productor(void* arg)
{
    RingQueue* q = (RingQueue*) arg;
    int i = 0;

    while(1)
    {
        q->Push()
    }

}

#define MAX_THR 4

int main()
{
    int ret;
    pthread_t ptid[MAX_THR],ctid[MAX_THR];
    
        

    for(int i = 0;i <MAX_THR;i++)
    {

    }

    return 0;    
}

//posix标准
//sem_t sem;

//int sem_init(sem_t* sem,int pshare,int value);//初始化
//pshare---决定当前信号量是进程还是线程， 0---进程 ，!0---线程
//value--实际的资源数量，初始换信号量计数器初值
//int sen_wait(sem_t );


//
//int sem_destory(sem_t* sem);销毁操作


