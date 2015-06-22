#include <fstream>
#include "Configuration.hpp"
#include "yaml-cpp/yaml.h"

using namespace std;
using namespace uipf;

// loads a configuration from a given .yaml file
/*
filename	the path to the .yaml file
*/
void Configuration::load(string filename){

	// clear current config
	chain_.clear();

	// load yaml file
	YAML::Node config = YAML::LoadFile(filename);

	// yaml file must be a map of step-name => configuration
	assert(config.IsMap());

	// create a ProcessingStep object from each config element
	YAML::const_iterator it = config.begin();
	for ( ; it != config.end(); ++it) {

		ProcessingStep step;

		step.name = it->first.as<string>();

		// iterate over step config, which is a map too
		assert(it->second.IsMap());
		YAML::const_iterator confIt = it->second.begin();
		for( ; confIt != it->second.end(); ++confIt) {
			string key = confIt->first.as<string>();
			if (key == "module") {
				step.module = confIt->second.as<string>();
			} else if (key == "input") {
				// input is a map of input dependencies
				assert(confIt->second.IsMap());
				YAML::const_iterator inputIt = confIt->second.begin();
				for(; inputIt != confIt->second.end(); ++inputIt) {
					string inputName = inputIt->first.as<string>();
					string dependsOnS = inputIt->second.as<string>();

					// split dependsOn by first . to separate step name and output name
					size_t dotPos = dependsOnS.find(".");
					pair<string, string> dependsOn( dependsOnS.substr( 0, dotPos ), dependsOnS.substr( dotPos + 1 ));

					step.inputs.insert( pair<string, pair<string, string> >(inputName, dependsOn) );
				}
			} else {
				// otherwise it is a parameter of the module
				step.params.insert( pair<string,string>(key, confIt->second.as<string>()) );
			}
		}

		chain_.insert( pair<string, ProcessingStep>(step.name, step) );
	}

}

// validate this config against a set of available modules
/*
modules		the path to the .yaml file
*/
vector<string> Configuration::validate(map<string, MetaData> modules){

	vector<string> errors;

	for(auto it = chain_.cbegin(); it != chain_.end(); ++it) {

		string inStep = string("In step '") + it->first + string("': ");
		ProcessingStep step = it->second;

		// check if module exists
		if (modules.count(step.module) == 0) {
			errors.push_back( inStep + string("Module '") + step.module + string("' could not be found.") );
			// can not do any further chechks on the step if module does not exist, so continue
			continue;
		}

		for(auto inputIt = step.inputs.cbegin(); inputIt != step.inputs.end(); ++inputIt) {
			// check if input exists for a module
			MetaData module = modules[step.module];
			if (module.getInputs().count(inputIt->first) == 0) {
				errors.push_back( inStep + string("Module '") + step.module + string("' has no input named '") + inputIt->first + string("'.") );
			}
			// check if dependencies refer to existing steps
			if (chain_.count(inputIt->second.first) == 0) {
				errors.push_back( inStep + string("Input '") + inputIt->first + string("' refers to non-existing step '") + inputIt->second.first + string("'.") );
			} else if (modules.count(chain_[inputIt->second.first].module) > 0) {
				// check if the referenced output exists
				MetaData refModule = modules[chain_[inputIt->second.first].module];
				if (refModule.getOutputs().count(inputIt->second.second) == 0) {
					errors.push_back( inStep + string("Input '") + inputIt->first
						+ string("' refers to non-existing output '") + inputIt->second.second
						+ string("' on module '") + chain_[inputIt->second.first].module + string("'.")
					);
				} else {
					// check if the type of output and input matches
					DataDescription output = refModule.getOutputs()[inputIt->second.second];
					if (output.getType() != module.getInputs()[inputIt->first].getType()) {
						errors.push_back( inStep + string("Type of input '") + inputIt->first
							+ string("' does not match the type of the referenced output.")
						);
					}
				}
			}
		}

		for(auto paramIt = step.params.cbegin(); paramIt != step.params.end(); ++paramIt) {
			// TODO check for mandatory params
		}

	}

	// TODO detect circular dependencies

	return errors;
}

// prints the current configurations yaml structure
/*
see also https://github.com/jbeder/yaml-cpp/wiki/How-To-Emit-YAML
*/
void Configuration::print(){

	string out = getYAML();

	std::cout << out;
	std::cout << std::endl;

}
// stores the current module configuration in a yaml file
/*
filename	the path to the .yaml file

see also https://github.com/jbeder/yaml-cpp/wiki/How-To-Emit-YAML
*/
void Configuration::store(string filename){

	string out = getYAML();

	std::ofstream fout(filename);
	fout << out;
}

// returns a the YAML representation of this configuration
// it can be used to print or store the YAML structure
string Configuration::getYAML(){

	// create YAML emitter
	YAML::Emitter out;

	// the YAML document consists of a map of processing steps
	out << YAML::BeginMap;
	map<string, ProcessingStep>::iterator it = chain_.begin();
	for (; it!=chain_.end(); ++it) {
		// each processing step has a name as the key and its config map as the value
		out << YAML::Key << it->first;
		out << YAML::Value << YAML::BeginMap;
			// first parameter is the module to be used
			out << YAML::Key << "module";
			out << YAML::Value << it->second.module;

			// second parameter are the inputs
			if (! it->second.inputs.empty()) {
				out << YAML::Key << "input";
				out << YAML::Value << YAML::BeginMap;
				map<string, pair<string, string> >::iterator inputIt = it->second.inputs.begin();
				for (; inputIt!=it->second.inputs.end(); ++inputIt) {
					out << YAML::Key << inputIt->first;
					out << YAML::Value << (inputIt->second.first + string(".") + inputIt->second.second);
				}
				out << YAML::EndMap;
			}

			// add parameters to the list
			map<string, string>::iterator paramIt = it->second.params.begin();
			for (; paramIt!=it->second.params.end(); ++paramIt) {
				out << YAML::Key << paramIt->first;
				out << YAML::Value << paramIt->second;
			}
		out << YAML::EndMap;
	}
	out << YAML::EndMap;

	return string(out.c_str());
}

// return processing chain  name => step
map<string, ProcessingStep> Configuration::getProcessingChain(){
	return chain_;
}

// returns the named processing step
ProcessingStep Configuration::getProcessingStep(string name){
	return chain_[name];
}

// adds a ProcessingStep to the chain
/*
 ProSt 		processing step, which is added to the chain
*/
void Configuration::addProcessingStep(ProcessingStep ProSt){
	chain_.insert(pair<std::string, ProcessingStep> (ProSt.name, ProSt));
}

// removes a ProcessingStep from the chain
/*
stepName	name of processingStep, which has to been deleted from the chain
*/
void Configuration::removeProcessingStep(string stepName){
	chain_.erase(stepName);
}

// sets the module name for a named processing step
void Configuration::setProcessingStepModule(string name, string module, MetaData metaData){
	string oldModule = chain_[name].module;

	// only update if the module has changed
	if (module.compare(oldModule) != 0) {
		chain_[name].module = module;

		// update params
		map<string, string> newParams;
		map<string, ParamDescription> paramsMeta = metaData.getParams();
		for(auto it = paramsMeta.begin(); it != paramsMeta.end(); ++it) {
			// add param to new list if it existed already, otherwise add default value i.e. ""
			if (chain_[name].params.count(it->first)) {
				newParams.insert( pair<string, string>(it->first, chain_[name].params[it->first]) );
			} else {
				newParams.insert( pair<string, string>(it->first, string("")) ); // TODO maybe add default value to ParamDescription of MetaData
			}
		}
		chain_[name].params = newParams;

		// update inputs
		map<string, pair<string, string> > newInputs;
		map<string, DataDescription> inputsMeta = metaData.getInputs();
		for(auto it = inputsMeta.begin(); it != inputsMeta.end(); ++it) {
			// add param to new list if it existed already, otherwise add default value i.e. ""
			if (chain_[name].inputs.count(it->first)) {
				newInputs.insert( pair<string, pair<string, string> >(it->first, chain_[name].inputs[it->first]) );
			} else {
				newInputs.insert( pair<string, pair<string, string> >(it->first, pair<string, string>(string(""), string("")) ) );
			}
		}
		chain_[name].inputs = newInputs;
	}
}

// sets the parameters for a named processing step
void Configuration::setProcessingStepParams(string name, map<string, string> params){
	chain_[name].params = params;
}

// sets the inputs for a named processing step
void Configuration::setProcessingStepInputs(string name, map<string, pair<string, string> > inputs){
	chain_[name].inputs = inputs;
}

