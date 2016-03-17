#include <bits/stdc++.h>

using namespace std;

int main(int argc, char *argv[])
{
	if(argc < 2 ) {
		printf("Usage: %s <URL>\n", argv[0]);
		return 1;
	}
	string command;
	stringstream ss;
	ss << argv[1];
	command = "wget " + ss.str() + " 1> /dev/null 2> /dev/null";
	if(system(command.c_str()))
		printf("Could not execute command\n");
	else
		printf("Command successfully executed\n");
	return 0;
}
