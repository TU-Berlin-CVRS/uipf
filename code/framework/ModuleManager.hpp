#include <opencv2/opencv.hpp>
#include "Configuration.hpp"

using namespace cv;

class ModuleManager{

	public:
		// constructor
		ModuleManager(void){};
		// destructor
		~ModuleManager(void){};

		// loads the module chain with the params from the configuration object
		void load(Configuration&);
		
		// runs the module chain with the params
		void run();

	private:
		// module chain
		Configuration config;

};

