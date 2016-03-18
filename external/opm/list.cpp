#include <bits/stdc++.h>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

using namespace boost::property_tree;
using namespace std;


void list_pack(vector<string> pack_list)
{

	stringstream ss;
	ifstream jsonFile("./installed.list");
	vector<string> names;
	if (jsonFile.good()){
		ptree pt;
		read_json(jsonFile, pt);
		for(auto & package_list:pt.get_child("installed")){
			for(auto & package:package_list.second){
                if (package.first == "packagename"){
                    if (pack_list.size() == 0 || find(pack_list.begin(), pack_list.end(), package.second.get_value<string>()) != pack_list.end())
                        names.push_back(package.second.get_value<string>());
                }
			}
		}
	}
	printf("Packages:\n");
	for(int i = 0; i < names.size(); i++)
	{
		cout << names[i] << endl;
	}
}
