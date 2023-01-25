#include<winsock.h>
#pragma comment(lib, "wsock32.lib")

#include<iostream>
#include<string>

using namespace std;

#define BUF_SIZE 1024
#define SERVER_PORT 8888

void ExitWithMessage(const string& message)
{
	cout << message << WSAGetLastError() << endl;
	WSACleanup();
	exit(1);
}

void printMessage(char* msg, int len)
{
	cout << "Primljena poruka: ";
	for (int i = 0; i < len; i++)
	{
		cout << msg[i];
	}
	cout << endl;
}
int main()
{
	WSAData wsa;
	SOCKET serverSock;
	if (WSAStartup(0x0202, &wsa) != 0)
	{
		ExitWithMessage("Startup failed");
	}
	if ((serverSock = socket(AF_INET, SOCK_DGRAM, 0)) == INVALID_SOCKET)
	{
		ExitWithMessage("Listenint socket failed");
	}
	sockaddr_in server;
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(SERVER_PORT);
	if (bind(serverSock, (struct sockaddr*)&server, sizeof(server)) == SOCKET_ERROR)
	{
		ExitWithMessage("bind() ");
	}

	sockaddr_in client;
	int addrlen = sizeof(sockaddr);
	while (true)
	{
		char echoBuf[BUF_SIZE];
		int recvMsgSize = recvfrom(serverSock, echoBuf, BUF_SIZE, 0, (sockaddr*)&client, &addrlen);

		if (recvMsgSize < 0)
		{
			ExitWithMessage("recv()");
		}
		printMessage(echoBuf, recvMsgSize);
		if (sendto(serverSock, echoBuf, recvMsgSize, 0, (sockaddr*)&client, addrlen) != recvMsgSize)
		{
			ExitWithMessage("send()");
		}
	}
	closesocket(serverSock);
	WSACleanup();
	return 0;
}