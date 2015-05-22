#ifndef MODULEINTERFACE_H
#define MODULEINTERFACE_H

#include <string>
#include <map>
#include "types/Elem.hpp"
#include "Context.hpp"

using namespace std;

// describes the m
class ModuleInterface {
	public:
		// constructor (can't be virtual!)
		ModuleInterface(void){};
		// destructor
		virtual ~ModuleInterface(void){};
		
		// method has to be implemented in the Module
		// input 	is a std::map of input resources, the names are described in the module meta description
		// params 	is a std::map of input paramaeters, the names are described in the module meta description
		// ouput 	is a std::map of output resources, the names are described in the module meta description
		// context 	is a container providing access to the current environment, allowing to open windows, write to logger etc...
		virtual void run(map<string, Elem>& input, map<string, string> params, map<string, Elem>& output, Context context) = 0;
		
};


#endif




