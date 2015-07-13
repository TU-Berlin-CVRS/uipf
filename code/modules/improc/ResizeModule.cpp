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

	// read the params (width and height) given for this step
	double width = data.getParam<double>("width", 0.0);
	double height = data.getParam<double>("height", 0.0);

	// get a pointer to the "image" input data
	Matrix::c_ptr oMatrix = data.getInputData<Matrix>("image");

	// get the actual opencv matrix of the input data
    Mat m = oMatrix->getContent();

	// do resize using opencv
    resize(m,m, Size (width,height));

	// set the result (image) on the datamanager
	data.setOutputData("image",new Matrix(m));

}

// returns the meta data of this module
MetaData ResizeModule::getMetaData() const
{
	DataDescriptionMap input = {
		{"image", DataDescription(MATRIX, "the image to resize.") }
	};

	DataDescriptionMap output = {
		{"image", DataDescription(MATRIX, "the result image.") }
	};

	ParamDescriptionMap params = {
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

