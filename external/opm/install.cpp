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
    ifstream jsonFile("../../installed.list");
    if (jsonFile.good()){
        ptree pt;
        read_json(jsonFile, pt);

        for(auto & package_list:pt.get_child("installed")){
            for(auto & package:package_list.second){
                if (package.second.get_value<string>() == ss.str()){
                    return true;
                }
            }
        }
    }
	return false;
}

vector<string> list(){

    stringstream ss;
    ifstream jsonFile("../../installed.list");
    vector<string> names;
    if (jsonFile.good()){
        ptree pt;
        read_json(jsonFile, pt);

        for(auto & package_list:pt.get_child("installed")){
            for(auto & package:package_list.second){
                names.push_back(package.first);
            }
        }
    }
    return names;
}


// #TODO check local repo download

void update_local(string pname, string binlink, string version)
{
 

    ifstream jsonFile("../../installed.list");
    ptree pt, mpt, npt;
    if (jsonFile.good()){
        read_json(jsonFile, pt);
        bool cont = true;
        for(auto & package_list:pt.get_child("installed")){
            ptree new_package;
            for(auto & package:package_list.second ){
                if (package.first == ""){
                    cont = false;
                    break;
                }
                new_package.put(package.first,package.second.get_value<string>());
            }
            if (cont)
                mpt.push_back(make_pair("", new_package));
            else
                break;
        }
    }
	
    ptree new_package;
    new_package.put("packagename",pname);
    new_package.put("binlink",binlink);
    new_package.put("version",version);
    mpt.push_back(make_pair("", new_package));
    npt.add_child("installed", mpt);
    write_json("../../installed.list",npt);
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
				if (package.first  == "conflink"){
					return package.second.get_value<string>();
				}
			}
		}
	}
}

int install_package_and_update(string fname)
{
    stringstream ss;
    ss << fname;
   
    ifstream jsonFile("./" + fname);
    
 //   ifstream jsonFile("../../conf/" + fname + ".conf");
    ptree pt;
    read_json(jsonFile, pt);
    int ret;
    string b_link, p_name, version;
    for(auto & package:pt){
		if (package.first == "binlink"){
            cout << "url" + package.second.get_value<string>();
            string s = package.second.get_value<string>();
            b_link = s;
	        char* s_copy = (char*)alloca(s.size() + 1);
	        memcpy(s_copy, s.c_str(), s.size() + 1);
            ret = download(s_copy);
           
		}
        else if (package.first == "version")
            version = package.second.get_value<string>();
        else if (package.first == "packagename")
            p_name = package.second.get_value<string>();
	}
    if (ret){
        update_local(p_name, b_link, version);
        return 1;
    }
    return -1;
}


int main(int argc, char *argv[])
{
	if(argc < 2 ) {
		printf("Usage: %s <PackageName>\n", argv[0]);
		return 1;
	}

    // #TODO if installed.list not there, skip this
/*	if(check_installed(argv[1]))
	{	
		printf("Package already installed\n");
		return 0;
	}

*/	//update the packages.list file
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
	 cout << "conflink: " << confurl << endl;

	//download the conf file
	s_copy = (char*)alloca(confurl.size() + 1);
	memcpy(s_copy, confurl.c_str(), confurl.size() + 1);

	//get the conf file name and delete it in case of wrong download
	string conf_fname;
	conf_fname = parsefilename(s_copy);
	cout << "conf name: " << conf_fname << endl;

	if(download(s_copy) == -1)
	{
		string command = "rm " + conf_fname;
		system(command.c_str());
		printf("Conf file could not be downloaded. Installation failed\n");
		return 0;
	}

	if(install_package_and_update(conf_fname) == -1)
	{
		printf("Package installation failed\n");
		return 0;
	}

	//remove conf file
	//string command = "rm " + conf_fname;
	//system(command.c_str());

	//update installed.list file
    return 0;
}
