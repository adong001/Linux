#include"epoll.hpp"

int main(int argc,char* argv[])
{
    if(argc != 3)
    {
        printf("./srv_epoll.hpp ip port\n");
        return -1;
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
    while(1)
    {
        std::vector<TcpSocket> list;
        bool ret = epoll.Wait(&list);//监控就绪的文件描述符添加到list中

        if(ret == false)//epoll监控失败
        {
            return -1;
        }

        for(int i = 0;i < list.size();i++)//轮询处理list中的就绪事件
        {
            if(list[i].GetFd() == listen_sock.GetFd())//是监听套接字
            {
                TcpSocket client_sock;
                bool ret = listen_sock.Accept(&client_sock);//为新到来的连接创建新套接字
                if(ret ==false)//创建失败重新创建
                {
                    continue;
                }
                client_sock.SetNonBlock();//新套接字设置为非阻塞,
                epoll.Add(client_sock);//加入epoll监控中
            }

            else//到来的是通行套接字
            {
                std::string buff;
                ret = list[i].Recv(&buff);
                if(ret == false)//连接中断导致接受失败
                {
                    epoll.Del(list[i]);//移除监控个套接字
                    list[i].Close();//关闭这个套接字
                    continue;//继续监控
                }
                std::cout << "client say:" << buff <<std::endl;
                std::cout << "server say:";
                buff.clear();
                std::cin >> buff;
                ret = list[i].Send(buff);//发送数据
                if(ret == false)//发送失败
                {
                    epoll.Del(list[i]);//移除监控
                    list[i].Close();
                }
            }
        }
    }
    listen_sock.Close();//监控结束,关闭监听套接字
    return 0;
}
