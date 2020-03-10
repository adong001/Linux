#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<netinet/in.h>//struct_sockadds_in结构体的定义
#include<sys/socket.h>//套接字接口
#include<arpa/inet.h>//一些字节序装换的接口
/*
 * 1.创建套接字
 * 2.为套接字绑定地址信息
 * 3.接收数据
 * 4.发送数据
 * 5.关闭套接字
 * */

int main(int argc,char* argv[])
{
    if(argc != 3)//通过参数传入服务端要绑定的地址信息
    {
        printf("em:./udp_srv 192.168.178.1 123456\n");
        return -1;
    }

    //通过./udp_srv 192.168.178.1 123456来运行程序
    //argc[0]--./udp_srv  argv[1]--192.168.178.1  argv[2]--123456
    const char* ip_addr= argv[1];//分别获取IP地址和端口数据
    uint16_t port_addr = atoi(argv[2]);


    //1.创建套接字
    //int socket(int domain, int type, int protocol);
    //domain(地址域)---AF_INET(IPV4) / AF_INET6(IPV6)...
    //type(套接字类型)---SOCK_STREAM(流式套接字) / SOCK_DGARM(数据包套接字)...
    //protocol(协议类型)---0(不同套接字的默认类型) / IPPROTO_TCP(tcp协议) / IPPROTO_UDP(udp协议)...
    //返回值---0(成功),其他值(errno)
    int sockfd = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
    if(sockfd < 0)
    {
        perror("socket error:");
        return -1;
    }

    //定义IPv4地址结构
    //htons---将无符号两个字节的数据转换成网络字节序的数据
    //inet_pton---将字符串IP地址转换成网络字节序IP地址
    
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;//sin_family这个成员用于表示地址结构类型
    addr.sin_port = htons(port_addr);//网络字节序端口
    inet_pton(AF_INET,ip_addr,&addr.sin_addr.s_addr);//按照IPV4标准将字符串IP地址转换成网络字节序IP地址
    //2.绑定地址信息
    int ret;
    socklen_t len = sizeof(struct sockaddr_in);

    //int bind(int sockfd, struct sockaddr *my_addr, socklen_t addrlen);
    //sockfd---创建完套接字的操作句柄
    //my_addr---地址信息结构
    //addrlen---地址信息结构的大小
    //返回值---0(成功),其他值(errno)
    ret = bind(sockfd,(struct sockaddr*)&addr,len);
    if(ret != 0)
    {
        perror("bind error:");
        return -1;
    }

    //3循环接收/发送数据
    while(1)
    {
        //这个文件作为服务器,先接受数据
        char buff[1024] = {0};
        struct sockaddr_in cliaddr; 
        //ssize_t recvfrom(int sockfd, void *buf, size_t len, int flags,struct sockaddr *src_addr, socklen_t *addrlen);
        //sockfd---创建套接字的操作句柄
        //buf  ---接收的数据
        //len  ---数据的长度
        //flag ---默认为0,缓冲区中没有数据就阻塞等待
        //src_addr---地址缓冲区的首地址,获取这个数据的源端地址信息
        //addrlen ---输入输出型参数,指定获取地址信息的长度,并且返回实际获取的长度
        //返回值 ---成功返回实际接收到的数据长度,失败返回-1
        ret = recvfrom(sockfd,buff,1023,0,(struct sockaddr*)&cliaddr,&len);

        if(ret < 0)
        {
            perror("recvfrom error:");
            close(sockfd);
            return -1;
        }

        char cli_ip[32] = {0};
        uint16_t cli_port = ntohs(cliaddr.sin_port);//将网络字节序端口转换为字符串端口
        inet_ntop(AF_INET,&cliaddr.sin_addr,cli_ip,32);//按照IPV4标准将网络字节序IP地址转换成字符串IP地址

        printf("client[%s:%d]say:%s\n",cli_ip,cli_port);

        printf("server say:");
        fflush(stdout);
        memset(buff,0x00,1024);//清空缓冲区
        scanf("%s",buff);

        //ssize_t sendto(int sockfd, const void *buf, size_t len, int flags,const struct sockaddr *dest_addr, socklen_t addrlen);
        //sockfd---套接字的操作句柄
        //buf   ---要发送的数据首地址
        //len   ---发送数据的长度
        //flag  ---选项参数,默认为0,缓冲区满则阻塞
        //dest_addr---目的端(接收端)的地址信息首地址
        //addr_len ---指定发送目的端的地址信息的长度
        //返回值---成功返回实际发送数据的长度,失败返回-1
        ret = sendto(sockfd,buff,strlen(buff),0,(struct sockaddr*)&cliaddr,len);
        if(ret < 0)
        {
            perror("sendto error:");
            close(sockfd);
            return -1;
        }
    }
    
    close(sockfd);
    return 0;
}
