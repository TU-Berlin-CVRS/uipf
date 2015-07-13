#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "ResizeModule.hpp"

using namespace std;
using namespace uipf;


// runs the module
/*
data	DataManager handles the input and ouput of this module
*/
void ResizeModule::run( DataManager& data) const
{
	using namespace cv;

	// print params for debugging
	data.listParams();

	// read the params (window size and sigma) given for this step
	double width = data.getParam<double>("width", 0.0);
	double height = data.getParam<double>("height", 0.0);

    LOG_I("w:" + utils::toString(width) + " h:" + utils::toString(height));
	// get a pointer to the "image" input data
	Matrix::c_ptr oMatrix = data.getInputData<Matrix>("image");
	// get the actual opencv matrix of the input data
    Mat src = oMatrix->getContent();
	Mat dst;

	// do gaussian blur using opencv
    Size s(width,height);
    resize(src,dst,s);

	// set the result (output) on the datamanager
	data.setOutputData("image",new Matrix(dst));

}

// returns the meta data of this module
MetaData ResizeModule::getMetaData() const
{
	map<string, DataDescription> input = {
		{"image", DataDescription(MATRIX, "the image to resize.") }
	};
	map<string, DataDescription> output = {
		{"image", DataDescription(MATRIX, "the result image.") }
	};
	map<string, ParamDescription> params = {
		{"width", ParamDescription("new width") },
        {"height", ParamDescription("new height") }
	};

	return MetaData(
		"Resizes an image using openCV.",
		"Image Processing",
		input,
		output,
		params
	);
}

