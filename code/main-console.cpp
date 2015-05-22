#include <iostream>
#include <string>
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

	// loads the configFile and create a Configuration
	Configuration conf;
	string configFileName = argv[1];
	conf.load(configFileName);

	// only for debug, print the loaded config
	conf.store("test.yaml");
	
	// loads the Configuration and runs it
	ModuleManager mm(conf);
	mm.run();	

	return 0;
}


