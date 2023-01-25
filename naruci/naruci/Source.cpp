#include<iostream>
#include<string>

using namespace std;

void parse(string s)
{
	int pos1 = s.find("tip=", 0);
	int pos2 = s.find("&", pos1 + 4);
	string tip = s.substr(pos1 + 4, pos2 - pos1 - 4);
	int x = -1;
	x = tip.find("+");
	if (x != -1)
	{
		tip = tip.replace(x,1, " ");
	}
	pos1 = s.find("vel=", pos2);
	pos2 = s.find("&", pos1 + 4);
	string vel = s.substr(pos1 + 4, pos2 - pos1 - 4);

	pos1 = s.find("kolicina=", pos2);
	string kol = s.substr(pos1 + 9);


	cout << "Content-Type: text/html\n\n";
	cout << "<html><head>\n";
	cout << "<title>Naruci</title>\n";
	cout << "</head><body>\n";
	if (!tip.empty())
	{
		cout << "<h1> Uspesno ste narucili " << tip << "</h1>\n";
	}
	else
		cout << "<h1> Neuspesno ste narucili</h1>\n";
	cout << "</body></html>\n";

}
int main()
{	
	string req = getenv("REQUEST_METHOD");
	string q_string = getenv("QUERY_STRING");
	string s;
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

		int bytes = atoi(getenv("CONTENT_LENGTH"));
		if (bytes > 0)
		{
			cin >> s;
			parse(s);
		}
		
	}
	return 0;
}