#ifndef METADATA_H
#define METADATA_H

#include <string>
#include <map>
#include "types/Data.hpp"

namespace uipf {

// holds meta data for input and output
class DataDescription {
	public:

		// constructor
		DataDescription(Type t, std::string d) : type_(t), description_(d) {};
		// constuctor to allow copying
		DataDescription() {};
		// destructor
		~DataDescription(void){};

		Type getType() { return type_; };
		std::string getDescription() { return description_; };

	private:
		// the type of the input/output data
		Type type_;
		// a textual description
		std::string description_;
};

// holds meta data for module parameters
class ParamDescription {
	public:

		// constructor
		ParamDescription(std::string d) : description_(d) {};
		// constuctor to allow copying
		ParamDescription() {};
		// destructor
		~ParamDescription(void){};

		std::string getDescription() { return description_; };

	private:
		// a textual description
		std::string description_;
};

// describes the module meta data
class MetaData {
	public:
		// constructor
		MetaData(
			std::string, // description
			std::map<std::string, DataDescription>, // input
			std::map<std::string, DataDescription>, // output
			std::map<std::string, ParamDescription> // params
		);
		// constuctor to allow copying
		MetaData(){};
		// destructor
		~MetaData(void){};

		// textual description of the module
		std::string getDescription();

		// list of inputs  name => description details
		std::map<std::string, DataDescription> getInputs();

		// list of outputs  name => description details
		std::map<std::string, DataDescription> getOutputs();

		// list of parameters  name => description details
		std::map<std::string, ParamDescription> getParams();

	private:
		std::string description_;

		std::map<std::string, DataDescription> inputs_;
		std::map<std::string, DataDescription> outputs_;
		std::map<std::string, ParamDescription> params_;

};

}

#endif

