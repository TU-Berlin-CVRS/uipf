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

using namespace uipf;
using namespace std;

// loads the configuration and stores in the ModuleManager
/*
conf	Configuration file, which has to be executed
*/
ModuleManager::ModuleManager(Configuration conf)
 : config_(conf)
{
	initModules();
}

// executes the Configuration file
/*
*/
void ModuleManager::run(){

	// get processing chain
	map<string, ProcessingStep> chain = config_.getProcessingChain();

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

	// run over the sortedChain and run the modules in the order given by the chain
	for (int i=0; i<sortedChain.size(); i++){

		ProcessingStep proSt = chain[sortedChain[i]];

		// load the module
		ModuleInterface* mod = loadModule(proSt.module);
		ModuleBase* modbase = dynamic_cast<ModuleBase*>(mod);
		if (modbase){
			modbase->setContext(&context);
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
		mod->run(inputs, proSt.params, *outputs);
		LOG_I( "Done with step '" + proSt.name + "'." );

		// fill the outputs of the current processing step
		stepsOutputs.insert(pair<string, map<string, Data::ptr>* > (proSt.name, outputs));
	}

	// delete the ouput map
	map<string, map<string, Data::ptr>* >::iterator it = stepsOutputs.begin();
	for (; it!=stepsOutputs.end(); ++it) {
		delete it->second;
	}

}

void ModuleManager::initModules()
{
	QDir pluginsDir = QDir(qApp->applicationDirPath());

	foreach (QString fileName, pluginsDir.entryList(QDir::Files))
	{
		QPluginLoader loader(pluginsDir.absoluteFilePath(fileName));
		QObject *plugin = loader.instance();
		if (plugin) {
			Logger::instance()->Info("found: " + fileName.toStdString());
			ModuleInterface* iModule = qobject_cast<ModuleInterface* >(plugin);

			//2DO: use metadata as key
			plugins_.insert( std::pair<std::string, ModuleInterface* >(iModule->name(), iModule) );

		}

	}

}

ModuleInterface* ModuleManager::loadModule(const std::string& name){

	Logger::instance()->Info("loadModule: " + name);

	return plugins_[name]; //2DO: safety, uppercase lowercase etc.

}

MetaData ModuleManager::getModuleMetaData(const std::string& name){

	// TODO this is dummy

	map<string, Type> in;
	in.insert( pair<string, Type>("image", MATRIX) );

	map<string, Type> out;
	out.insert( pair<string, Type>("image", MATRIX) );

	MetaData meta("Dummy module description for " + name, in, out);

	return meta;

}



