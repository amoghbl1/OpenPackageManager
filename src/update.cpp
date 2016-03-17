#include <bits/stdc++.h>
#include "download.hpp"

using namespace std;

<<<<<<< HEAD
int update(char* url)
{
	if(download(url) == 1)
		return 1;//packages.list successfully downloaded
	return -1;//packages.list could not be downloaded
=======

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
>>>>>>> 5d0ec88a328e33fc4532ffc6f1dcada799507302
}
