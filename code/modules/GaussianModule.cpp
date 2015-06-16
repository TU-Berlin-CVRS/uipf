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
	int nWindowSize = data.getParam<int>("windowSize",-1);
	double dSigma = data.getParam<double>("sigma",0.0);

	// get a pointer to the "image" input data
	const Matrix::ptr oMatrix = data.getInputData<Matrix>("image");
	// get the actual opencv matrix of the input data
	Mat m = oMatrix->getContent();

	// do gaussian blur using opencv
	GaussianBlur(m,m,Size( nWindowSize, nWindowSize ), dSigma, dSigma );

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
		{"windowSize", ParamDescription("window size of the kernel.") },
		{"sigma", ParamDescription("variance of the gaussian kernel.") }
	};

	return MetaData(
		"Applies Gaussian blurring to an image.",
		input,
		output,
		params
	);
}

