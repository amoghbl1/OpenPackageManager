#include <bits/stdc++.h>
#include "update.hpp"
#include "download.hpp"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

using namespace boost::property_tree;
using namespace std;

void rm(string fname)
{
	if(remove(fname.c_str()) != 0)
	{
		printf("Could not delete %s\n", fname.c_str());
		exit(1);
	}
}

bool check_installed(char *pname)
{

	stringstream ss;
	ss << pname;
	ifstream jsonFile("./installed.list");
	if (jsonFile.good()){
		ptree pt;
		try
		{
			read_json(jsonFile, pt);
		}
		catch(exception const& e)
		{
			printf("Error parsing json file: ");
			cerr << e.what() << endl;
			return false;
		}

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

vector<string> list()
{

	stringstream ss;
	ifstream jsonFile("./installed.list");
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


void update_local(string pname, string binlink, string version)
{


	ifstream jsonFile("./installed.list");
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
	new_package.put("packagename", pname);
	new_package.put("binlink", binlink);
	new_package.put("version", version);
	mpt.push_back(make_pair("", new_package));
	npt.add_child("installed", mpt);
	write_json("./installed.list", npt);
}

string fetchconfurl(char *pname)
{
	stringstream ss;
	ss << pname;
	ifstream jsonFile("./packages.list");
	ptree pt;
	try
	{
		read_json(jsonFile, pt);
	}
	catch(exception const& e)
	{
		printf("Error parsing json file: ");
		cerr << e.what() << endl;
		return "0";
	}

	for(auto & package_list:pt){
		if (package_list.second.get("packagename", "") == ss.str()){
			for(auto & package:package_list.second){
				if (package.first  == "conflink"){
					return package.second.get_value<string>();
				}
			}
		}
	}
	return "0";
}

int install_package_and_update(string fname)
{
	stringstream ss;
	ss << fname;

	ifstream jsonFile("./" + fname);

	ptree pt;
	try
	{
		read_json(jsonFile, pt);
	}
	catch(exception const& e)
	{
		printf("Error parsing json file: ");
		cerr << e.what() << endl;
		return -1;
	}

	int ret;
	string b_link, p_name, version;
	for(auto & package:pt){
		if (package.first == "binlink"){
			cout << "url" + package.second.get_value<string>() << endl;
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


int install(char *packname)
{
	// #TODO if installed.list not there, skip this
	if(check_installed(packname))
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
	string confurl = fetchconfurl(packname);


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
		rm(conf_fname);
		printf("Conf file could not be downloaded. Installation failed\n");
		return 0;
	}

	if(install_package_and_update(conf_fname) == -1)
		printf("Package installation failed\n");

	//remove conf file
	rm(conf_fname);

	return 0;
}
