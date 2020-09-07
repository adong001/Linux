#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<netinet/in.h>//struct_sockadds_in结构体的定义
#include<sys/socket.h>//套接字接口
#include<arpa/inet.h>//一些字节序装换的接口


int main(int argc, char* argv[])
{

    if(argc != 3)
    {
        perror("input error! em: ip:192.168.178.131 port:9000\n");
        return -1;
    }


    const char* ip_addr = argv[1];
    uint16_t port_addr = atoi(argv[2]);
    //1.创建套接字
    int ret;
    int sockfd = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
    if(sockfd < 0)
    {
        perror("s;ocket error:");
        return -1;
    }

    //2.Bind地址信息
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port_addr);
    addr.sin_addr.s_addr = inet_addr(ip_addr);
    //inet_pton(AF_INET,ip_addr,&addr.sin_addr.s_addr);
    socklen_t len = sizeof(struct sockaddr_in);

    ret = bind(sockfd,(struct sockaddr*)&addr,len);
    if(ret < 0)
    {
        perror("bind error:");
        return -1;
    }

    //3.接收发送信息
    while(1)
    {
        char buf[1024] = {0};
        struct sockaddr_in cliaddr;
        ret = recvfrom(sockfd,buf,1023,0,(struct sockaddr*)&cliaddr,&len);
        if(ret < 0)
        {
            perror("recvfrom error:");
            close(sockfd);
            return -1;
        }
        char cli_ip[32] = {0};
        inet_ntop(AF_INET,&cliaddr.sin_addr,cli_ip,32);
        uint16_t cli_port = ntohs(cliaddr.sin_port);
        printf("client[%s:%d]say:%s\n",cli_ip,cli_port,buf);
        printf("server say:");
        fflush(stdout);
        memset(buf,0x00,1024);
        scanf("%s",buf);
        ret = sendto(sockfd,buf,strlen(buf),0,(struct sockaddr*)&cliaddr,len);
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
