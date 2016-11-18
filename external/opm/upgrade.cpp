#include <bits/stdc++.h>
#include "update.hpp"
#include "download.hpp"
#include "install.hpp"
#include "remove.hpp"
#include "paths.hpp"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

using namespace boost::property_tree;
using namespace std;

void fillMap(ptree &root, map<string, pair<string, float> > &M, int ty=0)
{
    string check[] = {"binlink", "conflink"};
    for (ptree::value_type &x : root)
    {
        //cout<<x.first<<"\n";
        for (ptree::value_type &y : x.second)
        {
            string name, link;
            float ver;
            for(auto &z : y.second)
            {
                if(z.first == "packagename")
                    name = z.second.data();
                else if(z.first == check[ty]) link = z.second.data();
                else if(z.first == "version") ver = atof((z.second.data()).c_str());
            }
            M[name] = make_pair(link, ver);
        }
    }

} 

void upgrade(string arg){

    map<string, pair<string, float> > M1, M2;
    ptree installed_root, fetch_root;

    //fetch the package.list file
    string s = PACKAGES_LIST_URL;
    char* s_copy = (char*)alloca(s.size() + 1); 
    memcpy(s_copy, s.c_str(), s.size() + 1); 
    int pack_stat = update(s_copy);                                        
    if(pack_stat == -1){                                                   
        cout << "Error: Packages.list download failed!!" << endl;          
        return;                                                            
    }  

    read_json("packages.list", fetch_root);
    string s1 = INSTALLED_LIST_PATH;
    read_json(s1, installed_root);
    fillMap(installed_root, M1);
    //for(auto &x:M1) cout<<x.second.first<<" "<<x.second.second<<"\n";
    
    fillMap(fetch_root, M2, 1);
    //for(auto &x:M2) cout<<x.second.first<<" "<<x.second.second<<"\n";
    
    vector<pair<string, string>> ins_cand;

    for(auto &x:M1){
        auto name = x.first;
        auto ver = x.second.second;
        //cout << "comparing m1package: " << name << ", version: " << ver << " with m2packver: " << M2[name].second <<endl;
        if(M2[name].second > ver){
            pair<string, string> p = {name, M2[name].first};
            ins_cand.push_back(p);
        }
    }
    cout << "Packages to upgrade: " << endl;
    if(ins_cand.size() == 0) cout << "None" << endl;
    for(auto &x:ins_cand){
        cout << x.first << ": " << x.second << endl;

        //remove the older version
        string s3 = x.first;
        char* s_copy2 = (char*)alloca(s3.size() + 1); 
        memcpy(s_copy2, s3.c_str(), s3.size() + 1); 
        remove_pack(s_copy2);

        //install the newer version
        string s2 = x.second;
        char* s_copy1 = (char*)alloca(s2.size() + 1); 
        memcpy(s_copy1, s2.c_str(), s2.size() + 1); 
        install_package_and_update(x.first, s_copy1, APP_BIN_PATH + parsefilename(s_copy1));
    }
}
