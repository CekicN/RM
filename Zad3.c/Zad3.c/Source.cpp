#include<iostream>
#include<string>
#include<stdlib.h>
using namespace std;

void Kliknuto(int x);
void parse(string str)
{
	int poz1 = str.find("x=", 0);
	int poz2 = str.find("&", poz1);

	string val = str.substr(poz1 + 2, poz2 - poz1 - 2);

	int x = stoi(val);
	Kliknuto(x);
}
void Kliknuto(int x)
{
	string boja = x > 438/2 ? "zuta":"crvena";
	string bo = x > 438 / 2 ? "yellow" : "red";
	cout << "Content-Type: text/html\n\n";
	cout << "<html> <head>\n";
	cout << "<title>Zadatak 3</title>\n";
	cout << "<body>\n";
	cout << "<h1>Zadatak 3</h1>\n";
	cout << "<h3 style = \"color:"<< bo <<"\";>Kliknuta je :" << boja << " boja!</h3>\n";
	cout << "</body></html>\n";
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