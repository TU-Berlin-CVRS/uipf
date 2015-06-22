#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include "ShowImageModule.hpp"
#include "../framework/types/Matrix.hpp"

using namespace std;
using namespace uipf;

/*

*/
void ShowImageModule::run( DataManager& data) const
{
	using namespace cv;

	Matrix::c_ptr oMatrix = data.getInputData<Matrix>("image");
	if (oMatrix) {
		context_->displayImage(data.getParam<std::string>("title","view image"), *oMatrix,data.getParam<bool>("blocking",true));
	}
}

MetaData ShowImageModule::getMetaData() const
{
	map<string, DataDescription> input = {
		{"image", DataDescription(MATRIX, "the image to show.") },

	};
	map<string, ParamDescription> params = {
		{"title", ParamDescription("the title of the window.") },
		{"blocking", ParamDescription("'true' or 'false' determines if the processing chain has to wait for this window to close or not.",true) }
	};

	return MetaData(
		"Show an image by opening a window.",
		input,
		map<string, DataDescription>(), // no outputs
		params
	);
}

