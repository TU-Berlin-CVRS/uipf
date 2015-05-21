#include "Configuration.hpp"
#include "ModuleConfig.hpp"
#include "yaml-cpp/yaml.h"
#include <fstream>

using namespace std;

//
/*
*/
void Configuration::load(string filename){

	// clear current config
	chain.clear();

	// load yaml file
	YAML::Node config = YAML::LoadFile(filename);

	// yaml file must be a sequence of modules
	assert(config.IsSequence());

	// create a ModuleConfig object from each config element
	for (std::size_t i = 0; i < config.size(); i++) {

		ModuleConfig mod;

		// each module is a map with one element, modulename => moduleconfig
		assert(config[i].IsMap());
		YAML::const_iterator it = config[i].begin();
		if (it == config[i].end()) {
			// TODO error
			return;
		}

		mod.name = it->first.as<string>();

		// iterate over module config
		assert(it->second.IsMap());
		for(YAML::const_iterator confIt = it->second.begin(); confIt != it->second.end(); ++confIt) {
			string key = confIt->first.as<string>();
			if (key == "module") {
				mod.task = confIt->second.as<string>();
			} else if (key == "input") {
				// input is a map of input dependencies
				assert(confIt->second.IsMap());
				YAML::const_iterator inputIt = confIt->second.begin();
				for(; inputIt != confIt->second.end(); ++inputIt) {
					string inputName = inputIt->first.as<string>();
					string dependsOn = inputIt->second.as<string>();
					mod.inputs.insert( pair<string,string>(inputName, dependsOn) );
				}
			} else {
				mod.params.insert( pair<string,string>(key, confIt->second.as<string>()) );
			}
		}

		chain.push_back(mod);
	}

}

bool Configuration::validate(){
	// TODO validate if inputs and outputs correspond
	// TODO think of further validation needs
}

//
/*
	stores the current module configuration in a yaml file

	see also https://github.com/jbeder/yaml-cpp/wiki/How-To-Emit-YAML
*/
void Configuration::store(string filename){
	// TODO

	// create YAML emitter
	YAML::Emitter out;

	// the YAML document consists of a sequence of modules
	out << YAML::BeginSeq;
	for(int i = 0; i < chain.size(); i++) {
		// each module is a map which contains the module name as a key
		// the module config is the value
		out << YAML::BeginMap;
			out << YAML::Key << chain[i].name;
			out << YAML::Value << YAML::BeginMap;
				// first parameter is the module to be used
				out << YAML::Key << "module";
				out << YAML::Value << chain[i].task;

				// TODO add inputs / dependencies

				// add parameters to the list
				map<string,string>::iterator it = chain[i].params.begin();
				for (; it!=chain[i].params.end(); ++it) {
					out << YAML::Key << it->first;
					out << YAML::Value << it->second;
				}
			out << YAML::EndMap;
		out << YAML::EndMap;
	}
	out << YAML::EndSeq;

	std::cout << "Here's the output YAML:\n" << out.c_str();
	std::cout << std::endl;
}

