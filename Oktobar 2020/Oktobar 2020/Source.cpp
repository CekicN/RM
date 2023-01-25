#include<iostream>
#include<vector>
#include<string>
#include<winsock.h>
#include<map>
#include<fstream>
#pragma comment(lib, "wsock32.lib")
using namespace std;
#define BUF_SIZE 1024
void send(SOCKET clientSock, string s)
{
	cout << s << endl;
	send(clientSock, s.c_str(), s.length() + 1, 0);
}
string reciv(SOCKET clientSock)
{
	vector<char> echoBuf(BUF_SIZE);
	recv(clientSock, echoBuf.data(), BUF_SIZE, 0);

	string s = echoBuf.data();
	cout << "Pirmio: " << s << endl;
	return s;
}
map<string, string> recive(SOCKET clientSock, string tr)
{
	string s = reciv(clientSock);

	string a = s.substr(0, tr.length());
	s = s.substr(tr.length() + 1);
	map<string, string> mapa;
	mapa[a] = s;
	return mapa;
}

void sendCommand(SOCKET clientSock, string s)
{
	string sParam = "";
	cout << "Unesite: ";
	getline(cin, sParam);
	s = s + " " + sParam;
	send(clientSock, s);

}
void sendCommand(SOCKET clientSock, string h, string a)
{
	string s = h + " " + a;
	send(clientSock, s);
}
int main()
{
	WSAData wsa;
	WSAStartup(0x0202, &wsa);

	sockaddr_in server;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(25);
	server.sin_family = AF_INET;

	SOCKET listenSock = socket(AF_INET, SOCK_STREAM, 0);
	bind(listenSock, (sockaddr*)&server, sizeof(server));
	listen(listenSock, 3);

	map<string, string> mainMap;
	map<string, string> tmpMap;
	int z = 1;
	while (true)
	{
		sockaddr_in client;
		int clen = sizeof(sockaddr_in);
		SOCKET comSock = accept(listenSock, (sockaddr*)&client, &clen);
		
		sendCommand(comSock, "220");
		tmpMap = recive(comSock, "HELO");
		while (tmpMap["HELO"] == "")
		{
			send(comSock, "Error");
			tmpMap = recive(comSock, "HELO");
		}
		mainMap["HELO"] = tmpMap["HELO"];
		string s = "Hello " + mainMap["HELO"] + ", pleased to meet you";
		sendCommand(comSock, "250", s);

		tmpMap = recive(comSock, "MAIL FROM:");
		while (tmpMap["MAIL FROM:"] == "")
		{
			send(comSock, "Error");
			tmpMap = recive(comSock, "MAIL FROM:");
		}
		mainMap["MAIL FROM:"] = tmpMap["MAIL FROM:"];
		s = mainMap["MAIL FROM:"] + "Sender ok";
		sendCommand(comSock, "250", s);

		tmpMap = recive(comSock, "RCPT TO:");
		while (tmpMap["RCPT TO:"] == "")
		{
			send(comSock, "Error");
			tmpMap = recive(comSock, "RCPT TO:");
		}
		mainMap["RCPT TO:"] = tmpMap["RCPT TO:"];
		s = mainMap["RCPT TO:"] + "Racipient ok";
		sendCommand(comSock, "250", s);

		if (reciv(comSock) == "DATA")
		{
			s = "Enter mail, end with . on a line by itself";
			sendCommand(comSock, "354", s);
		}

		char b[5];
		sprintf(b, "%d", z);
		z++;
		string naziv = mainMap["MAIL FROM:"] + "_" + mainMap["RCPT TO:"] + "_" + b + ".txt";
		ofstream f(naziv.c_str(), ios::out);
		string data = reciv(comSock);
		s = "Message accepted for delivery";
		if (f.good())
		{
			f << mainMap["MAIL FROM:"] << endl;
			f << mainMap["RCPT TO:"] << endl;
			f << data << endl;
		}
		f.close();
		sendCommand(comSock, "250", s);
		//string q = reciv(comSock);

		//if (q == "QUIT")
		//{
		//	sendCommand(comSock, "221", "closing connection");
		//	closesocket(comSock);
		//	break;
		//}

	}
	closesocket(listenSock);
	WSACleanup();
	return 0;
}