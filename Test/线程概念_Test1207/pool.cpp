#include<cstdlib>
#include<unistd.h>
#include<iostream>
#include<mutex>
#include<cstdio>
#include<pthread.h>
#include<queue>
#include<ctime>
#define MAX_THREAD 5//最大线程数

typedef void(*task_handler)(int);
class ThreadTask
{
    private:
        //函数指针task_handler
        task_handler m_task_handler;
        int  m_val;

    public:
        ThreadTask(task_handler handler,int val):
            m_task_handler(handler),
            m_val(val)
    {}

        void Run()
        {
            return m_task_handler(m_val);
        }
};


class ThreadPool
{
    private:
        size_t m_thread_count;//记录线程的个数
        pthread_mutex_t m_mutex;
        pthread_cond_t m_cond;
        std::queue<ThreadTask> m_queue;

    public:
        ThreadPool(size_t thread_count = MAX_THREAD):
            m_thread_count(thread_count)
    {
        //初始化互斥锁和条件变量
        pthread_mutex_init(&m_mutex,NULL);
        pthread_cond_init(&m_cond,NULL);
        for(int i = 0; i < m_thread_count;i++)
        {
            pthread_t tid;
            int ret = pthread_create(&tid, NULL,thread_start,(void*)this);
            if(ret != 0)
            {
                perror("thread create error:");
                exit(0);
            }

            pthread_detach(tid);//线程分离
        }
    }

        ~ThreadPool()
        {
            pthread_mutex_destroy(&m_mutex);
            pthread_cond_destroy(&m_cond);
        }

        bool PushTask(ThreadTask& task)
        {
            pthread_mutex_lock(&m_mutex);//加锁
            m_queue.push(task);
            pthread_mutex_unlock(&m_mutex);//解锁
            pthread_cond_signal(&m_cond);//唤醒
            return true;
        }

        static void* thread_start(void* argc)
        {

            ThreadPool* tp = (ThreadPool*)argc;

            while(1)
            {
                pthread_mutex_lock(&(tp->m_mutex));

                while(tp->m_queue.empty())
                {
                    pthread_cond_wait(&(tp->m_cond),&(tp->m_mutex));
                }

                ThreadTask task = tp->m_queue.front();
                tp->m_queue.pop();
                pthread_mutex_unlock(&(tp->m_mutex));

                task.Run();

            }
            return NULL;
        } 

};



void task(int val)
{
    srand(time(NULL));
    int Sl = rand() % 5;//使每个进程的执行长度不同
    printf("thread:%p,  get data:%d,  sleep:%d\n",pthread_self(),val,Sl);
    sleep(Sl);
    return ;
}

int main()
{
    ThreadPool tp;
    for(int i = 0;i < 10; i++)
    {
        ThreadTask Task(task,i);
        tp.PushTask(Task);
    }
    while(1)
    {
        sleep(1);
    }
    return 0;
}
