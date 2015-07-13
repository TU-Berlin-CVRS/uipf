#include "ShowImageModule.hpp"

using namespace std;
using namespace uipf;

/*

*/
void ShowImageModule::run( DataManager& data) const
{
	using namespace cv;

	Matrix::c_ptr oMatrix = data.getInputData<Matrix>("image");
	if (oMatrix) {
		string title = data.getParam<std::string>("title", "");
		if (title.empty()) {
			title = context_->getProcessingStepName();
		}
		context_->displayImage(title, *oMatrix, data.getParam<bool>("blocking",true),data.getParam<bool>("autoclose",false));
	} else {
		LOG_E("Failed to show image.");
	}
}

MetaData ShowImageModule::getMetaData() const
{
	map<string, DataDescription> input = {
		{"image", DataDescription(MATRIX, "the image to show.") }
	};
	map<string, ParamDescription> params = {
		{"title", ParamDescription("the title of the window, defaults to the current processing step name", true) },
		{"blocking", ParamDescription("'true' or 'false' determines if the processing chain has to wait for this window to close or not.",true) },
		{"autoclose", ParamDescription("'true' or 'false' determines if the window automatically closes after blocking has ended.",true) }
	};

	return MetaData(
		"Show an image by opening a window.",
		"I/O",
		input,
		map<string, DataDescription>(), // no outputs
		params
	);
}

