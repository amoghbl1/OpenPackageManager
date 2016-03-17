#include <bits/stdc++.h>
#include "boost/property_tree/ptree.hpp"
#include "boost/property_tree/json_parser.hpp"
using namespace boost::property_tree;
using namespace std;
string parseFileName(char* url){
    char* c_name = strrchr(url, '/') + 1;
    stringstream ss;
    ss << c_name;
    return ss.str();
}
int main(int argc, char **argv){

    if (argc < 2){
        cout << "Usage " << argv[0] << "<URL>" << endl;
        return 1;
    }
    string cmd;
    stringstream ss;
    ss << argv[1];
    cmd = "wget " + ss.str() + " 1> /dev/null 2> /dev/null";
    if (system(cmd.c_str())){
        cout << "Could not download" << endl;
        return 1;
    }
    else
        cout << "Packages List Download successfully" << endl;

    
    ifstream jsonFile("./" + parseFileName(argv[1]));
    ptree pt;
    read_json(jsonFile, pt);

    for(auto & package_list:pt){
        for(auto & package: package_list.second){
            cout << package.first << ": " << package.second.get_value<string>() << "\n";
        }
    }
}
