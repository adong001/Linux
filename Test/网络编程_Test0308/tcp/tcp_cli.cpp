#include<iostream>
#include<stdlib.h>
#include"TcpSocket.hpp"

int main(int argc,char* argv[])
{
    if(argc != 3)
    {
        printf("em: ./tcp_cli srv_ip srv_port\n");
        return -1;
    }

    //argv[0]--./tcp_cli  argv[1]--IP地址  argv[2]--Potr端口号
    const std::string ip = argv[1];
    const uint16_t port = atoi(argv[2]);

    TcpSocket cli_sock;
    CHECK_RET(cli_sock.Socket());//创建套接字
    //CHECK_RET(cli_sock.Bind(ip,port))//客户端一般不主动绑定地址信息，
    //有可能会绑定失败，导致程序退出,通常让操作系统来绑定
    CHECK_RET(cli_sock.Connect(ip,port));

    while(1)
    {
        std::cout<<"client say:";
        std::string buff;
        std::cin >> buff;
        CHECK_RET(cli_sock.Send(buff));

        buff.clear();
        CHECK_RET(cli_sock.Recv(&buff));
        std::cout << "server say:"<< buff << std::endl;
    }
    cli_sock.Close();
    return 0;
}
