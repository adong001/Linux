#include<iostream>
#include<string>
#include<sys/epoll.h>
#include"TcpSocket.hpp"
using namespace std;

class Epoll
{
    private:
        int m_epfd;//epoll的操作句柄
 
    public:
        Epoll():
            m_epfd(-1)
        {
            m_epfd = epoll_create(1);//epoll_ctreate的参数已经忽略,只要大于0就行
            if(m_epfd < 0)
            {
                perror("epoll_create error；");
                exit(-1); 
            }
        }
};

