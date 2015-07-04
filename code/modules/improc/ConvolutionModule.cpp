#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "ConvolutionModule.hpp"

using namespace std;
using namespace uipf;


// runs the module
/*
data	DataManager handles the input and ouput of this module
*/
void ConvolutionModule::run( DataManager& data) const
{
	using namespace cv;

	// read the params (window size and sigma) given for this step
	int anchorX = data.getParam<int>("anchorX", -1);
	int anchorY = data.getParam<int>("anchorY", -1);
	double delta = data.getParam<double>("delta", 0);

	// get a pointers to the input data
	Matrix::c_ptr pImage = data.getInputData<Matrix>("image");
	Matrix::c_ptr pKernel = data.getInputData<Matrix>("kernel");
	// get the actual opencv matrix of the input data
	Mat image = pImage->getContent();
	Mat kernel = pKernel->getContent();

	Mat out;
	Point anchor = Point( anchorX, anchorY );
	filter2D(image, out, -1, kernel, anchor, delta, BORDER_DEFAULT );

	// set the result (output) on the datamanager
	data.setOutputData("image", new Matrix(out));

}

// returns the meta data of this module
MetaData ConvolutionModule::getMetaData() const
{
	map<string, DataDescription> input = {
		{"image", DataDescription(MATRIX, "the image to apply the filter on.") },
		{"kernel", DataDescription(MATRIX, "the image to use as the filter kernel.") }
	};
	map<string, DataDescription> output = {
		{"image", DataDescription(MATRIX, "the result image.") }
	};
	map<string, ParamDescription> params = {
		{"anchorX", ParamDescription("X coordinate of the kernel anchor point. Defaults to -1, which will use the kernel center.", true) },
		{"anchorY", ParamDescription("Y coordinate of the kernel anchor point. Defaults to -1, which will use the kernel center.", true) },
		{"delta", ParamDescription("optional value added to the filtered pixels before storing them in dst. Defaults 0.", true) }
	};

	return MetaData(
		"Applies Convolution filter to an image using openCV.",
		"Image Processing",
		input,
		output,
		params
	);
}

