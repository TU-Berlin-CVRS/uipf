#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <opencv2/opencv.hpp>
#include "ProcessingStep.hpp"
#include <string.h>
#include <map>

using namespace cv;
using namespace std;

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

		// validates the config by checking for logical errors
		bool validate();
	
	private:
		// chain of ProcessingSteps
		map<string, ProcessingStep> chain;		

		
};

#endif

