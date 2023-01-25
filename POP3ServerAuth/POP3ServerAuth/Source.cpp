#include<iostream>
#include<winsock.h>
#include<map>
#include<vector>

#pragma comment(lib, "wsock32.lib")
using namespace std;

#define BUF_SIZE 1024

void send(SOCKET clientSock, string s)
{
	send(clientSock, s.c_str(), s.length() + 1, 0);
	cout << "Saljem: " << s << endl;
}
string recive(SOCKET clientSock)
{
	vector<char> echoBuf(BUF_SIZE);
	recv(clientSock, echoBuf.data(), BUF_SIZE, 0);
	string s = echoBuf.data();
	cout << "Primio: " << s << endl;
	s = s.substr(s.find(' ') + 1);
	return s;
}
int main()
{
	map<string, string> mapUsernameAndPassword;
	mapUsernameAndPassword["Nikola"] = "Nikola";
	mapUsernameAndPassword["admin"] = "admin";

	WSAData wsa;
	WSAStartup(0x0202, &wsa);
	sockaddr_in server;
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(110);

	SOCKET listenSock = socket(AF_INET, SOCK_STREAM, 0);

	bind(listenSock, (sockaddr*)&server, sizeof(server));
	listen(listenSock, 3);

	while (true)
	{
		sockaddr_in client;
		int len = sizeof(struct sockaddr_in);
		SOCKET clientSock = accept(listenSock, (sockaddr*)&client, &len);
		const string sOk = "+OK";
		const string sERR = "-ERR";
		send(clientSock, sOk);

		string sUsername = recive(clientSock);

		while (mapUsernameAndPassword[sUsername] == "")
		{
			send(clientSock, sERR);
			sUsername = recive(clientSock);
		}
		send(clientSock, sOk);
		string pass = recive(clientSock);
		while (pass == "" || mapUsernameAndPassword[pass] == "")
		{
			send(clientSock, sERR);
			pass = recive(clientSock);
		}
		send(clientSock, sOk);
		closesocket(clientSock);
	}

	closesocket(listenSock);
	WSACleanup();
	return 0;
}
