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
	return "Could not find request package";
}

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
	char* s_copy = (char*)alloca(s.size()+1);
	memcpy(s_copy, s.c_str(), s.size()+1);
	update(s_copy);

	//fetch the url of the package.conf file
	string confurl = fetchconfurl(argv[1]);
	cout << "confurl: " << confurl << endl;

	return 0;
}
