#include <winsock.h>
#include <stdio.h>
#include <string.h>
#pragma comment(lib, "wsock32.lib")

int main()
{
	WSAData wsa;
	WSAStartup(0x0202, &wsa);

	SOCKET sock;
	sockaddr_in server; //specifikacija lokalne ili udaljene tacke u komunikaciji

	if( (sock = socket(AF_INET, SOCK_STREAM, 0)) == SOCKET_ERROR )
	{
		closesocket(sock);
		WSACleanup();
		exit(1);
	}

	server.sin_family = AF_INET;
	server.sin_port = htons(80);
	server.sin_addr.s_addr = inet_addr("160.99.14.201"); // ip za elfak.ni.ac.rs

	//uspostavlja vezu datog soketa sa udaljenom tackom koja se specificira sockaddr strukturom
	if( connect(sock,(sockaddr*)&server,sizeof(sockaddr) ) == SOCKET_ERROR )
	{
		closesocket(sock);
		WSACleanup();
		exit(1);
	}
	
	char send_data[] ="GET / HTTP/1.0\n\n";
	char recv_data[1024];
	ZeroMemory(recv_data,1024);

	send(sock,send_data,strlen(send_data),0);
	recv(sock,recv_data,1024,0);

	char* temp = strstr(recv_data, "Last-Modified: ");
	if(temp != NULL)
	{
		while(*temp != '\n') printf("%c", *temp++); 
		printf("\n");
	}
	else
		printf("No such information!\n");

	closesocket(sock);
	WSACleanup();

	return 0;
}