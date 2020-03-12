#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<cstring>
#include<iostream>
#include<netinet/in.h>//struct_sockadds_in结构体的定义
#include<sys/socket.h>//套接字接口
#include<arpa/inet.h>//一些字节序装换的接口
#define CHECK_RET(q) if((q)==false){return -1;}
/*
 * 1.创建套接字
 * 2.为套接字绑定地址信息
 * 3.接收数据
 * 4.发送数据
 * 5.关闭套接字
 * */

class UdpSocket
{
    private:
        int m_sockfd;
    public:
        //1.创建套接字
        bool Socket()
        {
            m_sockfd = socket(AF_INET,SOCK_STREAM,IPPROTO_UDP);
            if(m_sockfd < 0 )
            {
                perror("socket error:");
                return false;
            }
            return true;
        }


        void Addr(struct sockaddr_in* addr,const std::string& ip,const uint16_t port)
        {
            addr->sin_family = AF_INET;
            addr->sin_port = htons(port);
            inet_pton(AF_INET,ip.c_str(),&addr->sin_addr.s_addr);
        }

        //2.绑定地址信息
        bool Bind(const std::string& ip,const uint16_t port)
        {
            struct sockaddr_in* addr;
            Addr(addr,ip,port);
            socklen_t len = sizeof(struct sockaddr_in);
            
            int ret = bind(m_sockfd,(struct sockaddr*)&addr,len);
            if(ret < 0)
            {
                perror("bind error:");
                return false;
            }

            return true;

        }
        //3.接收数据
        bool Send(const std::string& data,const std::string& ip,const uint16_t port)
        {
            struct sockaddr_in addr;
            socklen_t len = sizeof(struct sockaddr_in);
            Addr(&addr,ip,port);

            //int sendto(套接字描述符,数据,数据长度,选项,对端信息首地址,对端信息长度)
            int ret = sendto(m_sockfd,data.c_str(),data.size(),0,(struct sockaddr*)&addr,len);
            if(ret < 0)
            {
                perror("sendto error:");
                return false;
            }
            return true;
        }

        //4.接收数据
        bool Recv(std::string* buff,std::string* dest_ip = NULL,uint16_t* dest_port = NULL)
        {
            char tmp[4096] = {0};
            struct sockaddr_in addr;
            socklen_t len = sizeof(struct sockaddr_in);

            //int recvfrom(套接字描述符,接收缓冲区,指定缓冲区大小,选项,d对端地址缓冲区,对端缓冲区长度)
            int ret = recvfrom(m_sockfd,tmp,4096,0,(struct sockaddr*)&addr,&len);
            if(ret < 0)
            {
                perror("recvfrom error:");
                return false;
            }
            
            buff->assign(tmp,ret);//给buff开辟ret长度的空间,将tmp中ret个空间
            if(dest_ip != NULL)
            {
                *dest_ip = inet_ntoa(addr.sin_addr);
            }

            if(dest_port != NULL)
            {
                *dest_port = ntohs(addr.sin_port);
            }
            return true;
        }

        //6.关闭套接字
        bool Close()
        {
            close(m_sockfd);
            return true;
        }
};


int main(int argc,char* argv[])
{
    if(argc != 3)//通过参数传入服务端要绑定的地址信息
    {
        printf("em:./udp_srv 192.168.178.1 123456\n");
        return -1;
    }

    //通过./udp_srv 192.168.178.1 123456来运行程序
    //argc[0]--./udp_srv  argv[1]--192.168.178.1  argv[2]--123456
    
    std::string ip = argv[1];
    uint16_t port = atoi(argv[2]);

    const char* ip_addr= argv[1];//分别获取IP地址和端口数据
    uint16_t port_addr = atoi(argv[2]);
    UdpSocket client;
    CHECK_RET(client.Socket());//检验套接字是否创建成功

    while(1)
    {
        printf("client say:");
        fflush(stdout);
        std::string buff;
        std::cin>>buff;
        client.Send(buff,ip,port);

        buff.clear();   
        client.Recv(&buff);
        printf("server say: %s\n",buff.c_str());
    }
    client.Close();
    return 0;
}
