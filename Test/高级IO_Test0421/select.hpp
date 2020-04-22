#include<iostream>
#include<vector>
#include<sys/select.h>
#include<time.h>
#include"TcpSocket.hpp"

class Select
{
    private:
        int m_maxfd;  //最大描述符
        fd_set m_rfds;//可读事件的描述符集合

    public:
        Select():
            m_maxfd(-1)
        {
            FD_ZERO(&m_rfds);//初始化清空写描述符
        }

        bool Add(TcpSocket& sock)//添加描述符到监控集合中来
        {
            int fd = sock.GetFd();
            FD_SET(fd,&m_rfds);//将描述符添加到集合中来
            m_maxfd = m_maxfd > fd ? m_maxfd : fd;//每次添加新的描述符，就要更新最大描述符数
            return true;
        }

        bool Clear(TcpSocket& sock)//从监控中移除描述符
        {
            int fd = sock.GetFd();
            FD_CLR(fd,&m_rfds);//从集合中移除描述符
            if(fd != m_maxfd)//判断删除的是否为最大描述符,不是就更新m_maxfd
            {
                return true;
            }
            for(int i = fd; i >= 0;--i)
            {
                if(FD_ISSET(i,&m_rfds))//重新判断最大的描述符是多少
                {
                    m_maxfd = i;
                    break;
                }
            }
            return true;
        }

        bool Wait(std::vector<TcpSocket>* list,int mtimeout = 3000)//开始监控，返回就绪的socket信息
        {   
            struct timeval tv;
            tv.tv_sec = mtimeout / 1000;//微秒
            tv.tv_usec= (mtimeout % 1000) / 1000;//毫秒

            fd_set tmp_rfds = m_rfds;//写IO集合,select后会改变m_rfds,返回时会将未就绪的描述符移除
            int nfds = select(m_maxfd + 1, &tmp_rfds,NULL, NULL, &tv);

            if(nfds < 0)//监控出错
            {
                perror("select error:");
                return false;
            }

            else if(nfds == 0)//监控等待超时
            {
                printf("wait timeout\n");
                return true;
            }

            for(int i = 0;i <= m_maxfd; i++)//挨个判断谁在集合谁就是就绪的
            {
                if(FD_ISSET(i, &tmp_rfds))
                {
                    TcpSocket sock;
                    sock.SetFd(i);
                    list->push_back(sock);//就绪描述符加入list中
                }
            }
            return true;
        }
};
