#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "GaussianModule.hpp"

using namespace std;
using namespace uipf;


// runs the module
/*
data	DataManager handles the input and ouput of this module
*/
void GaussianModule::run( DataManager& data) const
{
	using namespace cv;

	// print params for debugging
	data.listParams();

	// read the params (window size and sigma) given for this step
	int nWindowSize = data.getParam<int>("windowSize", 0);
	double sigmaX = data.getParam<double>("sigmaX", 0.0);
	double sigmaY = data.getParam<double>("sigmaY", 0.0);

	// get a pointer to the "image" input data
	Matrix::c_ptr oMatrix = data.getInputData<Matrix>("image");
	// get the actual opencv matrix of the input data
	Mat m = oMatrix->getContent();

	// do gaussian blur using opencv
	GaussianBlur(m,m,Size( nWindowSize, nWindowSize ), sigmaX, sigmaY );

	// set the result (output) on the datamanager
	data.setOutputData("image",new Matrix(m));

}

// returns the meta data of this module
MetaData GaussianModule::getMetaData() const
{
	map<string, DataDescription> input = {
		{"image", DataDescription(MATRIX, "the image to apply the filter on.") }
	};
	map<string, DataDescription> output = {
		{"image", DataDescription(MATRIX, "the result image.") }
	};
	map<string, ParamDescription> params = {
		{"windowSize", ParamDescription("window size of the kernel. Must be an odd number. Optional, defaults to 0 which means that it is calculated based on sigma.", true) },
		{"sigmaX", ParamDescription("variance of the gaussian kernel in X direction.") },
		{"sigmaY", ParamDescription("variance of the gaussian kernel in Y direction. Optional, defaults to sigmaX.", true) }
	};

	return MetaData(
		"Applies Gaussian blurring to an image using openCV.",
		"Image Processing",
		input,
		output,
		params
	);
}

