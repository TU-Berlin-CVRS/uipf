#include <iostream>
#include "GaussianModule.hpp"

using namespace std;
using namespace uipf;

// executes the Module
/*
input 		is a std::map of input resources, the names are described in the module meta description
params 		is a std::map of input paramaeters, the names are described in the module meta description
ouput 		is a std::map of output resources, the names are described in the module meta description
context 	is a container providing access to the current environment, allowing to open windows, write to logger etc...
*/
void GaussianModule::run(map<string, Data::ptr& >& input, map<string, string>& params, map<string, Data::ptr>& output ) const
{
	listParams(params);
}

std::string GaussianModule::name() const
{
	return "gaussian";
}
