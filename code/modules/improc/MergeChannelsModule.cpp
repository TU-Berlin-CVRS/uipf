#include "MergeChannelsModule.hpp"
#include "../../framework/types/List.hpp"

using namespace std;
using namespace uipf;


// runs the module
/*
data	DataManager handles the input and ouput of this module
*/
void MergeChannelsModule::run( DataManager& data) const
{
	using namespace cv;

	// get a pointers to the input data
	List<Matrix>::c_ptr pList = data.getInputData< List<Matrix> >("channels");
	std::list<Matrix::ptr> list = pList->getContent();

	vector<Mat> channels;
	for(auto it = list.begin(); it != list.end(); ++it) {
		channels.push_back((*it)->getContent());
	}

	// merge channels into image
	Mat image;
	merge(channels, image);

	// set the result (output) on the datamanager
	data.setOutputData("image", new Matrix(image));

}

// returns the meta data of this module
MetaData MergeChannelsModule::getMetaData() const
{
	map<string, DataDescription> input = {
		{"channels", DataDescription(MATRIX_LIST, "the image channel list.") }
	};
	map<string, DataDescription> output = {
		{"image", DataDescription(MATRIX, "the image as a result from the merge.") }
	};
	map<string, ParamDescription> params = {};

	return MetaData(
		"Merges a list of channel matrixes into an image.",
		"Image Processing",
		input,
		output,
		params
	);
}

