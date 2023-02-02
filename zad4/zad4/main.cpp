#include <winsock.h>
#include <stdio.h>
#include <string.h>
#pragma comment(lib, "wsock32.lib")

int main()
{
	WSAData wsa;
	WSAStartup(0x0202, &wsa);

	SOCKET sock;
	sockaddr_in server;

	if((sock = socket(AF_INET, SOCK_STREAM, 0)) == SOCKET_ERROR)
	{
		closesocket(sock);
		WSACleanup();
		exit(1);
	}

	server.sin_family = AF_INET;
	server.sin_port = htons(80);
	server.sin_addr.s_addr = htonl(INADDR_ANY);

	if(bind(sock, (sockaddr*)&server, sizeof(sockaddr)) == SOCKET_ERROR)
	{
		closesocket(sock);
		WSACleanup();
		exit(1);
	}

	if(listen(sock, 100) == SOCKET_ERROR)
	{
		closesocket(sock);
		WSACleanup();
		exit(1);
	}

	while(true)
	{
		SOCKET comSock;
		sockaddr_in client;

		int addr_len = sizeof(sockaddr);
		if((comSock = accept(sock, (sockaddr*)&client, &addr_len)) == SOCKET_ERROR) 
		{
			closesocket(sock);
			WSACleanup();
			exit(1); 
		}

		char sendBuf[1024];
		char recvBuf[1024];

		ZeroMemory(sendBuf, 1024);
		ZeroMemory(recvBuf, 1024);

		int bytes_send;
		int bytes_recv;

		bytes_recv = recv(comSock, recvBuf, 1024, 0);
		if(bytes_recv < 0)
		{
			closesocket(sock);
			WSACleanup();
			exit(1);
		}

		printf("%s\n", recvBuf);

		char fileName[25];
		int j=0;
		char* t=strstr(recvBuf,"/");
		while(t != nullptr && *t != ' ') fileName[j++] = *t++;
		fileName[j] = '\0';

		char path[100];
		ZeroMemory(path, 100);
		strcat(path, "C:\\Users\\Djordje\\Desktop\\lab5 od Darka\\zad4\\"); //ovde navodite putanju gde server cuva txt fajl...
		strcat(path, fileName);

		char temp[100];
		char body[1024];
		char body_len[10];
		ZeroMemory(temp, 100);
		ZeroMemory(body, 1024);
		FILE* f = fopen(path, "r");

		if(f != NULL)
		{
			while(!feof(f))
			{
				fgets(temp, 100, f);
				strcat(body, temp);
				ZeroMemory(temp, 100);
			}
			strcat(sendBuf, "HTTP/1.1 200 OK\n");
			_itoa(strlen(body), body_len, 10);
			strcat(sendBuf, "Content-Length: ");
			strcat(sendBuf, body_len);
			strcat(sendBuf, "\n");
			strcat(sendBuf, "Content-Type: text/plain\n\n");
			strcat(sendBuf, body);

			bytes_send = send(comSock, sendBuf, strlen(sendBuf)+1, 0);
			if(bytes_send < 0)
			{
				closesocket(sock);
				WSACleanup();
				exit(1);
			}

			fclose(f);
			closesocket(comSock); //ovo ne treba zaboraviti!
		}
		else
		{
			if(strstr(fileName, ".txt") != NULL) //proverava ekstenziju zahtevanog fajla...
			{

				strcat(sendBuf, "HTTP/1.1 404 Not Found\nContent-Length: 26\nContent-Type: text/plain\n\n404 Error: File not Found!");
				bytes_send = send(comSock, sendBuf, strlen(sendBuf), 0);
				if(bytes_send < 0)
				{
					closesocket(sock);
					WSACleanup();
					exit(1);
				}
			}
			else
			{
				strcat(sendBuf, "HTTP/1.1 400 Bad Request\nContent-Length: 23\nContent-Type: text/plain\n\n400 Error: Bad request!");
				bytes_send = send(comSock, sendBuf, strlen(sendBuf), 0);
				if(bytes_send < 0)
				{
					closesocket(sock);
					WSACleanup();
					exit(1);
				}
			}

			closesocket(comSock); //ovo ne treba zaboraviti!
		}
	}
	
	closesocket(sock);
	WSACleanup();

	return 0;
}