#include <fstream>
#include "Configuration.hpp"
#include "yaml-cpp/yaml.h"

using namespace std;
using namespace uipf;

//
/*
*/
void Configuration::load(string filename){

	// clear current config
	chain.clear();

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

		chain.insert( pair<string, ProcessingStep>(step.name, step) );
	}

}

bool Configuration::validate(){
	// TODO validate if inputs and outputs correspond and are not empty
	// TODO detect circular dependencies
	// TODO think of further validation needs
	return true;
}

//
/*
	stores the current module configuration in a yaml file

	see also https://github.com/jbeder/yaml-cpp/wiki/How-To-Emit-YAML
*/
void Configuration::store(string filename){

	// create YAML emitter
	YAML::Emitter out;

	// the YAML document consists of a map of processing steps
	out << YAML::BeginMap;
	map<string, ProcessingStep>::iterator it = chain.begin();
	for (; it!=chain.end(); ++it) {
		// each processing step has a name as the key and its config map as the value
		out << YAML::Key << it->first;
		out << YAML::Value << YAML::BeginMap;
			// first parameter is the module to be used
			out << YAML::Key << "module";
			out << YAML::Value << it->second.module;

			// second parameter are the inputs
			if (! it->second.inputs.empty()) {
				out << YAML::Key << "inputs";
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

	std::cout << "Here's the output YAML:\n" << out.c_str();
	std::cout << std::endl;

	// TODO write into real file instead of stdout
}

map<string, ProcessingStep> Configuration::getProcessingChain(){
	return chain;
}
