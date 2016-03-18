#include <bits/stdc++.h>
#include <curl/curl.h>
//#include <curl/types.h>
#include <curl/easy.h>
#include <string>
#include <stdio.h>

#define FILENAME_MAX_LENGTH 1024

using namespace std;

string parsefilename(char url[])
{
	char *cname = strrchr(url, '/') + 1;
	stringstream ss;
	ss << cname;
	return ss.str();
}

size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream) {
    size_t written = fwrite(ptr, size, nmemb, stream);
    return written;
}

int download(char *url)
{
	CURL *curl;
    FILE *outfile;
    CURLcode res;
    string command;
//	stringstream ss;
//	ss << url;
	string outfilename;
	outfilename = parsefilename(url);
    char outfilenamechararr[FILENAME_MAX_LENGTH];
    strncpy(outfilenamechararr, outfilename.c_str(), sizeof(outfilenamechararr));
    outfilenamechararr[sizeof(outfilenamechararr) - 1] = 0;
    
    curl = curl_easy_init();
    if (curl) {
        outfile = fopen(outfilenamechararr,"wb");
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, outfile);
        res = curl_easy_perform(curl);
        // Cleaning up
        curl_easy_cleanup(curl);
        fclose(outfile);
    }
    return 0;
}
