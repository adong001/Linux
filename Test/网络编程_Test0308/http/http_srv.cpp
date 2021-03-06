#include"TcpSocket.hpp"
#include<fstream>
#include<iostream>
#include<fstream>

int main(int argc,char* argv[])
{
    if(argc != 3)
    {
        std::cout << "em: ./tcp_scv host_ip host_port\n";
        return -1;
    }

    std::string ip = argv[1];
    uint16_t port = atoi(argv[2]);

    TcpSocket listen_sock;//监视套接字,这个对象只负责客户端连接时创建新的套接字
    CHECK_RET(listen_sock.Socket());//创建套接字
    CHECK_RET(listen_sock.Bind(ip,port));//绑定地址信息
    CHECK_RET(listen_sock.Listen());//监视

    while(1)
    {
        TcpSocket com_sock;//通信套接字,只负责和相应的客户端进行通信

        bool ret = listen_sock.Accept(&com_sock,&ip,&port);
        if(ret == false)
        {
            continue;//服务端不会一次接收客户端失败而退出
        }

        std::string buff;
        com_sock.Recv(&buff);
        std::cout << "http require[" <<buff<<"]\n";

        std::string FirstBalck = "HTTP/1.1 200 OK\r\n";
        std::string black = "\r\n";
        std::stringstream header;
        header << "Content-Length:" << "body.size()" << "\r\n";
        header << "Content-Type: text/html\r\n";
       // header << "Location:http://www.baidu.com/\r\n";
       //
     
        std::string body = "<html><body><div id=\"header\"><h1>我是小丹丹~~</h1></div><div id=\"header\"><h1>最爱看黄片~~</h1></d:iv></body></html>";
        /*body.resize(4096);
        std::ifstream ifs("html.txt",std::ifstream::binary);
        ifs.read((char*)body.c_str(),4096);
        while(!ifs.eof())
        {
            
            ifs.read(tmp,1024);
            int readnum = ifs.gcount();
            body.insert(tmp,tmp+readnum);
        }
        */
        
        com_sock.Send(FirstBalck);
        com_sock.Send(header.str());
        com_sock.Send(black);
        com_sock.Send(body);
        com_sock.Close();

    }
    listen_sock.Close();
    return 0;
}
