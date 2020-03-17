#include"TcpSocket.hpp"
#include<signal.h>
#include<sys/wait.h>
#include<iostream>


void sigcb(int signo)
{
    //WNOHANG---非阻塞操作
    //返回值-- >0(表示有子进程退出)  <=0 (没有子进程退出)
    //SIGCHLD是一个不可靠信号，因此采用循环处理
    while(waitpid(-1,NULL,WNOHANG) > 0);
}

int main(int argc,char* argv[])
{
    if(argc != 3)
    {
        std::cout << "em: ./tcp_scv host_ip host_port\n";
        return -1;
    }

    signal(SIGCHLD,sigcb);//对SIGCLD信号自定义处理,等到子进程退出后释放资源,避免父进程一直死等子进程退出

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
            continue;//服务端不会一次接收客户端失败而退出
        }

        std::cout << "new connection[" << cli_ip.c_str()<<":"<<cli_port<<"]\n";

        pid_t pid = fork();//创建子进程
        if(pid == 0)
        {
            while(1)
            {
                //每个子进程负责和对应的客户进程通信
                std::string buff;
                ret = com_sock.Recv(&buff);
                if(ret== false)
                {
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
            exit(0);//子进程结束，关闭套接字
        }
        com_sock.Close();
        //父子进程独有一份通信套接字,子进程与对应的客户端通信结束后，退出时父进程也要关闭对应的套接字
    }
    listen_sock.Close();
    return 0;
}
