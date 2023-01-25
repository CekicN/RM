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
	cout << "Poslata poruka: ";
	for (int i = 0; i < len; i++)
	{
		cout << msg[i];
	}
	cout << endl;
}

int main()
{
	WSAData wsa;
	SOCKET clientSock;
	if (WSAStartup(0x0202, &wsa) != 0)
	{
		ExitWithMessage("Startup failed");
	}
	if ((clientSock = socket(AF_INET, SOCK_DGRAM, 0)) == INVALID_SOCKET)
	{
		ExitWithMessage("Invalid socket");
	}

	sockaddr_in server;
	server.sin_family = AF_INET;;
	server.sin_addr.s_addr = inet_addr("127.0.0.1");
	server.sin_port = htons(8888);

	string echoBuf;
	int size = sizeof(sockaddr);

	while (true)
	{
		getline(cin, echoBuf);
		if (sendto(clientSock, echoBuf.c_str(),echoBuf.length(),0,(sockaddr*)&server, size) != echoBuf.length())
		{
			ExitWithMessage("send()");
		}
		
		int recvMsgSize = 0;
		char buff[BUF_SIZE];
		if ((recvMsgSize = recvfrom(clientSock,buff, BUF_SIZE, 0, (sockaddr*)&server, &size)) < 0)
		{
			ExitWithMessage("recv()");
		}
		printMessage(buff, recvMsgSize);
	}
	closesocket(clientSock);
	WSACleanup();
	return 0;
}