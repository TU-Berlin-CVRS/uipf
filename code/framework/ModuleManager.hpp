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
		ModuleManager();
		// destructor
		~ModuleManager(void){};

		// runs the module chain with the params
		void run(Configuration);

		MetaData getModuleMetaData(const std::string& name);

	private:
		// module chain
		
		ModuleInterface* loadModule(const std::string& name);

		void initModules();
		
		std::map<std::string,ModuleInterface*> plugins_;

};

}

