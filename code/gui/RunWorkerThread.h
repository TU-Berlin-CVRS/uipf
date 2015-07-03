#ifndef RUNWORKERTHREAD_H_
#define RUNWORKERTHREAD_H_
#include <QThread>
#include "../framework/Configuration.hpp"
#include "../framework/ModuleManager.hpp"

using namespace uipf;

//A class that does the background work in a thread.
//By deriving QThread QT handles signals/slot thread marshaling automatically.
//for this all Types used in signals have to be registered by qRegisterMetaType() -> see GUIEventDispatcher.cpp
class RunWorkerThread: public QThread {

	Q_OBJECT

public:
	RunWorkerThread(ModuleManager& mm, Configuration& config);
	virtual ~RunWorkerThread();

	//run module chain in a separate thread
	void run() Q_DECL_OVERRIDE;
	//tell modules to stop work now
	void stop();

	// Define signal:
 signals:
 	 //processing has finished -> tell GUI
	void finished();

private:

	uipf::ModuleManager& mm_;
	uipf::Configuration& config_;
};

#endif /* RUNWORKERTHREAD_H_ */
