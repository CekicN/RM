#include<string>
#include<iostream>
#include<fstream>
#include<io.h>
#include<fcntl.h>
using namespace std;

void PrikaziSliku(string s);
void parse(string s)
{
	int poz1 = s.find("slika=", 0);
	string r = s.substr(poz1 + 6);
	
	PrikaziSliku(r);
}
void PrikaziSliku(string s)
{
	string r = "Slike/" + s;
	ifstream f(r, ios::ate | ios::binary | ios::in);
	if (f.good())
	{
		cout << "Content-Type: image/png\n";
		int len = f.tellg();
		len++;
		cout << "Content_Length: " << len << "\n";
		cout << "Connection: Keep-Alive\n\n";
		f.seekg(ios_base::beg);
		_setmode(_fileno(stdout), _O_BINARY);
		char c;
		while (!f.eof())
		{
			f.read(&c, 1);
			cout << c;
		}
		f.close();
	}
	else
	{
		cout << "Content-Type: text/html\n\n";
		cout << "<html> <head>\n";
		cout << "<title>Rezultat</title>\n";
		cout << "<body>\n";
		cout << "<p>\n";
		cout << "Slika nije pronadjena";
		cout << "</p></body></html>\n";
	}
}
int main()
{
	string req = getenv("REQUEST_METHOD");
	string q_string = getenv("QUERY_STRING");
	string InputString;

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

		int br_bytes = atoi(getenv("CONTENT_LENGTH"));

		if (br_bytes > 0)
		{
			cin >> InputString;
			parse(InputString);
		}
	}
	return 0;
}