#include<vector>
#include<sys/epoll.h>
#include"TcpSocket.hpp"

class Epoll
{
    private:
        int m_epfd;//epoll的操作句柄

    public:
        Epoll():
            m_epfd(-1)
    {
        while(1)
        {
            m_epfd = epoll_create(1);//epoll_ctreate的参数已经忽略,只要大于0就行
            if(m_epfd < 0)//创建失败就直接退出
            {
                continue; 
            }
            else
            {
                break;
            }
        } 
    }

    bool Add(TcpSocket& sock)//添加指定事件
    {
        //epoll_ctl(操作句柄,操作,描述符,对应事件结构)
        int fd = sock.GetFd();//获取要添加的socket的fd

        struct epoll_event ev;//填充要监控事件的信息
        ev.data.fd = fd;
        ev.events = EPOLLIN | EPOLLET;//可读事件和边缘触发

        int ret = epoll_ctl(m_epfd,EPOLL_CTL_ADD,fd,&ev);
        if(ret < 0)
        {
            perror("epoll_ctl add error:");
            return false;
        }
        return true;
    }
    
    bool Del(TcpSocket& sock)//删除指定事件
    {
        int fd = sock.GetFd();
        int ret = epoll_ctl(m_epfd,EPOLL_CTL_DEL,fd,NULL);
        if(ret < 0)
        {
            perror("epoll_ctl del error:");
            return false;
        }
        return true;
    }

    bool Wait(std::vector<TcpSocket>* list,int timeout = 3000)
    {
        //epoll_wait(操作句柄,事件数组首地址,数组节点数量,超时时间)
        struct epoll_event evs[10];
        int ret = epoll_wait(m_epfd,evs,10,timeout);
        //就绪的事件都放在evs中,epoll_wait返回值就是就绪事件的个数
        if(ret < 0)
        {
            perror("epoll wait error:");
            return false;
        }
        else if(ret == 0)
        {
            printf("timeout\n");
            return true;
        }

        for(int i = 0;i < ret; i++)
        {
            if(evs[i].events & EPOLLIN)//判断就绪事件是否为可读事件
            {
                TcpSocket sock;
                sock.SetFd(evs[i].data.fd);
                list->push_back(sock);
            }
        }
        return true;
    }
};

