#include<winsock.h>
#pragma comment(lib, "wsock32.lib")
#include<string>
#include<iostream>
using namespace std;
void gasi(const string& msg )
{
	cout << msg << WSAGetLastError() << endl;
	WSACleanup();
	exit(1);
}

void printMsg(const char* msg)
{
	cout << "Poslata poruka" << msg << endl;
}
int main(int argc, char* argv[])
{
	WSAData wsa;
	SOCKET soket;

	if (WSAStartup(0x0202, &wsa) != 0)
	{
		gasi("wsa");
	}

	if ((soket = socket(AF_INET, SOCK_STREAM,0)) == INVALID_SOCKET)
	{
		gasi("Soket");
	}

	sockaddr_in server;
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = inet_addr("127.0.0.1");
	server.sin_port = htons(8888);

	if (connect(soket,(struct sockaddr*)&server, sizeof(server)) == SOCKET_ERROR)
	{
		gasi("connect");
	}

	char* ZaSlanje = new char[100];
	strcpy(ZaSlanje, "HELLO ");
	strcat(ZaSlanje, argv[1]);
	int s = strlen(ZaSlanje) + 1;
	if (send(soket,ZaSlanje, s, 0) != s)
	{
		gasi("send");
	}
	printMsg(ZaSlanje);
	char HI[1024];
	if ((s = recv(soket,HI, 1024, 0)) < 0)
	{
		gasi("recv");
	}
	cout<<"Primljeno: " << HI << endl;
	int a;

	while (true)
	{
		string sendBuf;
		getline(cin, sendBuf);
		a = sendBuf.length();
		if (send(soket, sendBuf.c_str(), a+1, 0) != a+1)
		{
			gasi("send");
		}
		printMsg(sendBuf.c_str());
		s = 0;
		char* recvBuf = new char[1024];
		do
		{

			if ((s = recv(soket, recvBuf, 1024, 0)) < 0)
			{
				gasi("recv");
			}
			cout << "Primljeno: " << recvBuf << endl;

		} while (s == 1024);
	}
	closesocket(soket);
	WSACleanup();
	return 0;
}