#include <opencv2/opencv.hpp>
#include "Configuration.hpp"


namespace uipf{
	
class ModuleManager{

	public:
		// constructor
		// loads the module chain with the params from the configuration object
		ModuleManager(uipf::Configuration);
		// destructor
		~ModuleManager(void){};

		// runs the module chain with the params
		void run();

	private:
		// module chain
		uipf::Configuration config;

};

}

