#include <iostream>
#include <string>
#include <QApplication>
#include "framework/ModuleManager.hpp"
#include "framework/Configuration.hpp"

using namespace std;
using namespace uipf;

// argument is a configFile
int main(int argc, char** argv){

	if (argc < 2) {
		std::cerr << "Usage Error, at least one argument is required!\n\n";
		std::cerr << "Usage: " << argv[0] << " <configFile>"<< std::endl;
		return 1;
	}
	
	// run a processing chain from a config file.
	// ./uipf -c example.yam
	if (string(argv[1]).compare("-c") == 0){
		
		
		// QUESTION: for what is this used??
		QApplication app (argc,argv);

		// loads the configFile and create a Configuration
		Configuration conf;
		string configFileName = argv[2];
		conf.load(configFileName);

		// only for debug, print the loaded config
		conf.store("test.yaml");
		
	} else{
	// run a single module.
	// ./uipf <moduleName> ...options...		
	// TODO finish it

		// loads the configFile and create a Configuration
		Configuration conf;

		// TODO if there are more than one input file, this step should be repeated
		ProcessingStep loadModule;
		loadModule.name = "loadModule";
		// TODO: check whether it is the correct name of the module and correct name of the param
		loadModule.module = "loadImage";	
		loadImage.params.insert (pair<string,string>("filename",???) ); // fill from -i

		// TODO if there are more than one output file, this step should be repeated
		ProcessingStep storeModule;
		storeModule.name = "storeModule";
		// TODO: check whether it is the correct name of the module and correct name of the param
		storeModule.module = "storeImage";	
		storeImage.input.insert (pair<string, pair<string, string> >("image",???) ); 	// fill from module's outputs
		storeImage.params.insert (pair<string,string>("filename",???) ); 				// fill from -o, if empty, create automatically

		ProcessingStep processModule;
		processModule.name = "processModule";
		processModule.module = argv[2];	
		// TODO make a loop
		process.input.insert (pair<string, pair<string, string> >(???,???) ); 	// fill from -i
		process.params.insert (pair<string,string>("filename",???) ); 			// fill from -p

		// TODO maybe a loop
		conf.addProcessingStep(loadModule);
		conf.addProcessingStep(storeModule);
		conf.addProcessingStep(processModule);
	}
	
	// loads the Configuration and runs it
	ModuleManager mm(conf);
	mm.run();	
	
	
	return 0;
}


