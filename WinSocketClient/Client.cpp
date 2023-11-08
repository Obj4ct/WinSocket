//服务端
#include<iostream>
#include <WinSock2.h>
#define PORT 8888 //[0,65536) 0-1024 是系统保留端口，不可在此区间选取，区间外随便取
#pragma comment (lib,"ws2_32.lib")
using namespace std;
bool InitSocket()
{
	WSADATA wsadata;
	if (WSAStartup(MAKEWORD(2, 2), &wsadata) != 0)
	{
		cout << "失败！失败代码是 " << WSAGetLastError() << endl;
		return false;
	}
	cout << "开启成功" << endl;
	return true;
}
bool CloseSocket()
{
	if (WSACleanup() != 0)
	{
		cout << "失败！失败代码是 " << WSAGetLastError() << endl;
		return false;
	}
	cout << "关闭成功" << endl;
	return true;
}

//创建客户端socket
SOCKET CreateClientSocket(const char* ip)
{
	//创建空SOCKET
	//第一个参数 地址协议ipv4 ipv6 
	//第二个参数 传输协议类型 流式套接字 数据包 
	//第三个参数 使用具体的某个传输协议
	SOCKET so = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (so == INVALID_SOCKET)
	{
		cout << "失败!" << endl;
		return INVALID_SOCKET;
	}
	//给socket绑定ip地址和端口号
	//连接服务器
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;//和创建socket时必须一样
	addr.sin_port = htons(PORT);//define htons 本地字节转网络字节
	addr.sin_addr.S_un.S_addr = inet_addr(ip);//网卡的任意ip//inet_addr("127.0.0.1");//转为二进制
	if (connect(so, (struct sockaddr*)&addr, sizeof(addr)) == INVALID_SOCKET)
	{
		cout << "失败" << endl;
		return false;
	}
	return so;

	
}

int main()
{
	InitSocket();

	//客户端端
	SOCKET client = CreateClientSocket("127.0.0.1");
	//给服务器发送信息
	char recvBuf[BUFSIZ] = { 0 };
	char sendBuf[BUFSIZ] = { 0 };
	while (true)
	{
		//发送消息
		cout << "发送消息：";
		gets_s(sendBuf, BUFSIZ);
		if (SOCKET_ERROR == send(client, sendBuf, strlen(sendBuf), 0))
		{
			cout << "失败！失败代码是 " << WSAGetLastError() << endl;
		}
		if (recv(client, recvBuf, BUFSIZ, 0) > 0)
		{
			cout << "接收的信息是:" << recvBuf << endl;
			memset(sendBuf, 0, sizeof(sendBuf));
		}
		
	}

	closesocket(client);
	CloseSocket();
	cout << "------------------连接关闭-----------------" << endl;
	//getchar();
}