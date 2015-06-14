#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include "ShowImageModule.hpp"
#include "../framework/types/Matrix.hpp"

using namespace std;
using namespace uipf;

// executes the Module
/*
input 		is a std::map of input resources, the names are described in the module meta description
params 		is a std::map of input paramaeters, the names are described in the module meta description
ouput 		is a std::map of output resources, the names are described in the module meta description
context 	is a container providing access to the current environment, allowing to open windows, write to logger etc...
*/
void
ShowImageModule::run (map < string, Data::ptr& >&input, map < string,string >& params, map < string, Data::ptr >&output) const
{
	using namespace cv;

	Matrix* oMatrix = getData<Matrix>(input,"image");
	if (oMatrix) {
		context_->displayImage(getParam<std::string>(params,"title","view image"), *oMatrix);
	}
}

/*

*/
void ShowImageModule::run( DataManager& data) const
{
	data.listParams();
	throw InvalidConfigException("test");
}

MetaData ShowImageModule::getMetaData() const
{
	map<string, DataDescription> input = {
		{"image", DataDescription(MATRIX, "the image to show.") }
	};
	map<string, ParamDescription> params = {
		{"title", ParamDescription("the title of the window.") }
	};

	return MetaData(
		"Show an image by opening a window.",
		input,
		map<string, DataDescription>(), // no outputs
		params
	);
}

