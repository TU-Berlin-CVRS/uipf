#ifndef PROCESSINGSTEP_H
#define PROCESSINGSTEP_H

#include <string.h>
#include <map>

using namespace std;

struct ProcessingStep{

		// name of this processing step in this configuration e.g. output
		string name;

		// name of the module that is used in this step e.g. storeImage
		string module;

		// a map of parameters configured for this module e.g. filename => image.png
		map<string, string> params;

		// inputs from other modules <inputName> => <nameOfOtherModule.outputOfOtherModule>
		map<string, string> inputs;

};

#endif

