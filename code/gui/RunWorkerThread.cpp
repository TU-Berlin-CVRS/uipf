/*
 * RunWorkerThread.cpp
 *
 *  Created on: 30 Jun 2015
 *      Author: uipf
 */

#include "RunWorkerThread.h"
#include "../framework/GUIEventDispatcher.h"
#include "../framework/Logger.hpp"

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

