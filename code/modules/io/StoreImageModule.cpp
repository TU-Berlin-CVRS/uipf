#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <stdexcept>

#include "StoreImageModule.hpp"

using namespace std;
namespace uipf
{

/*

*/
void StoreImageModule::run( DataManager& data) const
{
	using namespace cv;
	using namespace utils;

	Matrix::c_ptr oMatrix = data.getInputData<Matrix>("image");

	if (oMatrix) {
		std::string strFilename = data.getParam<std::string>("filename", "");
		if (strFilename.empty()) {
			strFilename = context_->getProcessingStepName() + string(".png");
		}

		int nQuality = data.getParam<int>("quality",-1);
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
				LOG_I( "Image written to file " + strFilename );
			}
			catch (runtime_error& ex) {
				LOG_E( "Exception converting image");
				return;
			}
		} else {
			imwrite( strFilename.c_str(), oMatrix->getContent() );
			LOG_I( "Image written to file " + strFilename );
		}
	}
}

MetaData StoreImageModule::getMetaData() const
{
	map<string, DataDescription> input = {
		{"image", DataDescription(MATRIX, "the image to save.") }
	};
	map<string, ParamDescription> params = {
		{"filename", ParamDescription("file name of the file to save to. imageformat is derived by fileending automatically. Defaults to '<step name>.png'.", true) },
		{"quality", ParamDescription("compression quality (optional)", true) }
	};

	return MetaData(
		"Store an image to a file.",
		"I/O",
		input,
		map<string, DataDescription>(), // no outputs
		params
	);
}

}//namespace

