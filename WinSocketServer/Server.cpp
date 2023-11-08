//�����
#include<iostream>
#include <WinSock2.h>
#define PORT 8888 //[0,65536) 0-1024 ��ϵͳ�����˿ڣ������ڴ�����ѡȡ�����������ȡ
#pragma comment (lib,"ws2_32.lib")
using namespace std;
bool InitSocket()
{
	WSADATA wsadata;
	if (WSAStartup(MAKEWORD(2, 2), &wsadata) != 0)
	{
		cout << "ʧ�ܣ�ʧ�ܴ����� " << WSAGetLastError() << endl;
		return false;
	}

	return true;
}
bool CloseSocket()
{
	if (WSACleanup() != 0)
	{
		cout << "ʧ�ܣ�ʧ�ܴ����� " << WSAGetLastError() << endl;
		return false;
	}
	
	return true;
}
//����������socket
SOCKET CreateSeverSocket()
{
	//������SOCKET
	//��һ������ ��ַЭ��ipv4 ipv6 
	//�ڶ������� ����Э������ ��ʽ�׽��� ���ݰ� 
	//���������� ʹ�þ����ĳ������Э��
	//����Э��Ϊ TCP
	SOCKET so = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	//TCP����ʧ��
	if (so == INVALID_SOCKET)
	{
		cout << "ʧ�ܣ�ʧ�ܴ����� " << WSAGetLastError() << endl;
		return INVALID_SOCKET;
	}
	//��socket��ip��ַ�Ͷ˿ں�
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;//�ʹ���socketʱ����һ��
	addr.sin_port = htons(PORT);//define htons �����ֽ�ת�����ֽ�
	addr.sin_addr.S_un.S_addr = ADDR_ANY;//����������ip//inet_addr("127.0.0.1");//תΪ������
	if (bind(so, (struct sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR)
	{
		cout << "ʧ�ܣ�ʧ�ܴ����� " << WSAGetLastError() << endl;
		return false;
	}
	//����
	listen(so, 100);
	return so;
	

}
int main()
{
	InitSocket();
	SOCKET server= CreateSeverSocket();
	//�ȴ��ͻ�������
	cout << "�ȴ��ͻ�������..." << endl;
	//����пͻ�����������
	SOCKET client=accept(server,NULL,NULL);
	if (INVALID_SOCKET==client)
	{
		cout << "ʧ�ܣ�ʧ�ܴ����� " << WSAGetLastError() << endl;
	}
	//���ԺͿͻ��˽���ͨ����
	//��ָ����socket����
	char revBuf[BUFSIZ] = { 0 };
	char sendBuf[BUFSIZ] = { 0 };
	while (true)
	{
		if (recv(client, revBuf, BUFSIZ, 0) > 0)
		{
			cout << "���յ���Ϣ��:" << revBuf << endl;
		}
		//������Ϣ
		cout << "������Ϣ��";
		memset(sendBuf, 0, sizeof(sendBuf));
		gets_s(sendBuf, BUFSIZ);
		if (SOCKET_ERROR == send(client, sendBuf, strlen(sendBuf), 0))
		{
			cout << "ʧ�ܣ�ʧ�ܴ����� " << WSAGetLastError() << endl;
		}
	}
	closesocket(client);
	closesocket(server);
	CloseSocket();
	cout << "------------------���ӹر�-----------------" << endl;
}