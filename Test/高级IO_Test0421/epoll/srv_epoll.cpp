#include"epoll.hpp"

int main(int argc,char* argv[])
{
    if(argc != 3)
    {
        printf("./srv_epoll.hpp ip port\n");
        return false;
    }

    std::string ip = argv[1];
    uint16_t port = std::stoi(argv[2]);

    TcpSocket listen_sock;
    CHECK_RET(listen_sock.Socket());//创建监听套接字
    CHECK_RET(listen_sock.SetNonBlock());//将文件设置为非阻塞
    CHECK_RET(listen_sock.Bind(ip,port));
    CHECK_RET(listen_sock.Listen());

    Epoll epoll;
    CHECK_RET(epoll.Add(listen_sock));
    return 0;
}
