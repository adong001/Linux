#include<stdlib.h>
#include<stdio.h>                            
#include<unistd.h>    
#include<fcntl.h>
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

        //地址转换
        void Addr(struct sockaddr_in* addr,const std::string& ip,const uint16_t& port)
        {
            addr->sin_family = AF_INET;
            addr->sin_port = htons(port);
            inet_pton(AF_INET,ip.c_str(),&addr->sin_addr.s_addr);
        }
    public:
        TcpSocket(int sockfd = -1):
            m_sockfd(sockfd)
    {}

        void SetFd(int fd)
        {
            m_sockfd = fd;
        }

        int GetFd()
        {
            return m_sockfd;
        }

        bool SetNonBlock()//将文件设置为非阻塞
        {

            int flag = fcntl(m_sockfd,F_GETFL,0);//获取文件属性
            int ret = fcntl(m_sockfd,F_SETFL,flag | SOCK_NONBLOCK);//在文件原有属性上设置为非阻塞
            if(ret == -1)
            {
                return false;
            }
            return true;
        }
        //1.创建套接字
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

        //2.绑定地址信息
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

        //3.监听
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

        //4.客户端发起请求连接
        bool Connect(const std::string& ip,const uint16_t& port)
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

        //5.服务端获取新建连接
        bool Accept(TcpSocket* sock,std::string* ip = NULL,uint16_t* port = NULL)
        {
            struct sockaddr_in cli_addr;//客户端地址信息
            socklen_t len = sizeof(struct sockaddr_in);

            int newfd = accept(m_sockfd,(struct sockaddr*)&cli_addr,&len);//客户端连接服务端是创建一个新的套接字
            if(newfd < 0 )
            {
                perror("accept error:");
                return false;
            }

            //新建的套接字用于和这个客户端通信,传入的sock对象被赋值为这个新的对象
            sock->m_sockfd = newfd;
            if(ip != NULL)
            {
                *ip = inet_ntoa(cli_addr.sin_addr);
            }

            if(port != NULL)
            {
                *port = ntohs(cli_addr.sin_port);
            }
            return true;
        }

        //6.接收数据
        bool Recv(std::string* buff)
        {
            while(1)
            {
                char tmp[10] = {0};
                //ssize_t recv(int sockfd, void *buf, size_t len, int flags);
                //sockfd---描述符
                //buf---接收缓冲区
                //len---指定接收长度
                //flags--选项参数，默认为0
                //返回值--正数(实际接收的数据长度) 0(接收数据过程中,连接中断) -1/errno(出错)
                ssize_t ret = recv(m_sockfd,tmp,4096,0);

                if(ret < 0 )
                {
                    if(errno == EAGAIN)//表示非阻塞下,缓冲区没有数据,就可以退出了
                    {
                        return true;
                    }
                    perror("recv error:");
                    return false;
                }
                else if(ret == 0)//连接断开
                {
                    printf("connection break\n");
                    return false;;
                }

                *buff +=tmp;//从tmp缓冲区中拷贝ret大小的数据到buff
            }
            return true;
        }

        //7.发送数据
        bool Send(const std::string& data)
        {
            ssize_t ret = send(m_sockfd,data.c_str(),data.size(),0);
            if(ret < 0 )
            {
                perror("send error:");
                return false;
            }       

            return true;
        }

        //8.关闭套接字
        bool Close()
        {
            int ret = close(m_sockfd);
            if(ret < 0)
            {
                perror("close error:");
                return false;
            }
            m_sockfd = -1;
            return true;
        }

};
