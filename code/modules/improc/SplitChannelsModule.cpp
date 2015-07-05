#include "SplitChannelsModule.hpp"
#include "../../framework/types/List.hpp"

using namespace std;
using namespace uipf;


// runs the module
/*
data	DataManager handles the input and ouput of this module
*/
void SplitChannelsModule::run( DataManager& data) const
{
	using namespace cv;

	// get a pointers to the input data
	Matrix::c_ptr pImage = data.getInputData<Matrix>("image");
	// get the actual opencv matrix of the input data
	Mat image = pImage->getContent();

	// split image into channels
	vector<Mat> channels;
	split(image, channels);

	// convert channels into abstract type
	std::list<Matrix::ptr> list;
	for(auto it = channels.begin(); it != channels.end(); ++it) {
		list.push_back(Matrix::ptr(new Matrix(*it)));
	}

	// set the result (output) on the datamanager
	data.setOutputData("channels", new List<Matrix>(list));

}

// returns the meta data of this module
MetaData SplitChannelsModule::getMetaData() const
{
	map<string, DataDescription> input = {
		{"image", DataDescription(MATRIX, "the image to split.") }
	};
	map<string, DataDescription> output = {
		{"channels", DataDescription(MATRIX_LIST, "the image channel list.") }
	};
	map<string, ParamDescription> params = {};

	return MetaData(
		"Splits an image into the different channels.",
		"Image Processing",
		input,
		output,
		params
	);
}

