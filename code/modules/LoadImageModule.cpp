#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "LoadImageModule.hpp"
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
LoadImageModule::run (map < string, Data::ptr& >&input, map < string, string >& params, map < string, Data::ptr >&output ) const
{
  using namespace cv;

  Mat image;
  std::string strFilename = params["filename"];
  // check whether to load the image in grayscale mode, defaults to color
  if (params.count("mode") > 0 && params["mode"].compare("grayscale") == 0) {
    image = imread (strFilename.c_str (), CV_LOAD_IMAGE_GRAYSCALE);	// Read the file
  } else {
    image = imread (strFilename.c_str (), CV_LOAD_IMAGE_COLOR);	// Read the file
  }

  if (!image.data)// Check for invalid input
  {
    throw new ErrorException(string("Could not open or find the image: ") + strFilename);
  }

  output.insert (pair < string, Data::ptr >("image", Matrix::ptr(new Matrix(image))));

}

/*

*/
void LoadImageModule::run( DataManager& data) const
{
	data.listParams();
	throw InvalidConfigException("test");
}

MetaData LoadImageModule::getMetaData() const
{
	map<string, DataDescription> output = {
		{"image", DataDescription(MATRIX, "the loaded image.") }
	};
	map<string, ParamDescription> params = {
		{"filename", ParamDescription("file name of the file to load from.") },
		{"mode", ParamDescription("can be either 'color' or 'grayscale' for whether to load the image in color or grayscale mode. Defaults to 'color'.") },
	};

	return MetaData(
		"Load an image from a file.",
		map<string, DataDescription>(), // no inputs
		output,
		params
	);
}

