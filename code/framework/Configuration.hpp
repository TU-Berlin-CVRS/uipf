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
		// returns a set of error messages and a set of affected steps, config is valid if messages are empty
		std::pair< std::vector<std::string>, std::vector<std::string> > validate(std::map<std::string, MetaData>);

		// return processing chain  name => step
		std::map<std::string, ProcessingStep> getProcessingChain() const;

		// returns the named processing step
		ProcessingStep getProcessingStep(std::string) const;

		// returns a value indicating whether a named processing step exists
		bool hasProcessingStep(std::string name) const;


		// methods to manipulate the config:

		// adds a ProcessingStep to the chain
		void addProcessingStep(ProcessingStep);

		// removes a ProcessingStep from the chain
		void removeProcessingStep(std::string);

		// rename a ProcessingStep
		bool renameProcessingStep(std::string, std::string);

		// sets the module name for a named processing step
		void setProcessingStepModule(std::string, std::string, MetaData);

		// sets the parameters for a named processing step
		void setProcessingStepParams(std::string, std::map<std::string, std::string>);

		// sets the inputs for a named processing step
		void setProcessingStepInputs(std::string, std::map<std::string, std::pair<std::string, std::string> >);

	private:
		// chain of ProcessingSteps name => step
		std::map<std::string, ProcessingStep> chain_;

		// returns the YAML representation of the config, used for storing and printing
		std::string getYAML();

		// detect circular dependencies in the inputs
		std::vector<std::string> detectCircularDependencies();

};

} // namespace

#endif
