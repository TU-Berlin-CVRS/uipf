#include <opencv2/opencv.hpp>
#include "Configuration.hpp"
#include "ModuleInterface.hpp"
#include "MetaData.hpp"
#include <QObjectList>
#include <map>

namespace uipf{

class ModuleManager{

	public:
		// constructor
		// loads the module chain with the params from the configuration object
		ModuleManager(Configuration);
		// destructor
		~ModuleManager(void){};

		// runs the module chain with the params
		void run();


	private:
		// module chain
		
		ModuleInterface* loadModule(const std::string& name);

		MetaData getModuleMetaData(const std::string& name);
		
		void initModules();

		
		Configuration config_; //Member Variables need to be declared with some kind of underscore notation...

		std::map<std::string,ModuleInterface*> plugins_;

};

}

