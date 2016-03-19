#include <bits/stdc++.h>
#include <curl/curl.h>
#include <curl/easy.h>
#include <string>
#include <iostream>
#include <cstdio>
#include <memory>

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

std::string exec(const char* cmd) {
    std::shared_ptr<FILE> pipe(popen(cmd, "r"), pclose);
    if (!pipe) return "ERROR";
    char buffer[128];
    std::string result = "";
    while (!feof(pipe.get())) {
        if (fgets(buffer, 128, pipe.get()) != NULL)
            result += buffer;
    }
    return result;
}

int download(char *url, string path)
{
	CURL *curl;
	FILE *outfile;
	CURLcode res;
    string execcommand = "chmod 755 ";
    execcommand += path;
	char outfilenamechararr[FILENAME_MAX_LENGTH];
	strncpy(outfilenamechararr, path.c_str(), sizeof(outfilenamechararr));
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
	ifstream f(outfilenamechararr);
	if (f.good()) {
        exec(execcommand.c_str());
		return 1;
    }
	return -1;
}
