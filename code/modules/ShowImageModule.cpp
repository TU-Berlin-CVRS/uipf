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
ShowImageModule::run (map < string, Data::ptr >&input, map < string,string >& params, map < string, Data::ptr >&output) const 
{

  using namespace cv;
//////2DO remove when yaml ordering works///////////
  Mat image;
  std::string strFilename = params["filename"];
  image = imread (strFilename.c_str (), CV_LOAD_IMAGE_COLOR);	// Read the file

  if (!image.data) // Check for invalid input
    {
	LOG_E("Could not open or find the image");
       return; //2DO Exception
    }

	
  input.insert (pair < string, Data::ptr >("sourceImage.image", Matrix::ptr(new Matrix(image))));

//////2DO remove when yaml ordering works///////////


  Matrix::ptr oMatrix = getData<Matrix>(input,"sourceImage.image");
  if (oMatrix)
  	context_->displayImage(getParam<std::string>(params,"title","no title"), *oMatrix);
}

std::string ShowImageModule::name () const
{
  return "showImage";
}
