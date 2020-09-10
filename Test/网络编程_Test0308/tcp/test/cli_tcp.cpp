#include<iostream>
#include"tcpsocket.hpp"

int main(int argc,char* argv[])
{
    if(argc != 3)
    {
        printf("em: ./cli_tcp 192.168.178.131 9000\n");
        return -1;
    }
    
    const std::string ip = argv[1];
    uint16_t port = atoi(argv[2]);
    TcpSocket cli_sock;
    CHECK_RET(cli_sock.Socket());
    CHECK_RET(cli_sock.Connnect(ip,port));

    while(1)
    {
        std::string buf;
        printf("client say:%s\n",buf.c_str());
        std::cin >> buf;

        CHECK_RET(cli_sock.Send(&buf[0]));
        buf.clear();
        CHECK_RET(cli_sock.Recv(&buf));
        printf("ip[%s] port[%d] server say:%s\n",ip.c_str(),port,buf.c_str());
    }
    cli_sock.Close();
    return 0;
}
