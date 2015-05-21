#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <opencv2/opencv.hpp>
#include "ModuleConfig.hpp"

using namespace cv;

class Configuration{

	public:
		// constructor
		Configuration(void){};
		// destructor
		~Configuration(void){};

		// loads the module chain with the params from the config file
		void load(string);
		
		// stores the module chain with the params in the config file
		void store(string);
	
	private:
		// chain of ModuleConfigs
		vector<ModuleConfig> chain;		

		
};

#endif

