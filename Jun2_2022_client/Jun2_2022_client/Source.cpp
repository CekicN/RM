#include<winsock.h>
#include<iostream>
#include<string>
#include<vector>

#pragma comment(lib, "wsock32.lib")
using namespace std;
#define BUF_SIZE 1024

void send(SOCKET clientSock, string s)
{
	send(clientSock, (char*)s.c_str(), s.length() + 1, 0);
	cout << "Saljem: " << s << endl;
}
string recive(SOCKET clientSock)
{
	vector<char> echoBuf(BUF_SIZE);
	recv(clientSock, echoBuf.data(), 1024, 0);
	string s = echoBuf.data();
	cout << "Primio: " << s << endl;
	return s;
}
bool sendCommand(SOCKET clientSock, string sCommand)
{
	string sParametar = "";
	cout << "Unesite: " << sCommand << ": ";
	getline(cin, sParametar);
	send(clientSock, sCommand + " " + sParametar);

	string sRes = recive(clientSock);
	int nInd = sRes.find("+OK");
	return nInd >= 0;
}
int main()
{
	WSAData wsa;
	SOCKET sock;
	WSAStartup(0x0202, &wsa);
	sockaddr_in server;
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = inet_addr("127.0.0.1");
	server.sin_port = htons(110);

	while (true)
	{
		sock = socket(AF_INET, SOCK_STREAM, 0);
		connect(sock, (sockaddr*)&server, sizeof(server));

		string sRes = recive(sock);
		int nInd = sRes.find("+OK");
		if (nInd >= 0)
		{
			while (!sendCommand(sock, "user"))
			{
				;//Korisnicko ime
			}
			while (!sendCommand(sock, "pass"))
			{
				;//sifra
			}
			cout << "Ulogovan si" << endl;
		}
		else {
			//server ne radi
		}

	}
	closesocket(sock);
	WSACleanup();
	return 0;
}