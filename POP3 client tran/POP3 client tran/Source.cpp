#include<winsock.h>
#include<iostream>
#include<string>
#include<vector>
#include<map>
#pragma comment(lib, "wsock32.lib")

#define BUF_SIZE 1024

using namespace std;
void send(SOCKET sock, string s)
{
	send(sock, s.c_str(), s.length() + 1, 0);
	cout << "Saljemo:" << s << endl;
}
void send(SOCKET sock, string s, string a)
{
	s = s + " " + a;
	send(sock, s.c_str(), s.length() + 1, 0);
	cout << "Saljemo:" << s << endl;
}
map<string, string> recive1(SOCKET sock)
{
	map<string, string> mapa;
	vector<char> echoBuf(BUF_SIZE);
	recv(sock, echoBuf.data(), BUF_SIZE, 0);
	string s = echoBuf.data();
	int n = stoi(s.substr(4, 1));
	for (int i = 0; i < n; i++)
	{
		recv(sock, echoBuf.data(), BUF_SIZE, 0);
		
	}


}
int main()
{
	WSAData wsa;
	WSAStartup(0x0202, &wsa);

	sockaddr_in server;

	server.sin_addr.s_addr = inet_addr("127.0.0.1");
	server.sin_family = AF_INET;
	server.sin_port = htons(110);

	SOCKET sock;
	while (true)
	{
		sock = socket(AF_INET, SOCK_STREAM, 0);
		connect(sock, (sockaddr*)&server, sizeof(server));

		send(sock, "list");
	}

	return 0;
}