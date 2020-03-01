#include<iostream>
#include<cstdio>
#include<cstdlib>
#include<mutex>
using namespace std;
//单例模式
//单例模式就是资源只会被加载一次，不管有多少资源，只有一份资源

//1、饿汉模式
class starving
{
   private:
         static int s_m_data;//将资源修饰为static,在程序初始化阶段完成资源的初始化,并且各个对象之间共用同一份资源
   public:
        int* get_instance()
        {
            return &s_m_data;
        }
};

int starving::s_m_data = 10;


//2、懒汉模式
std::mutex mu;
class lazybones
{
    private:
        //初始化阶段只是初始化了指针,并没有加载资源
        volatile static int* s_m_data;//防止编译器过度优化,保证资源内存可见性,每次判断都为空,申请资源
    public:
        volatile int* get_instance()
        {
            //若资源已经加载成功m后序访问资源的线程依然会加锁判断是否为空,平白多了加解锁的操作
            if(s_m_data == NULL)//对资源进行锁外判断,防止白加
            {
                mu.lock();
                if(s_m_data == NULL)//在访问这个资源的时候才会加载资源完成资源的初始化
                {
                    s_m_data = new int;
                }
                mu.unlock();
            }
            return s_m_data;
        }
};
volatile int* lazybones::s_m_data = NULL;

int main()
{
    //饿汉模式,资源在初始化就加载好
    starving a,c;
    printf("%p-%p\n",a.get_instance(),c.get_instance());

    //懒汉模式,资源在需要时才加载
    lazybones b,d;
    printf("%p-%p\n",b.get_instance(),d.get_instance());
    return 0;
}
