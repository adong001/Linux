#include"TcpSocket.hpp"
#include<signal.h>
#include<sys/wait.h>
#include<iostream>
#include<pthread.h>

void *thread_start(void* arg)//线程入口函数
{
    long sockfd = (long)arg;

    TcpSocket com_sock;
    com_sock.SetFd(sockfd);

    while(1)
    {
        if(sockfd < 0)
        {
            printf("connection break\n");
            pthread_exit(NULL);//每次检查连接是否断开,客户端断开就会关闭客户端套接字，m_sockfd = -1
            return NULL;
        }
        std::string buff;
    
        bool ret = com_sock.Recv(&buff);
        if(ret== false)
        {
            sleep(1);
            com_sock.Close();
            continue;//接收数据失败,重新接收
        }

        printf("client say:%s\n",buff.c_str());
        std::cout<<"server say:";
        buff.clear();
        std::cin>>buff;

        ret = com_sock.Send(buff);
        if(ret == false)
        {
            com_sock.Close();
            continue;//发送失败，重新发送
        }
    }
    com_sock.Close();
    return NULL;

}

int main(int argc,char* argv[])
{
    if(argc != 3)
    {
        std::cout << "em: ./tcp_scv host_ip host_port\n";
        return -1;
    }

    const std::string ip = argv[1];
    const uint16_t port = atoi(argv[2]);

    TcpSocket listen_sock;//监视套接字,这个对象只负责客户端连接时创建新的套接字
    CHECK_RET(listen_sock.Socket());//创建套接字
    CHECK_RET(listen_sock.Bind(ip,port));//绑定地址信息
    CHECK_RET(listen_sock.Listen());//监视

    while(1)
    {
        TcpSocket com_sock;//通信套接字,只负责和相应的客户端进行通信

        std::string cli_ip;
        uint16_t cli_port;
        bool ret = listen_sock.Accept(&com_sock,&cli_ip,&cli_port);
        if(ret == false)
        {
            printf("waiting connect!!!");
            sleep(1);
            continue;//服务端不会一次接收客户端失败而退出
        }

        std::cout << "new connection[" << cli_ip.c_str()<<":"<<cli_port<<"]\n";

        pthread_t tid;
        pthread_create(&tid,NULL,thread_start,(void*)(com_sock.GetFd()));
        pthread_detach(tid);
    }
    listen_sock.Close();
    return 0;
}
