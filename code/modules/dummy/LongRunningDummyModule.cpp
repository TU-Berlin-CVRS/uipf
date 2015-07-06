#include "LongRunningDummyModule.hpp"
#include <unistd.h>

using namespace std;
using namespace uipf;


/*

*/
void LongRunningDummyModule::run( DataManager& data) const
{
	unsigned int uiCounter = 30;
	while(uiCounter-->0)
	{
		LOG_I("I am soo busy!");

		if (context_->isStopRequested())
		{
			LOG_I("Hm, somebody wants me to stop doing my hard, hard work!");
			LOG_I("giving up...");
			break;
		}
		usleep(1000000);

	}
	if (data.hasInputData("image")) {
		Matrix::c_ptr oMatrix = data.getInputData<Matrix>("image");
		cv::Mat m = oMatrix->getContent();
		data.setOutputData<Matrix>("image", new Matrix(m));
	}
}

MetaData LongRunningDummyModule::getMetaData() const
{
	map<string, DataDescription> input = {
		{"image", DataDescription(MATRIX, "the image.") }
	};
	map<string, DataDescription> output = {
		{"image", DataDescription(MATRIX, "the same image again :)") }
	};

	return MetaData(
		"A dummy module for testing long running tasks",
		"Dummy",
		input,
		output,
		map<string, ParamDescription>()
	);
}

