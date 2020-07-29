#include"client.h"
#define STORE_FILE "./list_backup" //保存文件信息文件的路径
#define LISTEN_DIR "./backup/"	//待备份文件的路径
#define SERVER_IP "192.168.178.129" //服务端ip
#define SERVER_PORT 9000 //服务端port


int main()
{
	Test();
	CloudClient client(LISTEN_DIR, STORE_FILE,SERVER_IP, SERVER_PORT);
	client.Start();
    return 0;
}