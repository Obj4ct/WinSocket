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
	cout << "�����ɹ�" << endl;
	return true;
}
bool CloseSocket()
{
	if (WSACleanup() != 0)
	{
		cout << "ʧ�ܣ�ʧ�ܴ����� " << WSAGetLastError() << endl;
		return false;
	}
	cout << "�رճɹ�" << endl;
	return true;
}

//�����ͻ���socket
SOCKET CreateClientSocket(const char* ip)
{
	//������SOCKET
	//��һ������ ��ַЭ��ipv4 ipv6 
	//�ڶ������� ����Э������ ��ʽ�׽��� ���ݰ� 
	//���������� ʹ�þ����ĳ������Э��
	SOCKET so = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (so == INVALID_SOCKET)
	{
		cout << "ʧ��!" << endl;
		return INVALID_SOCKET;
	}
	//��socket��ip��ַ�Ͷ˿ں�
	//���ӷ�����
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;//�ʹ���socketʱ����һ��
	addr.sin_port = htons(PORT);//define htons �����ֽ�ת�����ֽ�
	addr.sin_addr.S_un.S_addr = inet_addr(ip);//����������ip//inet_addr("127.0.0.1");//תΪ������
	if (connect(so, (struct sockaddr*)&addr, sizeof(addr)) == INVALID_SOCKET)
	{
		cout << "ʧ��" << endl;
		return false;
	}
	return so;

	
}

int main()
{
	InitSocket();

	//�ͻ��˶�
	SOCKET client = CreateClientSocket("127.0.0.1");
	//��������������Ϣ
	char recvBuf[BUFSIZ] = { 0 };
	char sendBuf[BUFSIZ] = { 0 };
	while (true)
	{
		//������Ϣ
		cout << "������Ϣ��";
		gets_s(sendBuf, BUFSIZ);
		if (SOCKET_ERROR == send(client, sendBuf, strlen(sendBuf), 0))
		{
			cout << "ʧ�ܣ�ʧ�ܴ����� " << WSAGetLastError() << endl;
		}
		if (recv(client, recvBuf, BUFSIZ, 0) > 0)
		{
			cout << "���յ���Ϣ��:" << recvBuf << endl;
			memset(sendBuf, 0, sizeof(sendBuf));
		}
		
	}

	closesocket(client);
	CloseSocket();
	cout << "------------------���ӹر�-----------------" << endl;
	//getchar();
}