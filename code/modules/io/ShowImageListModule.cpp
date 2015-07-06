#include "ShowImageListModule.hpp"
#include "../../framework/types/List.hpp"

using namespace std;
using namespace uipf;

/*

*/
void ShowImageModule::run( DataManager& data) const
{
	using namespace cv;

	string title = data.getParam<std::string>("title", "");
	if (title.empty()) {
		title = context_->getProcessingStepName();
	}

	List<Matrix>::c_ptr pList = data.getInputData< List<Matrix> >("imageList");
	list<Matrix::ptr> list = pList->getContent();

	int i = 1;
	for(auto it = list.cbegin(); it != list.end(); ++it) {
		context_->displayImage(title + string(" ") + to_string(i), **it, false);
		i++;
	}

	if (data.getParam<bool>("blocking", true)) {
		context_->waitKey();
	}
}

MetaData ShowImageModule::getMetaData() const
{
	map<string, DataDescription> input = {
		{"imageList", DataDescription(MATRIX_LIST, "the list of images to show.") },

	};
	map<string, ParamDescription> params = {
		{"title", ParamDescription("the title of the window, defaults to the current processing step name", true) },
		{"blocking", ParamDescription("'true' or 'false' determines if the processing chain has to wait for this window to close or not.",true) }
	};

	return MetaData(
		"Shows images in a list of images by opening a window.",
		"I/O",
		input,
		map<string, DataDescription>(), // no outputs
		params
	);
}

