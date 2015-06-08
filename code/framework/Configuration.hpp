#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <string>
#include <map>
#include <opencv2/opencv.hpp>
#include "ProcessingStep.hpp"
#include "MetaData.hpp"

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
		// returns a set of error messages, config is valid if messages are empty
		std::vector<std::string> validate(std::map<std::string, MetaData>);

		// return processing chain  name => step
		std::map<std::string, ProcessingStep> getProcessingChain();

		// adds a ProcessingStep to the chain
		void addProcessingStep(ProcessingStep);

		// removes a ProcessingStep from the chain
		void removeProcessingStep(std::string);

	private:
		// chain of ProcessingSteps name => step
		std::map<std::string, ProcessingStep> chain;

};

}

#endif

