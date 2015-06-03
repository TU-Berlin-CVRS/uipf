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
void StoreImageModule::run(map<string, Data::ptr >& input, map<string, string>& params, map<string, Data::ptr >& output) const
{
using namespace cv;
/////////2DO remove when yaml ordering is working////////////////////
        listParams(params);
 Mat image;
  std::string strInFilename = getParam<std::string>(params,"infilename","");

  image = imread (strInFilename.c_str (), CV_LOAD_IMAGE_COLOR);	// Read the file

  if (!image.data)// Check for invalid input
    {	
	LOG_E("Could not open or find the image");
	return; //2DO Exception
    }

  input.insert (pair < string, Data::ptr >("process.image", Matrix::ptr(new Matrix(image))));

/////////2DO remove when yaml ordering is working////////////////////
	
	Matrix::ptr oMatrix = getData<Matrix>(input,"process.image");
	  
        if (oMatrix)
        {
  	         std::string strFilename = getParam<std::string>(params,"filename","noname.png");
         	 imwrite( strFilename.c_str(), oMatrix->getContent() );
  	
  	}
}

std::string StoreImageModule::name() const
{
	return "storeImage";
}
