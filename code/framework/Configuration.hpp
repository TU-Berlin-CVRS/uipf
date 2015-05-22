#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <string>
#include <map>
#include <opencv2/opencv.hpp>
#include "ProcessingStep.hpp"

namespace uipf{


class Configuration{

	public:
		// constructor
		Configuration(void){};
		// destructor
		~Configuration(void){};

		// loads the module chain with the params from the config file
		void load(std::string);
		
		// stores the module chain with the params in the config file
		void store(std::string);

		// validates the config by checking for logical errors
		bool validate();
	
	private:
		// chain of ProcessingSteps
		std::map<std::string, uipf::ProcessingStep> chain;		

		
};

}

#endif

