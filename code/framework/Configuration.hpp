#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <opencv2/opencv.hpp>

using namespace cv;

class Configuration{

	public:
		// constructor
		Configuration(void){};
		// destructor
		~Configuration(void){};

		// loads the module chain with the params from the config file
		void load(string);
		
		// stores the module chain with the params in the config file
		void store(string);
	
	//~ private:
		// TODO properties

};

#endif

