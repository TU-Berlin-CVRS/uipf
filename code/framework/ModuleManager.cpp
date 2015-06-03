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

	// TODO sort processing steps

	// get processing chain
	map<string, ProcessingStep> chain = config_.getProcessingChain();

	Context context;

	// iterate over all modules and run them
	map<string, ProcessingStep>::iterator it = chain.begin();
	for (; it!=chain.end(); ++it) {

		// load the module
		ModuleInterface* mod = loadModule(it->second.module);
		ModuleBase* modbase = dynamic_cast<ModuleBase*>(mod);
		if (modbase)
			modbase->setContext(&context);
		
		
		// TODO fill outputs with inputs from other modules
		map<string, Data::ptr > inputs;
		map<string, Data::ptr > outputs;

		LOG_I( "Running step '" + it->first + "'..." );
		mod->run(inputs, it->second.params, outputs);
		LOG_I( "Done with step '" + it->first + "'." );

		// TODO store outputs somewhere
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
		ModuleInterface* iModule =
		    qobject_cast<ModuleInterface* >(plugin);

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



