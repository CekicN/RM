#include<iostream>
#include<string>

using namespace std;
void parse(string q)
{
	cout << "Content-Type: text/html\n\n";
	cout << "<html> <head>\n";
	cout << "<title>Rezultat</title>\n";
	cout << "<body>\n";
	cout << "<h1>"<< q <<"</h1>\n";
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