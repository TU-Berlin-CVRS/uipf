#include <opencv2/opencv.hpp>
#include "Configuration.hpp"

using namespace cv;

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

};

