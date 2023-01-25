#include<iostream>
#include<fcntl.h>
#include<string>
#include<stdlib.h>
#include<io.h>
#include<fstream>
using namespace std;

void rezultat(string s, string a);
void parse(string str)
{
	int pos1 = str.find("indeks=", 0);
	int pos2 = str.find("&", pos1 + 7);
	string indeks = str.substr(pos1 + 7, pos2 - pos1 - 7);

	pos1 = str.find("ext=", pos2);
	string ext = str.substr(pos1 + 4);
	rezultat(indeks, ext);
}

void rezultat(string s, string a)
{
	s = s + "." + a;
	ifstream f(s, ios::ate | ios::binary | ios::in);
	if (f.good())
	{
		int size = f.tellg();
		size++;
		cout << "Content-Type: image/" << a << "\n";
		cout << "Content-Length: " << size << "\n";
		cout << "Connection: Keep-Alive\n\n";
		f.seekg(ios_base::beg);
		_setmode(_fileno(stdout), _O_BINARY);
		char c;
		while (!f.eof())
		{
			f.read(&c, 1);
			cout << c;
		}
	}
	f.close();

}
int main()
{
	string req = getenv("REQUEST_METHOD");
	string q_string = getenv("QUERY_STRING");
	string postInputString;

	if (req == "GET")
	{
		if (!q_string.empty())
		{
			parse(q_string);
		}
	}
	else if (req == "POST")
	{
		if (!q_string.empty())
		{
			parse(q_string);
		}

		int br_byte = atoi(getenv("CONTENT_LENGTH"));

		if (br_byte > 0)
		{
			cin >> postInputString;
			parse(postInputString);
		}
	}

	return 0;
}