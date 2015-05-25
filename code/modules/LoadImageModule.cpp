#include <iostream>
#include "LoadImageModule.hpp"

using namespace std;
using namespace uipf;

// executes the Module
/*
input 		is a std::map of input resources, the names are described in the module meta description
params 		is a std::map of input paramaeters, the names are described in the module meta description
ouput 		is a std::map of output resources, the names are described in the module meta description
context 	is a container providing access to the current environment, allowing to open windows, write to logger etc...
*/
void LoadImageModule::run(map<string, Data*>& input, map<string, string> params, map<string, Data*>& output, Context context) const
{
	cout << "Dummy Module has been executed with the following params:";
	cout << endl;

	map<string, string>::iterator it = params.begin();
	for( ; it!=params.end(); ++it) {
		cout << "    " << it->first << " - " << it->second << endl;
	}

}

std::string LoadImageModule::name() const
{
	return "loadImage";
}
