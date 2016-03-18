#include <bits/stdc++.h>
#include "download.hpp"
#include "paths.hpp"

using namespace std;

int update(char* url)
{
	if(download(url, APP_BIN_PATH) == 1)
		return 1;//packages.list successfully downloaded
	return -1;//packages.list could not be downloaded
}
