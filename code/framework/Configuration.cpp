#include <fstream>
#include "Configuration.hpp"
#include "yaml-cpp/yaml.h"
#include "Logger.hpp"
#include "InvalidConfigException.hpp"

using namespace std;
using namespace uipf;

// loads a configuration from a given .yaml file
/*
filename	the path to the .yaml file
*/
void Configuration::load(string filename){

	//basic requirement checks
	bool bHasModule = false;

	try
	{
		// clear current config
		chain_.clear();

		// load yaml file
		YAML::Node config = YAML::LoadFile(filename);

		// yaml file must be a map of step-name => configuration
		if (!config.IsMap())
			throw InvalidConfigException("check if you provided at least one step.");


		// create a ProcessingStep object from each config element
		YAML::const_iterator it = config.begin();
		for ( ; it != config.end(); ++it) {

			ProcessingStep step;

			step.name = it->first.as<string>();

			// iterate over step config, which is a map too
			if (!it->second.IsMap())
				throw InvalidConfigException("check if you provided valid value(s) in your yaml for step '" + it->first.as<string>() + "'");

			YAML::const_iterator confIt = it->second.begin();

			for( ; confIt != it->second.end(); ++confIt) {
				string key = confIt->first.as<string>();
				if (key == "module")
				{
					bHasModule = true;
					step.module = confIt->second.as<string>();
				}
				else if (key == "input")
				{
					// input is a map of input dependencies
					if (!confIt->second.IsMap())
						throw InvalidConfigException("check if you provided valid keys and a values for 'input'");

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
					try
					{
						step.params.insert( pair<string,string>(key, confIt->second.as<string>()) );
					}
					catch(YAML::TypedBadConversion<std::string>& ex)
					{
						throw InvalidConfigException("check if you provided a valid key and value for '"+ key + "'");
					}
				}
			}

			chain_.insert( pair<string, ProcessingStep>(step.name, step) );
		}
	}
	catch(InvalidConfigException& ex)
	{
		LOG_E(string(ex.what()));
	}
	catch(YAML::BadFile& ex)
	{
		LOG_E("Error loading '" + filename +"'");
	}
	catch(YAML::TypedBadConversion<std::string>& ex)
	{
		LOG_E(string(ex.what()));
	}
	catch(YAML::ParserException& ex)
	{
		LOG_E(string(ex.what()));
	}
	catch(...)
	{
		if (bHasModule)
			LOG_E("your yaml seems to be invalid.");
		else
			LOG_E("your yaml seems to be invalid. check if you defined 'module'");
	}

}

// validate this config against a set of available modules returns a list of error message and a list of affected steps
/*
modules		meta data of the existing modules
*/
pair< vector<string>, vector<string> > Configuration::validate(map<string, MetaData> modules){

	vector<string> errors;
	vector<string> affectedSteps;

	for(auto it = chain_.cbegin(); it != chain_.end(); ++it) {

		string inStep = string("In step '") + it->first + string("': ");
		ProcessingStep step = it->second;

		// check if module exists
		if (modules.count(step.module) == 0) {
			errors.push_back( inStep + string("Module '") + step.module + string("' could not be found.") );
			affectedSteps.push_back(step.name);
			// can not do any further chechks on the step if module does not exist, so continue
			continue;
		}

		// metadata of the module of the current step
		MetaData module = modules[step.module];

		for(auto inputIt = step.inputs.cbegin(); inputIt != step.inputs.end(); ++inputIt) {
			// check if input exists for a module
			if (module.getInputs().count(inputIt->first) == 0) {
				errors.push_back( inStep + string("Module '") + step.module + string("' has no input named '") + inputIt->first + string("'.") );
				affectedSteps.push_back(step.name);
				continue;
			}
			// check if input is set when not optional
			if (inputIt->second.first.empty()) {
				if (!module.getInputs()[inputIt->first].getIsOptional()) {
					errors.push_back( inStep + string("Input '") + inputIt->first + string("' is not optional and not defined.") );
					affectedSteps.push_back(step.name);
				}
				continue;
			}
			// check if dependencies refer to existing steps
			if (chain_.count(inputIt->second.first) == 0) {
				errors.push_back( inStep + string("Input '") + inputIt->first + string("' refers to non-existing step '") + inputIt->second.first + string("'.") );
				affectedSteps.push_back(step.name);
			} else if (modules.count(chain_[inputIt->second.first].module) > 0) {
				// check if the referenced output exists
				MetaData refModule = modules[chain_[inputIt->second.first].module];
				if (refModule.getOutputs().count(inputIt->second.second) == 0) {
					errors.push_back( inStep + string("Input '") + inputIt->first
						+ string("' refers to non-existing output '") + inputIt->second.second
						+ string("' on module '") + chain_[inputIt->second.first].module + string("'.")
					);
					affectedSteps.push_back(step.name);
				} else {
					// check if the type of output and input matches
					DataDescription output = refModule.getOutputs()[inputIt->second.second];
					if (output.getType() != module.getInputs()[inputIt->first].getType()) {
						errors.push_back( inStep + string("Type of input '") + inputIt->first + string("' ( ")
							+ type2string(module.getInputs()[inputIt->first].getType())
							+ string(" ) does not match the type of the referenced output '") + inputIt->second.first + string(".") + inputIt->second.second + string("'")
							+ string(" which is of type ")
							+ type2string(output.getType()) + string(".")
						);
						affectedSteps.push_back(step.name);
					}
				}
			}
		}

		auto paramDesc = module.getParams();
		//check, if all mandatory parameters are given
		for(auto paramIt = paramDesc.cbegin(); paramIt != paramDesc.end(); ++paramIt)
		{
			if (!paramIt->second.getIsOptional()) {
				if ( step.params.find(paramIt->first) == step.params.end()) {
					errors.push_back("mandatory parameter '" + string(paramIt->first) +  "' of module '"+step.module+"' is not set!");
					affectedSteps.push_back(step.name);
				} else if (step.params[paramIt->first].empty()) {
					errors.push_back("mandatory parameter '" + string(paramIt->first) +  "' of module '"+step.module+"' is empty!");
					affectedSteps.push_back(step.name);
				}
			}
		}

		auto inputDesc = module.getInputs();
		//check, if all mandatory inputs are given
		for(auto paramIt = inputDesc.cbegin(); paramIt != inputDesc.end(); ++paramIt)
		{
			if (!paramIt->second.getIsOptional()) {
				if ( step.inputs.find(paramIt->first) == step.inputs.end()) {
					errors.push_back("mandatory input: '" + string(paramIt->first) +  "' of module '"+step.module+"' is not set!");
					affectedSteps.push_back(step.name);
				}
			}
		}

		/*auto outputDesc = module.getOutputs();
		//check, if all mandatory outputs are given
		for(auto paramIt = outputDesc.cbegin(); paramIt != outputDesc.end(); ++paramIt)
		{
			if (!paramIt->second.getIsOptional())
				if ( step.outputs.find(paramIt->first) == step.outputs.end()) //2DO: no outputs here
					errors.push_back("mandatory output: '" + string(paramIt->first) +  "' of module '"+step.module+"' is not set!");

		}*/

		//check, if all params given in yaml do exist in Metdata
		for(auto paramIt = step.params.cbegin(); paramIt != step.params.end(); ++paramIt) {
			try
			{
				auto paramDescription = module.getParam(paramIt->first);
			}
			catch(InvalidConfigException& ex)
			{
				errors.push_back(ex.what());
				affectedSteps.push_back(step.name);
			}
		}

	}

	// check for circular dependencies
	vector<string> circleSteps = detectCircularDependencies();
	if (circleSteps.size() > 0) {
		auto it = circleSteps.begin();
		string stepNames = *it;
		affectedSteps.push_back(*it);
		++it;
		for (; it!=circleSteps.end(); ++it) {
			stepNames += string(", ") + *it;
			affectedSteps.push_back(*it);
		}
		errors.push_back( string("Circular dependency detected between the following configuration steps: ") + stepNames);
	}

	return pair< vector<string>, vector<string> >(errors, affectedSteps);
}

// detect circular dependencies in the inputs
vector<string> Configuration::detectCircularDependencies() {

	// list of names that are involved in the circular dependency
	vector<string> stepNames;

	// detect circular dependencies
	map<string, ProcessingStep> chainTmp;
	chainTmp.insert(chain_.begin(), chain_.end());

	// contains the names of the processing steps in the correct order
	vector<string> sortedChain;

	// boolean, describes, whether any elem could be added
	bool elemWasAdded;

	// iterate over all processing steps and order them
	while(!chainTmp.empty()){
		// initially set to false
		elemWasAdded = false;
		map<string, ProcessingStep>::iterator itProSt = chainTmp.begin();
		while(itProSt!=chainTmp.end()) {
			// add all modules without any dependencies
			if(itProSt->second.inputs.size() == 0){
				sortedChain.push_back(itProSt->first);
				// delete and set pointer to next element
				itProSt = chainTmp.erase(itProSt);
				// an elem was added
				elemWasAdded = true;
			} else {
				// go through dependencies, and add only the modules, where module
				// on which they depend have already been added
				map<string, pair<string,string> >::iterator it = itProSt->second.inputs.begin();
				int i = 1;
				for (; it!=itProSt->second.inputs.end(); ++it) {
					// skip empty references (unset optional input)
					if (it->second.first.empty()) {
						continue;
					}
					if (find(sortedChain.begin(), sortedChain.end(), it->second.first) != sortedChain.end()){
						i *=1;
					} else{
						i *=0;
					}
				}
				if (i == 1){
					sortedChain.push_back(itProSt->first);
					// delete and set pointer to next element
					itProSt = chainTmp.erase(itProSt);
					// an elem was added
					elemWasAdded = true;
				} else {
					// try next element
					++itProSt;
				}
			}
		}
		if(!elemWasAdded){ // a circle has been detected

			// detect only the concrete circle, and not also all the steps, which depend on it

			// find out, which modules belong directly to the chain, and don't only get inputs from it
			map<string, ProcessingStep> chainCircle;
			chainCircle.insert(chainTmp.begin(), chainTmp.end());

			// contains the names of the processing steps, which don't belong to the circle, but only point on it
			vector<string> restChain;

			bool elemWasAdded2 = true;
			// iterate over all processing steps and move all, which don't belong to the chain into restChain
			while(elemWasAdded2){
				// initially set to false
				elemWasAdded2 = false;

				// list, which contains all modules, which provide a used output
				list<string> modulesProvideOutput;

				map<string, ProcessingStep>::iterator itProSt = chainCircle.begin();
				for(;itProSt!=chainCircle.end();++itProSt) {
					// Input-Map of current Step in the circle chain
					map<string, pair<string, string> > tempInp = itProSt->second.inputs;
					map<string, pair<string, string> >::iterator itInp = tempInp.begin();
					for(;itInp!=tempInp.end();++itInp) {
						// skip empty references (unset optional input)
						if (itInp->second.first.empty()) {
							continue;
						}
						modulesProvideOutput.push_front(itInp->second.first);
					}
				}
				modulesProvideOutput.sort();
				modulesProvideOutput.unique();

				itProSt = chainCircle.begin();
				while(itProSt!=chainCircle.end()) {
					// check, whether a module does not appear in the list (module does not have an output, or it is unused)
					list<string> help = modulesProvideOutput;
					help.push_front(itProSt->first);
					help.sort();
					help.unique();
					if(help.size() > modulesProvideOutput.size()) {
						restChain.push_back(itProSt->first);
						// delete and set pointer to next element
						itProSt = chainCircle.erase(itProSt);
						// an elem was added
						elemWasAdded2 = true;
					} else {
					// try next element
						++itProSt;
					}
				}
			}

			// TODO divide the circles, when there are many

			// fill list of names that are involved in the circular dependency
			for (auto it = chainCircle.begin(); it!=chainCircle.end(); ++it) {
				stepNames.push_back(it->first);
			}
			break;
		}
	}

	return stepNames;
}

// prints the current configurations yaml structure
/*
see also https://github.com/jbeder/yaml-cpp/wiki/How-To-Emit-YAML
*/
void Configuration::print() {

	string out = getYAML();

	LOG_I(out);
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
map<string, ProcessingStep> Configuration::getProcessingChain() const {
	return chain_;
}

// returns a value indicating whether a named processing step exists
bool Configuration::hasProcessingStep(string name) const {
	return chain_.count(name) != 0;
}

// returns the named processing step
ProcessingStep Configuration::getProcessingStep(string name) const {
	return chain_.at(name);
}

// adds a ProcessingStep to the chain
/*
 ProSt 		processing step, which is added to the chain
*/
void Configuration::addProcessingStep(ProcessingStep proSt){
	chain_.insert(pair<std::string, ProcessingStep> (proSt.name, proSt));
}

// removes a ProcessingStep from the chain
/*
stepName	name of processingStep, which has to been deleted from the chain
*/
void Configuration::removeProcessingStep(string stepName){
	chain_.erase(stepName);
}

// rename a ProcessingStep
/*
oldName	name of processingStep, which has to be renamed
newName	name of processingStep, which has to be renamed

returns true on success, false on failure
*/
bool Configuration::renameProcessingStep(string oldName, string newName){

	// checks whether the name has been changed
	if (oldName.compare(newName) == 0) {
		return false;
	}

	// check whether the new name does not already exist
	if (chain_.count(newName) == 0) {

		// create the processing step with the old name and the processing step with the new name
		ProcessingStep newStep = chain_[oldName];
		newStep.name = newName;

		// remove the processing step with the old name and add the processing step with the new name
		chain_.erase(oldName);
		chain_.insert(pair<string, ProcessingStep> (newStep.name, newStep));

		// update all the references
		for(auto it = chain_.begin(); it != chain_.end(); ++it) {

			ProcessingStep& step = it->second;
			for(auto inputIt = step.inputs.begin(); inputIt != step.inputs.end(); ++inputIt) {

				// if this input refers to the old step, rename it
				if (oldName.compare(inputIt->second.first) == 0) {
					step.inputs[inputIt->first].first = newName;
				}
			}
		}

		return true;
	} else {
		// name already exists - dont allow the change
		return false;
	}
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
