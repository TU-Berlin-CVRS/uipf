#include "RunWorkerThread.h"

using namespace uipf;

RunWorkerThread::RunWorkerThread(ModuleManager& mm, Configuration& config)
 : mm_(mm),config_(config)
{

}

RunWorkerThread::~RunWorkerThread() {
}

void RunWorkerThread::stop()
{
	mm_.requestStop();
}

void RunWorkerThread::run()
{
	mm_.run(config_);
}
