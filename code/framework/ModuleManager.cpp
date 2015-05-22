#include "ModuleManager.hpp"

#include <string>

using namespace uipf;
using namespace std;

// loads the configuration and stores in the ModuleManager
/*
conf	Configuration file, which has to be executed
*/
ModuleManager::ModuleManager(Configuration conf){
	config = conf;
}

// executes the Configuration file
/*
*/
void ModuleManager::run(){

	// TODO sort processing steps

	// get processing chain
	map<string, ProcessingStep> chain = config.getProcessingChain();

	Context context;

	// iterate over all modules and run them
	map<string, ProcessingStep>::iterator it = chain.begin();
	for (; it!=chain.end(); ++it) {

		// load the module
		ModuleInterface* mod = loadModule(it->second.module);

		// TODO fill outputs with inputs from other modules
		map<string, Data*> inputs;
		map<string, Data*> outputs;

		cout << "Running step '" << it->first << "'..." << endl;
		mod->run(inputs, it->second.params, outputs, context);
		cout << "Done with step '" << it->first << "'." << endl << endl;

		// TODO store outputs somewhere
	}

}

ModuleInterface* ModuleManager::loadModule(string name){
	return new DummyModule;
}

MetaData ModuleManager::getModuleMetaData(string name){

	// TODO this is dummy

	map<string, Type> in;
	in.insert( pair<string, Type>("image", MATRIX) );

	map<string, Type> out;
	out.insert( pair<string, Type>("image", MATRIX) );

	MetaData meta("Dummy module description for " + name, in, out);

	return meta;

}



