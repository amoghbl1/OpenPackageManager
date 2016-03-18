#include <bits/stdc++.h>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

using namespace boost::property_tree;
using namespace std;


void list_pack(string pack_list)
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
	printf("Packages:\n");
	for(int i = 0; i < names.size(); i++)
	{
		cout << names[i] << endl;
	}
}
