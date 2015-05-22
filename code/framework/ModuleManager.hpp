#include <opencv2/opencv.hpp>
#include "Configuration.hpp"
#include "ModuleInterface.hpp"
#include "MetaData.hpp"

// TODO remove this include when dynamic loading works
#include "../modules/DummyModule.hpp"

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
		Configuration config;

		ModuleInterface* loadModule(std::string name);

		MetaData getModuleMetaData(std::string name);

};

}

