#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include "StoreImageModule.hpp"

using namespace std;
namespace uipf
{

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
        using namespace utils;
        
	Matrix* oMatrix = getData<Matrix>(input,"image");

	if (oMatrix) {
		std::string strFilename = getParam<std::string>(params,"filename","noname.png");
		int nQuality = getParam<int>(params,"quality",-1);
		if (nQuality != -1)
		{
		        std::vector<int> compression_params;
		        if (endswith(strFilename,"jpeg") || endswith(strFilename,"jpg"))        
        		    compression_params.push_back(CV_IMWRITE_JPEG_QUALITY);        		 
        		else if (endswith(strFilename,"png"))
        		    compression_params.push_back(CV_IMWRITE_PNG_COMPRESSION);
        		else if (endswith(strFilename,"bmp") || endswith(strFilename,"ppm") || endswith(strFilename,"pgm"))
        		    compression_params.push_back(CV_IMWRITE_PXM_BINARY);
        		    
                        compression_params.push_back(nQuality);


                         try {
                                imwrite(strFilename.c_str(), oMatrix->getContent(), compression_params);
                        }
                        catch (runtime_error& ex) {
                                LOG_E( "Exception converting image");
                        
                        }
                }       
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
		{"filename", ParamDescription("file name of the file to save to. imageformat is derived by fileending automatically.") },
		{"quality", ParamDescription("compression quality (optional)") }		
	};

	return MetaData(
		"Store an image to a file.",
		input,
		map<string, DataDescription>(), // no outputs
		params
	);
}

}//namespace

