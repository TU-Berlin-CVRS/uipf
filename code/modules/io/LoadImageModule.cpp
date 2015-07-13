#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "LoadImageModule.hpp"
#include "../framework/types/Matrix.hpp"

using namespace std;
using namespace uipf;


/*

*/
void LoadImageModule::run( DataManager& data) const
{
	 using namespace cv;

	  Mat image;
	  std::string strFilename = data.getParam<std::string>("filename","");
	  // check whether to load the image in grayscale mode, defaults to color
	  if (data.getParam<std::string>("mode","color").compare("grayscale") == 0)
	  {
	    image = imread (strFilename.c_str (), CV_LOAD_IMAGE_GRAYSCALE);	// Read the file
	  }
	  else
	  {
	    image = imread (strFilename.c_str (), CV_LOAD_IMAGE_COLOR);	// Read the file
	  }

	  if (!image.data)// Check for invalid input
	  {
	    throw ErrorException(string("Could not open or find the image: ") + strFilename);
	  }

	  data.setOutputData("image",new Matrix(image));

}

MetaData LoadImageModule::getMetaData() const
{
	map<string, DataDescription> output = {
		{"image", DataDescription(MATRIX, "the loaded image.") }
	};
	map<string, ParamDescription> params = {
		{"filename", ParamDescription("file name of the file to load from.") },
		{"mode", ParamDescription("can be either 'color' or 'grayscale' for whether to load the image in color or grayscale mode. Defaults to 'color'.",true) },
	};

	return MetaData(
		"Load an image from a file.",
		"I/O",
		map<string, DataDescription>(), // no inputs
		output,
		params
	);
}

