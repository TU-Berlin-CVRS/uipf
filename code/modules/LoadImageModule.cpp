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
LoadImageModule::run (map < string, Data::ptr >&input, map < string, string >& params, map < string, Data::ptr >&output ) const 
{

  using namespace cv;
  Mat image;
  std::string strFilename = params["filename"];
  image = imread (strFilename.c_str (), CV_LOAD_IMAGE_COLOR);	// Read the file

  if (!image.data)// Check for invalid input
    {	
	LOG_E("Could not open or find the image");
	return; //2DO Exception
    }

	
  output.insert (pair < string, Data::ptr >("sourceImage.image", Matrix::ptr(new Matrix(image))));

  listParams(params);
}

std::string LoadImageModule::name () const
{
  return "loadImage";
}
