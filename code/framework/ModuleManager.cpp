#include "ModuleManager.hpp"


#include <QPluginLoader>
#include <QApplication>
#include <QObject>
#include <QDir>
#include <QObjectList>
#include <QStringList>
#include <string>
#include "Logger.hpp"
#include "ModuleBase.hpp"
#include "ModuleInterface.hpp"
#include "ErrorException.hpp"
#include "InvalidConfigException.hpp"
#include "DataManager.hpp"

using namespace uipf;
using namespace std;

// loads the configuration and stores in the ModuleManager
/*
conf	Configuration file, which has to be executed
*/
ModuleManager::ModuleManager() {
	initModules();
}

ModuleManager::~ModuleManager()
{
	// delete all the pluginloaders
	map<string, QPluginLoader*>::iterator it = plugins_.begin();
	for (; it!=plugins_.end(); ++it) {
		delete it->second;
	}
}

// executes the Configuration file
/*
*/
void ModuleManager::run(Configuration config){

	// get processing chain
	map<string, ProcessingStep> chain = config.getProcessingChain();

	map<string, ProcessingStep> chainTmp;
	chainTmp.insert(chain.begin(), chain.end());

	// contains the names of the processing steps in the correct order
	vector<string> sortedChain;


	// iterate over all processing steps and order them
	while(!chainTmp.empty()){
		map<string, ProcessingStep>::iterator itProSt = chainTmp.begin();
		while(itProSt!=chainTmp.end()) {

			// add all modules without any dependencies
			if(itProSt->second.inputs.size() == 0){
				sortedChain.push_back(itProSt->first);
				// delete and set pointer to next element
				itProSt = chainTmp.erase(itProSt);
			} else {
				// go through dependencies, and add only the modules, where module
				// on which they depend have already been added
				map<string, pair<string,string> >::iterator it = itProSt->second.inputs.begin();
				int i = 1;
				for (; it!=itProSt->second.inputs.end(); ++it) {
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
				} else {
					// try next element
					++itProSt;
				}
			}
		}
	}

	Context context;

	// contains the outputs of the processing steps
	map<string, map<string, Data::ptr>* > stepsOutputs;

	LOG_I( "Starting processing chain." );

	// run over the sortedChain and run the modules in the order given by the chain
	for (unsigned int i=0; i<sortedChain.size(); i++){

		ProcessingStep proSt = chain[sortedChain[i]];

		// load the module
		ModuleInterface* module;
		string moduleName = proSt.module;
		if (hasModule(moduleName)) {
			module = loadModule(moduleName);
			module->setContext(&context);

		} else {
			LOG_E( "Module '" + moduleName + "' could not be found." );
			break;
		}

		// prepare an empty map of outputs that will be filled by the module
		map<string, Data::ptr >* outputs = new map<string, Data::ptr>();
		// inputs are references to the data pointer to allow sending one output to multiple steps
		map<string, Data::ptr& > inputs;

		// fill the inputs of the current processing step by taking it from the stored outputs
		map<string, pair<string, string> >::iterator it = proSt.inputs.begin();
		for (; it!=proSt.inputs.end(); ++it) {
			map<string, Data::ptr>* out = stepsOutputs[it->second.first];
			Data::ptr& pt = out->find(it->second.second)->second;
			inputs.insert(pair<string, Data::ptr&>(it->first, pt));
		}

		LOG_I( "Running step '" + proSt.name + "'..." );

		try {
			DataManager dataMnrg(inputs, proSt.params, *outputs);
			module->run(dataMnrg);

		} catch (ErrorException e) {
			LOG_E( string("Error: ") + e.what() );
			break;
		} catch (InvalidConfigException e) {
			LOG_E( string("Invalid config: ") + e.what() );
			break;
		} catch (std::exception e) {
			LOG_E( string("Error: module threw exception: ") + e.what() );
			break;
		}

		LOG_I( "Done with step '" + proSt.name + "'." );

		// fill the outputs of the current processing step
		stepsOutputs.insert(pair<string, map<string, Data::ptr>* > (proSt.name, outputs));

		// TODO delete module, check for side effects with the data pointers first
	}

	// delete the ouput map
	map<string, map<string, Data::ptr>* >::iterator it = stepsOutputs.begin();
	for (; it!=stepsOutputs.end(); ++it) {
		delete it->second;
	}

	LOG_I( "Finished processing chain." );
}

// initialize all modules by creating a map of module names and the plugin loader instance
// that can be used later to instantiate the module
void ModuleManager::initModules()
{
	QDir pluginsDir = QDir(qApp->applicationDirPath());

	foreach (QString fileName, pluginsDir.entryList(QDir::Files))
	{
		QPluginLoader* loader = new QPluginLoader(pluginsDir.absoluteFilePath(fileName));
		QObject *plugin = loader->instance();
		if (plugin) {
			Logger::instance()->Info("found module: " + fileName.toStdString());
			ModuleInterface* iModule = qobject_cast<ModuleInterface* >(plugin);

			plugins_.insert( std::pair<std::string, QPluginLoader*>(iModule->name(), loader) );
			delete iModule;
		}

	}

}

// check whether a module exists
bool ModuleManager::hasModule(const std::string& name){
	return (plugins_.count(name) > 0);
}

// creates an instance of a module and returns it
ModuleInterface* ModuleManager::loadModule(const std::string& name)
{
	if (plugins_.count(name) > 0) {

		QPluginLoader* loader = plugins_[name];
		QObject *plugin = loader->instance();
		if (plugin) {
			Logger::instance()->Info("load module: " + name);
			return qobject_cast<ModuleInterface* >(plugin);
		}
	}
	return nullptr;

}

// returns the meta data of a module
MetaData ModuleManager::getModuleMetaData(const std::string& name)
{
	ModuleInterface* module = loadModule(name);
	MetaData metaData = module->getMetaData();
	delete module;
	return metaData;
}

// returns the meta data for all modules indexed by module name
map<string, MetaData> ModuleManager::getAllModuleMetaData()
{
	map<string, MetaData> result;
	for(auto it = plugins_.cbegin(); it != plugins_.end(); ++it) {
		result.insert( pair<string, MetaData>( it->first, getModuleMetaData(it->first) ) );
	}
	return result;
}
