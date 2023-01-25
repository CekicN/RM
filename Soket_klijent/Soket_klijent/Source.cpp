#include<winsock.h>
#pragma comment(lib, "wsock32.lib")
#include<iostream>
#include<string>
using namespace std;

#define BUF_SIZE 1024
#define SERVER_PORT 8888
void ExitWithError(const string& message)
{
	cout << message << WSAGetLastError() << endl;
	WSACleanup();
	exit(1);
}
void printMessage(char* message, int len)
{
	cout << "Poslata poruka: ";
	for (int i = 0; i < len; i++)
	{
		cout << message[i];
	}
	cout << endl;
}

int main()
{
	WSAData wsa;
	SOCKET clientSock;
	if (WSAStartup(0x0202, &wsa) != 0)
	{
		ExitWithError("wsa failed");
	}
	if ((clientSock = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
	{
		ExitWithError("listenint socket failed");
	}
	sockaddr_in server;
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = inet_addr("127.0.0.1");//inet_addr izvlaci tacnu adresu iz ip adrese u tackastoj notaciji 127.0.0.
	server.sin_port = htons(SERVER_PORT);

	if (connect(clientSock,(struct sockaddr*)&server, sizeof(server)) == SOCKET_ERROR)
	{
		ExitWithError("connect() failed");
	}
	while (true)
	{
		string echoBuf;
		getline(cin, echoBuf);

		if (send(clientSock,echoBuf.c_str(),echoBuf.length(), 0) != echoBuf.length())
		{
			ExitWithError("send() failed");
		}
		int recvMsgsize = 0;
		char* buff = (char*)malloc(BUF_SIZE);
		do
		{
			if ((recvMsgsize = recv(clientSock, buff, BUF_SIZE, 0)) < 0)
			{
				ExitWithError("recv() failed");
			}
			printMessage(buff, recvMsgsize);
		} while (recvMsgsize == BUF_SIZE);
		free(buff);
	}
	closesocket(clientSock);
	WSACleanup();
	
	return 0;
}