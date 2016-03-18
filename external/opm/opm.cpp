#include<bits/stdc++.h>
#include "install.hpp"
#include "list.hpp"
#include "search.hpp"

using namespace std;

//Command: ./opm command <arguments>

void _install_help(bool e)
{
	printf("Install a package:\n\t./opm install packagename\n");
	if(e) exit(1);
}

void _remove_help(bool e)
{
	printf("Remove a package:\n\t./opm remove packagename\n");
	if(e) exit(1);
}

void _upgrade_help(bool e)
{
	printf("Upgrade a package:\n\t./opm updgrade packagename\n");
	printf("Upgrade all packages:\n\t./opm updgrade all\n");
	if(e) exit(1);
}

void _list_help(bool e)
{
	printf("List all installed packages:\n\t./opm list all\n");
	printf("Check if a space separated list of packages are installed or not:\n\t./opm install packagelist\n");
	if(e) exit(1);
}

void _search_help(bool e)
{
	printf("List all packages on repo:\n\t./opm search all\n");
	printf("Search for a space separated list of packages on repo:\n\t./opm search packagelist\n");
	if(e) exit(1);
}

void help(int verbosity)
{
	printf("Usage: ./opm <command> <argument>\n");
	if(verbosity == 1)
	{
		printf("\nCommands:\n\n");
		_install_help(false);
		_remove_help(false);
		_upgrade_help(false);
		_list_help(false);
		_search_help(true);
	}
}

void suggest()
{
	help(0);
	printf("\nMaybe you are looking for:\n");
}

int main(int argc, char *argv[])
{
	if(argc < 2)
	{
		help(0);
		exit(0);
	}
	
	string comm(argv[1]);
	if(!comm.compare("install"))
	{
		if((argc == 2) || (argc > 3))
		{
			suggest();
			_install_help(true);
		}
		install(argv[2]);
	}
	else if(!comm.compare("remove"))
	{
		if((argc == 2) || (argc > 3))
		{
			suggest();
			_remove_help(true);
		}
		//remove(argv[2]);
	}
	else if(!comm.compare("upgrade"))
	{
		if((argc == 2) || (argc > 3))
		{
			suggest();
			_upgrade_help(true);
		}
		//upgrade(argv[2]);
	}
	else if(!comm.compare("list"))
	{
		if(argc == 2)
		{
			suggest();
			_list_help(true);
		}
		vector <string> packs;
		if(argc == 3)
		{
			string arg(argv[2]);
			if(!arg.compare("all"))
				packs.clear();
			else
				packs.push_back(arg);
		}
		else
		{
			for(int i = 2; i < argc; i++)
			{
				string arg(argv[i]);
				packs.push_back(arg);
			}
		}
		list_pack(packs);
	}
	else if(!comm.compare("search"))
	{
		if(argc == 2)
		{
			suggest();
			_search_help(true);
		}
		vector <string> packs;
		if(argc == 3)
		{
			string arg(argv[2]);
			if(!arg.compare("all"))
				packs.clear();
			else
				packs.push_back(arg);
		}
		else
		{
			for(int i = 2; i < argc; i++)
			{
				string arg(argv[i]);
				packs.push_back(arg);
			}
		}
		search_pack(packs);
	}
	else if((!comm.compare("-h")) || (!comm.compare("--help")))
		help(1);
	else
		cout << "Unknown Command: " << comm << endl;
	
	return 0;
}
