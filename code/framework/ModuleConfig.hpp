#ifndef MODULECONFIG_H
#define MODULECONFIG_H

#include <string.h>
#include <map>

using namespace std;

struct ModuleConfig{

		// name of this processing step in this configuration e.g. output
		string name;

		// name of the module that is used in this step e.g. storeImage
		string module;

		// a map of parameters configured for this module
		map<string, string> params;

		// input	e.g. <nameOfOtherModule.outputOfOtherModule>
		map<string, string> inputs;

};

#endif

