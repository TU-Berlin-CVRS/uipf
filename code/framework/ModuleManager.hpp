#ifndef MODULEMANAGER_H
#define MODULEMANAGER_H

#include <opencv2/opencv.hpp>
#include <map>
#include <QPluginLoader>

#include "Configuration.hpp"
#include "ModuleInterface.hpp"
#include "MetaData.hpp"

namespace uipf{

class ModuleManager{

	public:
		// constructor
		ModuleManager();
		// destructor
		~ModuleManager(void);

		// runs the module chain with the params
		void run(Configuration);

		// returns meta data information for a named module
		MetaData getModuleMetaData(const std::string& name);

		// returns a value indicating whether the named module exists
		bool hasModule(const std::string& name);

		// returns a map of module name => meta data
		std::map<std::string, MetaData> getAllModuleMetaData();

		// if set modules should finish their work
		void requestStop(){ context_.bStopRequested_ = true; }

		// set pause and resume to enable blocking windows in GUI mode
		void pauseChain(){ context_.bPaused_ = true; }
		void resumeChain(){ context_.bPaused_ = false; }

		//tell modules that we have a GUI
		void setHaveGUI(){ context_.bHasGUI_ = true; }

	private:

		// instantiate a named module
		ModuleInterface* loadModule(const std::string& name);

		// check which modules exist and populate  plugins_
		void initModules();

		// map: module name -> plugin loader instance which can instantiate a module
		std::map<std::string, QPluginLoader*> plugins_;

		Context context_;
};

} // namespace

#endif
