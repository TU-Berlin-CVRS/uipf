#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include "StoreImageModule.hpp"

using namespace std;
using namespace uipf;

// executes the Module
/*
input 		is a std::map of input resources, the names are described in the module meta description
params 		is a std::map of input paramaeters, the names are described in the module meta description
ouput 		is a std::map of output resources, the names are described in the module meta description
context 	is a container providing access to the current environment, allowing to open windows, write to logger etc...
*/
void StoreImageModule::run(map<string, Data::ptr& >& input, map<string, string>& params, map<string, Data::ptr >& output) const
{
	using namespace cv;

	Matrix* oMatrix = getData<Matrix>(input,"image");

	if (oMatrix) {
		std::string strFilename = getParam<std::string>(params,"filename","noname.png");
		imwrite( strFilename.c_str(), oMatrix->getContent() );
	}
}

std::string StoreImageModule::name() const
{
	return "storeImage";
}

MetaData StoreImageModule::getMetaData() const
{
	map<string, DataDescription> input = {
		{"image", DataDescription(MATRIX, "the image to save.") }
	};
	map<string, ParamDescription> params = {
		{"filename", ParamDescription("file name of the file to save to.") }
		// TODO cover more parameters from http://docs.opencv.org/modules/highgui/doc/reading_and_writing_images_and_video.html#imwrite, e.g. store as PNG or JPEG
	};

	return MetaData(
		"Store an image to a file.",
		input,
		map<string, DataDescription>(), // no outputs
		params
	);
}

