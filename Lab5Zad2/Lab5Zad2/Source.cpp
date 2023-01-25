#include<winsock.h>
#pragma comment(lib, "wsock32.lib")

#include<iostream>
using namespace std;

int main()
{
	WSAData wsa;
	SOCKET sock;

	WSAStartup(0x0202, &wsa);

	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == SOCKET_ERROR)
	{
		closesocket(sock);
		WSACleanup();
		exit(1);
	}

	sockaddr_in server;

	server.sin_family = AF_INET;
	server.sin_addr.s_addr = inet_addr("160.99.14.201");
	server.sin_port = htons(80);
	if (connect(sock, (sockaddr *)&server,sizeof(server)) == SOCKET_ERROR)
	{
		closesocket(sock);
		WSACleanup();
		exit(1);
	}

	char send_data[] = "GET / HTTP/1.0\n\n";
	char recv_data[1024];
	ZeroMemory(recv_data, 1024);

	send(sock, send_data, strlen(send_data), 0);
	recv(sock, recv_data, 1024, 0);

	char* temp = strstr(recv_data, "Server: ");
	while (*temp != '\n')printf("%c", *temp++);
	printf("\n");


	closesocket(sock);
	WSACleanup();
	return 0;
}