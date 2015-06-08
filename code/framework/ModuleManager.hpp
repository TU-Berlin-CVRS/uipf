#include <opencv2/opencv.hpp>
#include "Configuration.hpp"
#include "ModuleInterface.hpp"
#include "MetaData.hpp"
#include <QObjectList>
#include <QPluginLoader>
#include <map>

namespace uipf{

class ModuleManager{

	public:
		// constructor
		ModuleManager();
		// destructor
		~ModuleManager(void);

		// runs the module chain with the params
		void run(Configuration);

		MetaData getModuleMetaData(const std::string& name);

	private:
		// module chain

		ModuleInterface* loadModule(const std::string& name);

		bool hasModule(const std::string& name);
		
		void initModules();

		// map: module name -> plugin loader instance which can instantiate a module
		std::map<std::string, QPluginLoader*> plugins_;

};

}

