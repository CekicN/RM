#include<iostream>
#include<winsock.h>
#include<string>

#pragma comment(lib, "wsock32.lib")
int main()
{
	WSAData wsa;
	WSAStartup(0x0202, &wsa);
	sockaddr_in server;
	server.sin_family = AF_INET;
	server.sin_port = htons(80);
	server.sin_addr.s_addr = inet_addr("160.99.12.87");

	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	connect(sock, (sockaddr*)&server, sizeof(server));

	char sendBuf[] = "GET /cenovnik.html HTTP1.0\n\n";
	char recv_data[1024];
	ZeroMemory(recv_data, 1024);

	send(sock, sendBuf, strlen(sendBuf) + 1, 0);
	recv(sock, recv_data, 1024, 0);
	//HTTP/1.0 400
	char p[10];
	int j = 0;
	for (int i = 9; i < 12; i++)
	{
		p[j++] = recv_data[i];
	}
	p[j] = '\0';
	if (strcmp(p, "200") == 0)
	{
		printf("VRACENO");
		char* temp = strstr(recv_data, "Last-Modified: ");
		while (*temp != '\n')printf("%c", *temp++);
		printf("\n");
	}
	else
		printf("NIJE VRACENO");
	
	return 0;
}