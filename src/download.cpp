#include <cstdio>
#include <iostream>
#include <string.h>
#include <algorithm>
#include "url2file.hpp"

using namespace std;

string parsefilename(char url[])
{
	string s;
	int i = strlen(url);
	while(url[i--] != '/')
		s += url[i + 1];
	reverse(s.begin(), s.end());
	return s;
}

int main(int argc, char *argv[])
{
	if(argc < 2 ) {
		printf("Usage: %s <URL>\n", argv[0]);
		return 1;
	}
	string fname;
	fname = parsefilename(argv[1]);
	download(fname, argv[1]);
	return 0;
}
