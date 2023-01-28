#include<iostream>
#include<string>

using namespace std;

void Prikazi(string food, string size, string amount);
void parse(string q)
{
	int pos1 = q.find("food=", 0);
	int pos2 = q.find("&", pos1 + 5);
	string food = q.substr(pos1 + 5, pos2 - pos1 - 5);

	pos1 = q.find("size=", pos2);
	pos2 = q.find("&", pos1 + 5);
	string size = q.substr(pos1 + 5, pos2 - pos1 - 5);

	pos1 = q.find("amount=", pos2);
	string amount = q.substr(pos1 + 7);


	Prikazi(food, size, amount);
}

void Prikazi(string food, string size, string amount)
{
	cout << "Content-Type: text/html\n\n";
	cout << "<html><head>\n";
	cout << "<title>Food</title>\n";
	cout << "<body>\n";
	if (food == "Pasta")
	{
		cout << "<h1>Dobar izbor</h1>\n";
	}
	else
		cout << "<h1>Nije dobar izbor</h1>\n";
	cout << "</body></html>\n";
}
int main()
{
	string method = getenv("REQUEST_METHOD");
	string q = getenv("QUERY_STRING");
	string e;
	if (method == "GET")
	{
		if (!q.empty())
		{
			parse(q);
		}
	}
	else if (method == "POST")
	{
		if (!q.empty())
		{
			parse(q);
		}
		int bytes = atoi(getenv("CONTENT_LENGTH"));
		cin >> e;
		parse(e);
	}
	return 0;
}