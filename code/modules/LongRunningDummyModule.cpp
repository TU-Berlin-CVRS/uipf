#include "LongRunningDummyModule.hpp"
#include <unistd.h>

using namespace std;
using namespace uipf;


/*

*/
void LongRunningDummyModule::run( DataManager& data) const
{
	unsigned int uiCounter = 100;
	while(uiCounter-->0)
	{
		LOG_I("I am soo busy!");

		if (context_->bStopRequested_)
		{
			LOG_I("Hm, somebody wants me to stop doing my hard, hard work!");
			LOG_I("giving up...");
			break;
		}
		usleep(1000000);

	}
}

MetaData LongRunningDummyModule::getMetaData() const
{
	return MetaData(
		"A dummy module for testing long running tasks",
		"Dummy",
		map<string, DataDescription>(),
		map<string, DataDescription>(),
		map<string, ParamDescription>()
	);
}

