#include <bits/stdc++.h>

using namespace std;

string parsefilename(char url[])
{
	char *cname = strrchr(url, '/') + 1;
	stringstream ss;
	ss << cname;
	return ss.str();
}

int download(char *url)
{
	string command;
	stringstream ss;
	ss << url;
	string filename;
	filename = parsefilename(url);
	command = "wget " + ss.str();
	command += " 1> /dev/null 2> /dev/null -O " + filename;
	if(system(command.c_str()))

		return -1;
	else
		return 1;
}
