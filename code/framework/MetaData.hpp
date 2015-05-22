#ifndef METADATA_H
#define METADATA_H

#include <string>
#include <map>
#include "types/Data.hpp"

namespace uipf {

// describes the module meta data
class MetaData {
	public:
		// constructor
		MetaData(std::string, std::map<std::string, Type>, std::map<std::string, Type>);
		// destructor
		~MetaData(void){};

		// textual description of the module
		std::string getDescription();

		// list of inputs  name => type
		std::map<std::string, Type> getInputs();

		// list of outputs  name => type
		std::map<std::string, Type> getOutputs();

	private:
		std::string description;

		std::map<std::string, Type> inputs;
		std::map<std::string, Type> outputs;

};

}

#endif

