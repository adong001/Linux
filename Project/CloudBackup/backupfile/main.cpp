#include"client.h"
#define STORE_FILE "./list_backup" //�����ļ���Ϣ�ļ���·��
#define LISTEN_DIR "./backup/"	//�������ļ���·��
#define SERVER_IP "192.168.178.129" //�����ip
#define SERVER_PORT 9000 //�����port


int main()
{
	Test();
	CloudClient client(LISTEN_DIR, STORE_FILE,SERVER_IP, SERVER_PORT);
	client.Start();
    return 0;
}