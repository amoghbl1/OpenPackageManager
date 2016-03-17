#include <bits/stdc++.h>
#include "download.hpp"

using namespace std;


int main(int argc, char *argv[])
{
	if(argc < 2 ) {
		printf("Usage: %s <URL>\n", argv[0]);
		return 1;
	}
	if(download(argv[1]) == 1)
		printf("Command executed\n");
	else
		printf("Command could not be executed\n");
	return 0;
}
