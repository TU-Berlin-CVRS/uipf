#ifndef MODULECONFIG_H
#define MODULECONFIG_H

#include <opencv2/opencv.hpp>
#include "Elem.hpp"

using namespace cv;

class ModuleConfig{

	public:
		// constructor
		ModuleConfig(void){};
		// destructor
		~ModuleConfig(void){};

	private:
		// task of the module	e.g. storeImage					
		string task;
		
		// name of the module in this configuration		e.g. output 
		string name;			
		
		// input	e.g. <nameOfOtherModule.propertieOfOtherModule>
		vector<Elem> inputVector;
		
		// output	e.g. <fileName>
		vector<Elem> outputVector;		
		

};

#endif




