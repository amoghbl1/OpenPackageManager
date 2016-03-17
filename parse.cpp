#include <bits/stdc++.h>
#include "boost/property_tree/ptree.hpp"
#include "boost/property_tree/json_parser.hpp"
using namespace boost::property_tree;
using namespace std;
int main(int argc, char **argv){
    
    ifstream jsonFile("./test.json");
    ptree pt;
    read_json(jsonFile, pt);

    for(auto & package_list:pt){
        for(auto & package: package_list.second){
            cout << package.first << ": " << package.second.get_value<string>() << "\n";
        }
    }
    

}
