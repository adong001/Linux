#include<stdlib.h>                                                                                                                              
#include<stdio.h>                       
#include<unistd.h>      
#include<string>                                                        
#include<netinet/in.h>//struct_sockadds_in结构体的定义          
#include<sys/socket.h>//套接字接口          
#include<arpa/inet.h>//一些字节序装换的接口      
                                 
#define BACKLOG 10 //同一时间最大连接数      
#define CHECK_RET(q) if((q)==false){return -1;}//判断TcpSocket封装的函数是否调用成功 


class TcpSocket
{
    private:
        int m_sockfd;
    
        void Addr(struct sockaddr_in* addr,const std::string& ip,const uint16_t port)
        {
            addr->sin_family = AF_INET;
            addr->sin_port = htons(port);
            inet_pton(AF_INET,ip.c_str(),&(addr->sin_addr.s_addr));
        }

  public:

        TcpSocket():m_sockfd(-1)
        {}

        bool Socket()
        {
            m_sockfd = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
            if(m_sockfd < 0)
            {
                perror("socket error:");
                return false;
            }
            return true;
        }

        bool Bind(const std::string& ip,const uint16_t port)
        {
            struct sockaddr_in addr;
            Addr(&addr,ip,port);
            socklen_t len = sizeof(struct sockaddr_in);
            int ret = bind(m_sockfd,(struct sockaddr*)&addr,len);
            if(ret < 0)
            {
                perror("bind error:");
                return false;
            }
            return true;

        }

        bool Listen(int backlog = BACKLOG)
        {
            int ret = listen(m_sockfd,backlog);
            if(ret < 0)
            {
                perror("listen error:");
                return false;
            }
            return true;
        }

        bool Connnect(const std::string& ip,const uint16_t port)
        {
            struct sockaddr_in addr;
            Addr(&addr,ip,port);
            socklen_t len = sizeof(struct sockaddr_in);
            int ret = connect(m_sockfd,(struct sockaddr*)&addr,len);
            if(ret < 0)
            {
                perror("connect error:");
                return false;
            }
            return true;
        }

        bool Accept(TcpSocket& sock,std::string* ip = NULL ,uint16_t* port = NULL)
        {
            struct sockaddr_in addr;
            socklen_t len = sizeof(struct sockaddr_in);
            int clisockfd = accept(m_sockfd,(struct sockaddr*)&addr,&len);
            if(clisockfd < 0)
            {
                perror("accept error:");
                return false;
            }
            sock.m_sockfd = clisockfd;
            if(ip != NULL)
            {
                *ip = inet_ntoa(addr.sin_addr);
            }
            if(port != NULL)
            {
                *port = ntohs(addr.sin_port);
            }
            return true;
        }


        bool Send(const std::string& data)
        {
            int ret = send(m_sockfd,data.c_str(),data.size(),0);
            if(ret < 0)
            {
                perror("send error:");
                return false;
            }
            return true;
        }

        bool Recv(std::string* buf)
        {
            char tmp[4096] = {0};
            int ret = recv(m_sockfd, tmp, 4096,0);
            if(ret < 0)
            {
                perror("recv error:");
                return false;
            }
            else if(ret == 0)
            {
                printf("connect break\n");
                return false;
            }
            buf->assign(tmp,ret);
            return true;
        }

        void Close()
        {
            close(m_sockfd);
        }
};
