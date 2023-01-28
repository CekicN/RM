#include<winsock.h>
#pragma comment(lib, "wsock32.lib")

#include<iostream>

using namespace std;

void gasi(const string& msg)
{
	cout << msg << WSAGetLastError() << endl;
	WSACleanup();
	exit(1);
}
void PrimljenaPoruka(const char* msg)
{
	cout<< "Primljena poruka:" << msg << endl;
}
int main()
{
	WSAData wsa;
	SOCKET wsock;

	if (WSAStartup(0x0202, &wsa))
	{
		gasi("wsa");
	}

	if ((wsock = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
	{
		gasi("wsoket");
	}

	sockaddr_in server;
	server.sin_family = AF_INET;
	server.sin_port = htons(8888);
	server.sin_addr.s_addr = INADDR_ANY;

	if (bind(wsock, (struct sockaddr*)&server, sizeof(server)) == SOCKET_ERROR)
	{
		gasi("bind");
	}

	if (listen(wsock,3) < 0)
	{
		gasi("listen");
	}
	int size;
	while (true)
	{
		SOCKET comSock;
		sockaddr_in client;
		int size = sizeof(struct sockaddr_in);
		if ((comSock = accept(wsock,(struct sockaddr*)&client, &size)) < 0)
		{
			gasi("accept");
		}

		char recvBuf[1024];
		
		if ((size = recv(comSock, recvBuf, 1024, 0)) < 0)
		{
			gasi("recv");
		}
		PrimljenaPoruka(recvBuf);
		char naziv[20];
		int i, j;
		i = 0;
		j = 6;
		while (recvBuf[j] != '\0')
		{
			naziv[i++] = recvBuf[j++];
		}
		naziv[i] = '\0';

		char sendBuf[1024];
		strcpy(sendBuf, "HI ");
		strcat(sendBuf, naziv);

		if (send(comSock, sendBuf, strlen(sendBuf)+1, 0) != strlen(sendBuf) + 1)
		{
			gasi("send");
		}

		if ((size = recv(comSock, recvBuf, 1024, 0)) < 0)
		{
			gasi("recv");
		}
		recvBuf[size] = '\0';
		PrimljenaPoruka(recvBuf);
		i = 1;
		char br[5];
		

		while (size > 0)
		{
			sprintf(br, "%d", i);
			strcpy(sendBuf, "MSG - ");
			strcat(sendBuf, br);
			strcat(sendBuf, ": ");
			strcat(sendBuf, recvBuf);
			i++;

			if (send(comSock, sendBuf, strlen(sendBuf) + 1, 0) != strlen(sendBuf) + 1)
			{
				gasi("send");
			}
			if ((size = recv(comSock, recvBuf, 1024, 0)) < 0)
			{
				gasi("recv");
			}
			recvBuf[size] = '\0';
		}

	}

	return 0;
}
