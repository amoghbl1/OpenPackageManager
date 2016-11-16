#include <bits/stdc++.h>
#include "update.hpp"
#include "download.hpp"
#include "paths.hpp"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

using namespace boost::property_tree;
using namespace std;

int install(char *packname);

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
    string installedlist = APP_BIN_PATH;
    installedlist += "installed.list";
	ifstream jsonFile(installedlist);
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

void update_local(string pname, string binlink, string version)
{

    string installedlist = APP_BIN_PATH;
    installedlist += "installed.list";
	ifstream jsonFile(installedlist);
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
	write_json(installedlist, npt);
    string execcommand = "chmod 755 ";
    execcommand += installedlist;
    // This comes from download.hpp
    exec(execcommand.c_str());
}

string fetchconfurl(char *pname)
{
	stringstream ss;
	ss << pname;
    string packageslist = APP_BIN_PATH;
    packageslist += "packages.list";
    ifstream jsonFile(packageslist);
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

	for(auto & ppt:pt){
        for(auto & package_list:ppt.second){
            bool ok = 0;
            for(auto & package : package_list.second) {
                if (package.first == "packagename" && package.second.data() == ss.str()){
                    ok = 1;
                }
                if (package.first  == "conflink" && ok){
                    return package.second.data();
                }
            }

        }
    }
    return "0";
}

int install_package_and_update(string pacname, char* confurl, string fname)
{
    if(download(confurl, fname) == -1)
    {
        rm(fname);
        printf("Conf file could not be downloaded. Installation failed\n");
        exit(0);
    }
    stringstream ss;
    ss << fname;

    ifstream jsonFile(fname);

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
    vector<pair <string, float> > dp;
    int ret;
    string b_link, p_name, version;
    //for(auto & ppt:pt){
        for(auto & package:pt){
            if (package.first == "binlink"){
                cout << "binurl: " + package.second.get_value<string>() << endl;
                string s = package.second.get_value<string>();
                b_link = s;
                char* s_copy = (char*)alloca(s.size() + 1);
                memcpy(s_copy, s.c_str(), s.size() + 1);
                string s1 = (string)APP_PACKAGES_PATH;
                s1 += pacname;
                ret = download(s_copy, s1);

            }
            else if (package.first == "version")
                version = package.second.get_value<string>();
            else if (package.first == "packagename")
                p_name = package.second.get_value<string>();
            else if(package.first == "dependencies"){
                for(auto &x : package.second){
                    dp.push_back(make_pair(x.first, stof(x.second.data())));
                }
            
            }
        }
    cout << "dependencies: "<<endl;
    for(auto &x:dp){
        cout << x.first << ": " << x.second << endl;
        string s = x.first;
        char* s_copy = (char*)alloca(s.size() + 1);
        memcpy(s_copy, s.c_str(), s.size() + 1);
        install(s_copy);
    }
    //}
    if (ret){
        update_local(p_name, b_link, version);
        return 1;
    }
    return -1;
}


int install(char *packname)
{
    string pacname(packname);
    // #TODO if installed.list not there, skip this
    if(check_installed(packname))
    {	
        printf("Package already installed\n");
        return 0;
    }

    string s = PACKAGES_LIST_URL;
    char* s_copy = (char*)alloca(s.size() + 1);
    memcpy(s_copy, s.c_str(), s.size() + 1);
    update(s_copy);

    //fetch the url of the package.conf file
    string confurl = fetchconfurl(packname);

    if(confurl.compare("0") == 0)
    {
        printf("Could not locate requested package. Installation failed\n");
        exit(0);
    }
    cout << "conflink: " << confurl << endl;

    //download the conf file
    s_copy = (char*)alloca(confurl.size() + 1);
    memcpy(s_copy, confurl.c_str(), confurl.size() + 1);

    //get the conf file name and delete it in case of wrong download
    string conf_fname;
    conf_fname = parsefilename(s_copy);
    cout << "conf name: " << conf_fname << endl;

    string abspath = APP_BIN_PATH;
    abspath += conf_fname;

    if(install_package_and_update(pacname, s_copy, abspath) == -1)
    {
        // rm(abspath);
        printf("Package installation failed\n");
        exit(0);
    }

    //remove conf file
    rm(abspath);

    return 0;
}
