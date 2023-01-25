#include<iostream>
#include<stdio.h>
#include<string>
#include<vector>
#include<winsock.h>

#pragma comment(lib, "wsock32.lib")
using namespace std;

#define BUF_SIZE 1024

void send(SOCKET clientSock, string s)
{

	cout << s << endl;
	send(clientSock, s.c_str(), s.length() + 1, 0);
}
string recive(SOCKET clientSock)
{
	vector<char> echoBuf(BUF_SIZE);
	recv(clientSock, echoBuf.data(), BUF_SIZE, 0);

	string s = echoBuf.data();
	cout << "Primio: " << s << endl;
	s = s.substr(0, 3);
	return s;
}

void sendCommand(SOCKET clientSock, string s)
{
	string sParam = "";
	cout << "Unesite: " << endl;
	getline(cin, sParam);
	s = s + " " + sParam;
	send(clientSock, s);

}
int main()
{
	WSAData wsa;
	WSAStartup(0x0202, &wsa);

	sockaddr_in server;
	server.sin_addr.s_addr = inet_addr("127.0.0.1");
	server.sin_family = AF_INET;
	server.sin_port = htons(25);

	SOCKET clientSock;

	while (true)
	{
		clientSock = socket(AF_INET, SOCK_STREAM, 0);
		connect(clientSock, (sockaddr*)&server, sizeof(server));
		string s = recive(clientSock);//PRIMA 220
		while (s != "220")
		{
			send(clientSock, "Error");
			s = recive(clientSock);
		}

		sendCommand(clientSock, "HELO");//SALJE HELO

		s = recive(clientSock); //PRIMA 250
		while (s != "250")
		{
			send(clientSock, "Error");
			s = recive(clientSock);
		}
		sendCommand(clientSock, "MAIL FROM:");
		s = recive(clientSock); //PRIMA 250
		while (s != "250")
		{
			send(clientSock, "Error");
			s = recive(clientSock);
		}
		sendCommand(clientSock, "RCPT TO:");
		s = recive(clientSock); //PRIMA 250
		while (s != "250")
		{
			send(clientSock, "Error");
			s = recive(clientSock);
		}
		send(clientSock, "DATA");
		s = recive(clientSock); //PRIMA 354
		while (s != "354")
		{
			send(clientSock, "Error");
			s = recive(clientSock);
		}

		char data[BUF_SIZE];
		strcpy(data, "");
		char tmp[100];
		char provera[5];
		do
		{
			fgets(tmp, 100, stdin);
			strcat(data, tmp);
			int l = strlen(data) - 3;
			int j = 0;
			for (int i = l; i <= strlen(data); i++)
			{
				provera[j++] = data[i];
			}


		} while (strcmp(provera, "\n.\n"));
		send(clientSock, data);

		s = recive(clientSock); //PRIMA 250
		while (s != "250")
		{
			send(clientSock, "Error");
			s = recive(clientSock);
		}
		//send(clientSock, "QUIT");

		//s = recive(clientSock); //PRIMA 221
		//while (s != "221")
		//{
		//	send(clientSock, "Error");
		//	s = recive(clientSock);
		//}

		//break;

	}
	closesocket(clientSock);
	WSACleanup();
	return 0;
}