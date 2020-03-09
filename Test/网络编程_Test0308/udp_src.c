#include<stdio.h>
#include<stdlib.h>
#include<unist.h>
#include<netinet/in.h>//struct_sockadds_in结构体的定义
#include<sys/socket.h>//套接字接口
#include<arpa/inet.h>//一些字节序装换的接口
/*
 * 1.创建套接字
 * 2.
 * */

int mian(int argc,char* argv[])
{
    if(argc != 3)
    {
        printf("em: ./udp_src 192.168.122.132 90000\n");
        return -1;
    }

    const char* ip_addr= argv[1];
    uint16_t port_addr = atoi(argv[2]);

    int sockfd = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
    if(sockfd < 0)
    {
        perror()
    }
    return 0;
}
