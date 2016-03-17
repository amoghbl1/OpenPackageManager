#include <bits/stdc++.h>
#include "download.hpp"

using namespace std;

int update(char* url)
{
	if(download(url) == 1)
		return 1;//packages.list successfully downloaded
	return -1;//packages.list could not be downloaded
}
