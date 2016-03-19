#include <bits/stdc++.h>
#include "download.hpp"
#include "paths.hpp"

using namespace std;

int update(char* url)
{
	string s = (string)APP_BIN_PATH;
	s += "packages.list";
	if(download(url, s) == 1)
		return 1;//packages.list successfully downloaded
	return -1;//packages.list could not be downloaded
}
