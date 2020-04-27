#include"select.hpp"

int main(int argc,char* argv[])
{
    if(argc != 3)
    {
        printf("usage: ./srv_select ip port\n");
        return -1;
    }

    std::string srv_ip = argv[1];
    uint16_t srv_port = std::stoi(argv[2]);
    Select sel;

    TcpSocket listen_sock;
    CHECK_RET(listen_sock.Socket());//创建套接字
    CHECK_RET(listen_sock.Bind(srv_ip,srv_port));//为套接字绑定地址信息
    CHECK_RET(listen_sock.Listen());//开始监听

    sel.Add(listen_sock);//添加监控套接字
    while(1)
    {
        std::vector<TcpSocket> list;
        bool ret = sel.Wait(&list);//开始监控新的套接字
        if(ret == false)//监控出错
        {
            return -1;
        }

        for(auto sock: list)//遍历就绪集合中所有描述符
        {
            if(sock.GetFd() == listen_sock.GetFd())//这个就绪套接字是监听套接字,说明有新连接到来
            {
                TcpSocket client;
                bool ret = listen_sock.Accept(&client);//获取新连接的fd;
                if(ret == false)//获取失败，继续获取
                {
                    continue;
                }
                sel.Add(client);//将新的通信套接字添加到监控队列中
            }

            else//这个到来的套接字是一个通信套接字,那就通信即可
            {
                std::string buf;
                ret = sock.Recv(&buf);//服务端接收数据
                if(ret == false)
                {
                    sel.Clear(sock);//监控出错,移除描述符
                    sock.Close();//关闭套接字
                    continue;
                }
                std::cout << "client say: " << buf << std::endl;
                buf.clear();

                std::cout << "srever say : ";//服务端发送数据
                std::cin >> buf;
                ret = sock.Send(buf);
                if(ret == false)
                {
                    sel.Clear(sock);//监控出错,移除描述符
                    sock.Close();//关闭套接字
                    continue;
                }
            }
        }
    }
    listen_sock.Close();//关闭监听套接字
    return 0;
}
