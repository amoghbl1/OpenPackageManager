#include <bits/stdc++.h>

using namespace std;

string parsefilename(char url[])
{
	char *cname = strrchr(url, '/') + 1;
	stringstream ss;
	ss << cname;
	return ss.str();
}

int main(int argc, char *argv[])
{
	if(argc < 2 ) {
		printf("Usage: %s <URL>\n", argv[0]);
		return 1;
	}
	string command;
	stringstream ss;
	ss << argv[1];
	string filename;
	filename = parsefilename(argv[1]);
	command = "wget " + ss.str();
	command += " 1> /dev/null 2> /dev/null -O " + filename;
	if(system(command.c_str()))
		printf("Could not execute command\n");
	else
		printf("Command successfully executed\n");
	return 0;
}
