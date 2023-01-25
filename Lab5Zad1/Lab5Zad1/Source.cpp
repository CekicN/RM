#include<winsock.h>
#pragma comment(lib, "wsock32.lib")

#include<iostream>
#include<string>
#include<vector>
#define BUF_SIZE 20000
using namespace std;

void Gasi(const string& msg)
{
	cout << msg << WSAGetLastError() << endl;
	WSACleanup();
	exit(1);
}
int main()
{
	WSAData wsa;
	SOCKET soket;

	if (WSAStartup(0x0202, &wsa) != 0)
	{
		Gasi("wsa");
	}

	if ((soket = socket(AF_INET,SOCK_STREAM, 0)) == INVALID_SOCKET)
	{
		Gasi("sock");
	}

	sockaddr_in server;
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(8000);

	if (bind(soket,(struct sockaddr*)&server, sizeof(sockaddr)) == SOCKET_ERROR)
	{
		Gasi("bind");
	}
	
	if (listen(soket, 100) == SOCKET_ERROR)
	{
		Gasi("listen");
	}

	while (true)
	{
		SOCKET comSoket;
		sockaddr_in client;
		FILE* f;

		int addr_len = sizeof(sockaddr);
		comSoket = accept(soket, (struct sockaddr*)&client, &addr_len);
		if (comSoket == SOCKET_ERROR)
		{
			Gasi("comSoket");
		}

		char* recvBuf = (char*)malloc(BUF_SIZE);
		char* sendBuf = new char[BUF_SIZE];
		ZeroMemory(recvBuf, BUF_SIZE);

		int bytes_send;
		int bytes_recv;

		bytes_recv = recv(comSoket, recvBuf, BUF_SIZE, 0);
		if (bytes_recv < 0)
		{
			Gasi("recv");
		}
		cout << recvBuf << endl;

		char* fileName = (char*)malloc(25);
		char* t = strstr(recvBuf, "/");
		int j = 0;
		while (*t != ' ')fileName[j++] = *t++;
		fileName[j] = '\0';

		char* path = (char*)malloc(100);
		ZeroMemory(path, 100);
		strcat(path, "C:\\Users\\Nikola\\Desktop\\Faks\\3.godina\\Mreze\\Vezbe\\Lab5Zad1\\");

		char file[25];
		int i = 1;
		while (fileName[i] != '\0')
		{
			file[i - 1] = fileName[i];
			i++;
		}
		file[i - 1] = '\0';
		char ext[5];
		int p = strlen(file) - 3;
		int q = 0;
		for (; file[p] != '\0'; p++)
		{
			ext[q++] = file[p];
		}
		ext[q] = '\0';
		char temp[BUF_SIZE];
		vector<char> body;
		char body_len[10];

		ZeroMemory(temp, 1024);
		
		cout << "FILE " << file << endl;
		strcat(path, file);
		cout << "PATH " << path << endl;

		f = fopen(path, "rb");

		if (f != NULL)
		{
			fseek(f, 0, ios::end);
			int size = ftell(f);
			fseek(f, 0, ios::beg);
			char buf[10];
			sprintf(buf, "%d", size);

			ZeroMemory(sendBuf, BUF_SIZE);
			char c;
			while (!feof(f))
			{
				fscanf(f, "%c", &c);
				body.push_back(c);
			}

			FILE* fcpy = fopen("C:\\Users\\Nikola\\Desktop\\Faks\\3.godina\\Mreze\\Vezbe\\Lab5Zad1\\cpy.png", "wb");
			if (fcpy)
			{
				fwrite(body.data(), 1, size, fcpy);
				fclose(fcpy);
			}
			strcat(sendBuf, "HTTP/1.0 200 OK\r\n");
			strcat(sendBuf, "Content-Type: image/");
			strcat(sendBuf, ext);
			strcat(sendBuf, "\r\n");
			strcat(sendBuf, "Content-Length: ");
			strcat(sendBuf, buf);
			strcat(sendBuf, "\r\n\r\n");

			send(comSoket, sendBuf, strlen(sendBuf), 0);
			bytes_send = send(comSoket, body.data(), body.size(), 0);
			if (bytes_send < 0)
			{
				Gasi("send");
			}
			fclose(f);
			closesocket(comSoket);

		}
		else
		{
				strcat(sendBuf, "HTTP/1.1 400 Bad Request\nContent-Length: 23\nContent-Type: text/plain\n\n400 Error: Bad request!");
				bytes_send = send(comSoket, sendBuf, strlen(sendBuf) + 1, 0);
				if (bytes_send < 0)
				{
					Gasi("send");
				}

			closesocket(comSoket);
		}
		break;
	}

	closesocket(soket);
	WSACleanup();
	return 0;
}