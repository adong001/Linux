#include<pthread.h>
#include<iostream>
#include<ctime>
#include<unistd.h>
#include<cstdio>
#include<queue>
#include<cstdlib>
#define MAX_THREAD 5
using namespace std;

typedef void(*task_handler_t)(int);//定义了一个函数指针类型，这个类型叫task_handler_t

//template<class T>
class ThreadTask
{
//线程池中的线程只需要一个TreadTask任务，然后调用Run接口就能完成任务处理
    private:
        int m_data;
        task_handler_t m_handler;
    public:
        ThreadTask(int data,task_handler_t handler):
            m_data(data),
            m_handler(handler)
    {}
    ~ThreadTask()
     {}
    void Run()
    {
        return m_handler(m_data);
    }
};

//template<class T>
class ThreadPool
{
    private:
        int m_thread_count;//定义线程池中线程的数量
        std::queue<ThreadTask> m_task_queue;//任务队列,此处不做数量的最大限制(生产者不在阻塞)
        pthread_mutex_t m_mutex;//互斥保护task_queue的操作
        pthread_cond_t m_cond;//这个条件变量用于实现线程池中线程的阻塞等待
        
        //thread_start是一个类的成员函数,代用隐藏参数this(pthread_creat调用时会出错)
        //将其作为静态成员函数,就不会有this,再将this指针通过以通过传参传进来
       static void* thread_start(void* arg)//线程入口函数
        {
            ThreadPool* pool = (ThreadPool*) arg;
            //获取一个任务，然后调用任务对象的Run接口
            while(1)
            {
                //判断队列是否为空,则线程需要阻塞等待
                pthread_mutex_lock(&pool->m_mutex);
                while(pool->m_task_queue.empty())
                {
                    pthread_cond_wait(&pool->m_cond,&pool->m_mutex);
                }
                ThreadTask task = pool->m_task_queue.front();
                pool->m_task_queue.pop();
                pthread_mutex_unlock(&pool->m_mutex);
                task.Run();
            }
            return NULL;
        }

    public:
        ThreadPool(int max_thread = MAX_THREAD):
            m_thread_count(max_thread)
       {
            pthread_mutex_init(&m_mutex,NULL);
            pthread_cond_init(&m_cond,NULL);
            for(int i = 0;i < m_thread_count;i++)
            {
                pthread_t tid;
                int ret = pthread_creat(&tid,NULL,thread_start,(void*)this);
                if(!ret)
                {
                    cout<<"creat thread error\n";
                    exit(0);
                }
                pthread_detach(tid);
            }
        }

        ~ThreadPool()
        {
            pthread_mutex_destroy(&m_mutex);
            pthread_cond_destroy(&m_cond);
        }

        bool PushTask(const ThreadTask& task)
        {
            pthread_mutex_lock(&m_mutex);
            m_task_queue.push(task);
            pthread_mutex_unlock(&m_mutex);
            pthread_cond_signal(&m_cond);
            return true;
        }


};

//处理任务都是一个套路，并且若是任务的处理方法发生改变,需要将线程池的代码也要改变
//当前线程池与外部的耦合度非常高
//
//若书处理的数据以及这个数据的处理方法,都能够由外部传入，线程池的线程就不在需要关心任何任务处理
//只需要传入使用的方法和数据就可以了
//
//
void test(int data)//用户自己自定义的入口函数
{
    srand(time(NULL));
    int sec = rand() % 5;
    printf("thread:%p get data:%d sleep %d sec\n",pthread_self(),data,sec);
    sleep(sec);
    return;
}
int main()
{
    ThreadPool tp;
    for(int i = 0;i< 10;i++)
    {
        ThreadTask task(i,test);
        tp.PushTask(task);
    }
    return 0;
}
