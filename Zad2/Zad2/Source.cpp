#include<iostream>
#include<string>
#include<fstream>
#include<io.h>
#include<fcntl.h>
using namespace std;
void PrikaziSliku(int br);
void parse(string str)
{
	int pos1 = str.find("broj=", 0);
	string val = str.substr(pos1 + 5);
	int ival = stoi(val);


	PrikaziSliku(ival);
}

void PrikaziSliku(int br)
{
	if (br % 2 != 0)
	{
		ifstream file("Slike/slika1.png", ios::ate | ios::in | ios::binary);

		if (file.good())
		{
			cout << "Content-Type: image/png\n";
			int len = file.tellg();
			len++;
			cout << "Content_Length: " << len << "\n";
			cout << "Connection: Keep-Alive\n\n";
			file.seekg(ios_base::beg);
			_setmode(_fileno(stdout), _O_BINARY);
			char buf;

			while (!file.eof())
			{
				file.read(&buf, 1);
				printf("%c", buf);
			}
		}
		file.close();
	}
	else
	{
		ifstream file("Slike/slika1.png", ios::ate | ios::in | ios::binary);

		if (file.good())
		{
			cout << "Content-Type: image/png\n";
			int len = file.tellg();
			len++;
			cout << "Content_Length: " << len << "\n";
			cout << "Connection: Keep-Alive\n\n";
			file.seekg(ios_base::beg);
			_setmode(_fileno(stdout), _O_BINARY);
			char buf;

			while (!file.eof())
			{
				file.read(&buf, 1);
				printf("%c", buf);
			}
		}
		file.close();
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