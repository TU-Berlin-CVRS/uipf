#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "GaussianModule.hpp"

using namespace std;
using namespace uipf;


/*

*/
void GaussianModule::run( DataManager& data) const
{
	using namespace cv;

	data.listParams();
	const Matrix* oMatrix = data.getInputData<Matrix>("image");

	int nWindowSize = data.getParam<int>("windowSize",-1);
	double dSigma = data.getParam<double>("sigma",0.0);

	cv::Mat m = oMatrix->getContent(); //2DO cloning whith clear output naming

	GaussianBlur(m,m,Size( nWindowSize, nWindowSize ), dSigma, dSigma );
	//2DO naming unclear: is it just "image" or "process.image"??? What wins: Metadata or yaml-config-file?
	data.setOutputData("image",new Matrix(m));

}

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

