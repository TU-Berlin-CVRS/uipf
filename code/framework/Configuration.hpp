#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <string>
#include <map>
#include "ProcessingStep.hpp"
#include "MetaData.hpp"

namespace uipf{

class Configuration{

	public:
		// constructor
		Configuration(void){};
		// destructor
		~Configuration(void){};

		// loads the processing chain from the config file
		void load(std::string);

		// stores the processing chain into a config file
		void store(std::string);

		// prints the yaml representation of the config
		void print();

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
		std::map<std::string, ProcessingStep> chain_;

		// returns the YAML representation of the config, used for storing and printing
		std::string getYAML();

};

}

#endif

