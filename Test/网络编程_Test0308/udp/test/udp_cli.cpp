#include<stdio.h>                                                     
#include<stdlib.h>
#include<unistd.h>
#include<cstring>
#include<iostream>
#include<netinet/in.h>//struct_sockadds_in结构体的定义
#include<sys/socket.h>//套接字接口
#include<arpa/inet.h>//一些字节序装换的接口

#define CHECK_RET(FUN) if(FUN == false){return -1;}

class Client
{
    private:
        int m_sockfd;

    void Addr(struct sockaddr_in& addr,const std::string& ip,const uint16_t port)
    {
        addr.sin_family = AF_INET;
        addr.sin_port = htons(port);
        addr.sin_addr.s_addr = inet_addr(ip.c_str());
    }

    public:
        Client(int sockfd = -1):
            m_sockfd(sockfd)
         {}

        //1.创建套接字
        bool Socket()
        {
            m_sockfd = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
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
            Addr(addr,ip,port);
            int len = sizeof(struct sockaddr_in);
            int ret = bind(m_sockfd,(struct sockaddr*)&addr,len);
            if(ret < 0)
            {
                perror("nidn error:");
                close(m_sockfd);
                return false;
            }
            return true;
        }
        
        //3.接收消息
        bool Recv(std::string& buff,std::string* det_ip = NULL,uint16_t* det_port = NULL)
        {
            char tmp[4096] = {0};
            struct sockaddr_in addr;
            socklen_t len = sizeof(struct sockaddr_in);
            int ret = recvfrom(m_sockfd,tmp,4096,0,(struct sockaddr*)&addr,&len);
            if(ret < 0)
            {
                perror("recvfrom error:");
                close(m_sockfd);
                return false;
            }

            //assign(tmp,ret)--给buff开辟ret长度的空间，将tmp拷贝进去
            buff.assign(tmp,ret);
            if(det_ip != NULL)
            {
                *det_ip = inet_ntoa(addr.sin_addr);
            }
            if(det_port != NULL)
            {
                *det_port = ntohs(addr.sin_port);
            }
            return true;

        }

        //4.发送消息
        bool Send(const std::string& data,const std::string& ip,const uint16_t& port)
        {
            struct sockaddr_in addr;
            int len = sizeof(struct sockaddr_in);
            Addr(addr,ip,port);
            int ret = sendto(m_sockfd,data.c_str(),data.size(),0,(struct sockaddr*)&addr,len);
            if(ret < 0)
            {
                perror("send error:");
                close(m_sockfd);
                return false;
            }
            return true;
        }
        //5.关闭套接字
        bool Close()
        {
            if(close(m_sockfd) < 0)
            {
                perror("close error:");
                return false;
            }
            return true;
        }
};


int main(int argc,char* argv[])
{
    if(argc != 3)
    {
        printf("em:./udp_cli 192.168.178.131 9000\n");
        return -1;
    }
    std::string ip = argv[1];
    uint16_t port = atoi(argv[2]);
    const char* ip_addr = argv[1];
    uint16_t port_addr = atoi(argv[2]);
    
    Client client;
    CHECK_RET(client.Socket());

    while(1)
    {
        printf("client say:");
        fflush(stdout);
        std::string buff;
        std::cin >> buff;
        client.Send(buff,ip,port);
        buff.clear();
        client.Recv(buff);
        printf("server say: %s\n",buff.c_str());
    }
    client.Close();
    return 0;
}

