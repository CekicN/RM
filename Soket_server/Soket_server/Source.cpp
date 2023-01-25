#include<winsock.h>
#pragma comment(lib, "wsock32.lib")

#include<iostream>
#include<string>
#include<vector>
#define SERVER_PORT 8888
#define BUF_SIZE 1024
using namespace std;

void ExitWithError(const string& message)
{
	cout << message << "Error code:" << WSAGetLastError() << endl;
	WSACleanup();
	exit(1);
}
void HandleError(const string& message)
{
	cout << message << "Error code:" << WSAGetLastError() << endl;
}
void printMessage(char* message, int len)
{
	cout << "Primljena poruka:";
	for (auto i = 0; i < len; i++)
	{
		cout << message[i];
	}
	cout << endl;
}
int main()
{
	WSAData wsa;
	SOCKET listensock;
	if (WSAStartup(0x0202, &wsa) != 0)
		ExitWithError("Startup Failed");

	if ((listensock = socket(AF_INET,SOCK_STREAM, 0)) == INVALID_SOCKET)
	{
		ExitWithError("Listening socket not created");
	}
	/*
		htons - host to netwwork short 16b
		htonl - host to network long 32b
		ntohs - obrnuto
		ntohl - obrnuto
	*/

	//Dodela porta listen socketu
	sockaddr_in server;
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(SERVER_PORT);
	//Bindovanje
	if (bind(listensock, (struct sockaddr*)&server, sizeof(server)) == SOCKET_ERROR)
	{
		ExitWithError("bind() failed");
	}

	//Listen osluskuje dolazece konekcije
	if (listen(listensock, 3) < 0)
	{
		ExitWithError("listen() failed");
	}

	while (true)
	{
		sockaddr_in client;
		int clen = sizeof(struct sockaddr_in);
		SOCKET clientSock;

		if ((clientSock = accept(listensock,(struct sockaddr*)&client,&clen)) < 0)
		{
			HandleError("accept() failed");
		}

		//Primanje poruke od klijenta
		char* echoBuf = (char*)malloc(BUF_SIZE);
		//recv vraca broj procitanih bajtova
		int recvMsgSize = recv(clientSock, echoBuf, BUF_SIZE, 0);
		if (recvMsgSize < 0)
		{
			HandleError("recv() failed");
		}

		printMessage(echoBuf, recvMsgSize);

		while (recvMsgSize > 0)
		{
			if (send(clientSock, echoBuf, recvMsgSize, 0) != recvMsgSize)
			{
				HandleError("send() failed");
			}
			if ((recvMsgSize = recv(clientSock, echoBuf, BUF_SIZE, 0)) < 0)
			{
				HandleError("recv() failed");
			}
			printMessage(echoBuf, recvMsgSize);
		}
		closesocket(clientSock);
		free(echoBuf);

	}
	closesocket(listensock);
	WSACleanup();

	return 0;
}