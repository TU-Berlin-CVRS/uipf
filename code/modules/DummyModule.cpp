#include <iostream>
#include "DummyModule.hpp"

using namespace std;
using namespace uipf;

// executes the Module
/*
input 		is a std::map of input resources, the names are described in the module meta description
params 		is a std::map of input paramaeters, the names are described in the module meta description
ouput 		is a std::map of output resources, the names are described in the module meta description
context 	is a container providing access to the current environment, allowing to open windows, write to logger etc...
*/
void DummyModule::run(map<string, Data::ptr& >& input, map<string, string>& params, map<string, Data::ptr>& output) const
{
	listParams(params);
	throw InvalidConfigException("test");
}


/*

*/
void DummyModule::run( DataManager& data) const
{
	data.listParams();
	throw InvalidConfigException("test");
}

MetaData DummyModule::getMetaData() const
{
	return MetaData(
		"A dummy module for testing purposes. It does nothing but printing the parameters.",
		map<string, DataDescription>(),
		map<string, DataDescription>(),
		map<string, ParamDescription>()
	);
}

