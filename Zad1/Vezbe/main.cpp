#include<iostream>
#include<string.h>
#include<string>
#include<stdlib.h>

using namespace std;
int res;
void rezultat(int x, int y);
void parse(string str)
{
	int pos1 = str.find("slika.x=", 0);
	int pos2 = str.find("&", pos1 + 7);
	int x = stoi(str.substr(pos1 + 8, pos2 - pos1 - 8));


	int pos1 = str.find("slika.y=", pos2);
	int y = stoi(str.substr(pos1 + 8));

	rezultat(x, y);
}

void rezultat(int x, int y)
{

	cout << "Content-Type: text/html\n\n";
	cout << "<html> <head>\n";
	cout << "<title>Rezultat</title>\n";
	cout << "<body>\n";
	cout << "<h1>Zadatak 1 Rezultat</h1>\n";
	cout << "<p>" << x << ", " << y << "</p>\n";
	cout << "</body></html>\n";


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
	else if(req == "POST")
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