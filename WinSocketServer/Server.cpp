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

	return true;
}
bool CloseSocket()
{
	if (WSACleanup() != 0)
	{
		cout << "失败！失败代码是 " << WSAGetLastError() << endl;
		return false;
	}
	
	return true;
}
//创建服务器socket
SOCKET CreateSeverSocket()
{
	//创建空SOCKET
	//第一个参数 地址协议ipv4 ipv6 
	//第二个参数 传输协议类型 流式套接字 数据包 
	//第三个参数 使用具体的某个传输协议
	//传输协议为 TCP
	SOCKET so = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	//TCP连接失败
	if (so == INVALID_SOCKET)
	{
		cout << "失败！失败代码是 " << WSAGetLastError() << endl;
		return INVALID_SOCKET;
	}
	//给socket绑定ip地址和端口号
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;//和创建socket时必须一样
	addr.sin_port = htons(PORT);//define htons 本地字节转网络字节
	addr.sin_addr.S_un.S_addr = ADDR_ANY;//网卡的任意ip//inet_addr("127.0.0.1");//转为二进制
	if (bind(so, (struct sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR)
	{
		cout << "失败！失败代码是 " << WSAGetLastError() << endl;
		return false;
	}
	//监听
	listen(so, 100);
	return so;
	

}
int main()
{
	InitSocket();
	SOCKET server= CreateSeverSocket();
	//等待客户端连接
	cout << "等待客户端连接..." << endl;
	//如果有客户端请求连接
	SOCKET client=accept(server,NULL,NULL);
	if (INVALID_SOCKET==client)
	{
		cout << "失败！失败代码是 " << WSAGetLastError() << endl;
	}
	//可以和客户端进行通信了
	//从指定的socket接收
	char revBuf[BUFSIZ] = { 0 };
	char sendBuf[BUFSIZ] = { 0 };
	while (true)
	{
		if (recv(client, revBuf, BUFSIZ, 0) > 0)
		{
			cout << "接收的信息是:" << revBuf << endl;
		}
		//发送消息
		cout << "发送消息：";
		memset(sendBuf, 0, sizeof(sendBuf));
		gets_s(sendBuf, BUFSIZ);
		if (SOCKET_ERROR == send(client, sendBuf, strlen(sendBuf), 0))
		{
			cout << "失败！失败代码是 " << WSAGetLastError() << endl;
		}
	}
	closesocket(client);
	closesocket(server);
	CloseSocket();
	cout << "------------------连接关闭-----------------" << endl;
}