#include <bits/stdc++.h>
#include "update.hpp"
#include "download.hpp"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

using namespace boost::property_tree;
using namespace std;

bool check_installed(char *pname)
{
	stringstream ss;
	ss << pname;
	ifstream jsonFile("./installed.list");
	ptree pt;
	read_json(jsonFile, pt);

	for(auto & package_list:pt){
		for(auto & package:package_list.second){
			if (package.second.get_value<string>() == ss.str()){
				return true;
			}
		}
	}
	return false;
}

string fetchconfurl(char *pname)
{
	stringstream ss;
	ss << pname;
	ifstream jsonFile("./packages.list");
	ptree pt;
	read_json(jsonFile, pt);

	for(auto & package_list:pt){
		if (package_list.second.get("packagename", "") == ss.str()){
			for(auto & package:package_list.second){
				if (package.first  == "conflink")
					return package.second.get_value<string>();
			}
		}
	}
	return "0";
}

/*int install_package(string fname)
{
	//return 1 on success, -1 otherwise
}*/

/*int update_local(char* pname)
{
	//return 1 on success, -1 otherwise
}*/

int main(int argc, char *argv[])
{
	if(argc < 2 ) {
		printf("Usage: %s <PackageName>\n", argv[0]);
		return 1;
	}

	//check if installed
	if(check_installed(argv[1]))
	{	
		printf("Package already installed\n");
		return 0;
	}

	//update the packages.list file
	string s = "https://raw.githubusercontent.com/amoghbl1/OpenPackageManager/master/packages.list";
	char* s_copy = (char*)alloca(s.size() + 1);
	memcpy(s_copy, s.c_str(), s.size() + 1);
	update(s_copy);

	//fetch the url of the package.conf file
	string confurl = fetchconfurl(argv[1]);
	if(confurl.compare("0") == 0)
	{
		printf("Could not locate requested package. Installation failed\n");
		return 0;
	}
	//cout << "conflink: " << confurl << endl;

	//download the conf file
	s_copy = (char*)alloca(confurl.size() + 1);
	memcpy(s_copy, confurl.c_str(), confurl.size() + 1);

	//get the conf file name and delete it in case of wrong download
	string conf_fname;
	conf_fname = parsefilename(s_copy);
	//cout << "conf name: " << conf_fname << endl;

	if(download(s_copy) == -1)
	{
		string command = "rm " + conf_fname;
		system(command.c_str());
		printf("Conf file could not be downloaded. Installation failed\n");
		return 0;
	}

	//install package
	/*if(install_package(conf_fname) == -1)
	{
		string command = "rm " + conf_fname;
		system(command.c_str());
		printf("Package installation failed\n");
		return 0;
	}*/

	//update installed.list file
	//while(update_local(argv[1]) != 1){}

	return 0;
}
