#include<iostream>
#include<sys/epoll.h>
#include<string>
#include"TcpSocket.hpp"
using namespace std;

int main(int argc,char* argv[])
{
    if(argc != 3)
    {
        printf("./srv_epoll.hpp ip port\n");
        return false;
    }

    string ip = argv[1];
    uint16_t port = stoi(argv[2]);

    TcpSocket listen_sock;
    CHECK_RET(listen_scok.Socket());//创建监听套接字
    CHECK_RET(listen_sock.SetNonBlock());//将文件设置为非阻塞
    return 0;
}
