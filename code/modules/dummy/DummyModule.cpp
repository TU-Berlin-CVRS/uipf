#include <iostream>
#include "DummyModule.hpp"

using namespace std;
using namespace uipf;


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
		"Dummy",
		map<string, DataDescription>(),
		map<string, DataDescription>(),
		map<string, ParamDescription>()
	);
}

