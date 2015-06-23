#ifndef GUIEVENTDISPATCHER_H_
#define GUIEVENTDISPATCHER_H_

#include <QObject>

//A class that handles communication between QTGUI components and modelclasses
//by exposing signals.
//so model classes can trigger the GUI without the need of implementing QOBject
class GUIEventDispatcher : public QObject
{
Q_OBJECT

public:
	GUIEventDispatcher();
	virtual ~GUIEventDispatcher();

signals: //for QT to connect
	void reportProgressEvent(const float& value);

public: //methods for model to call and trigger GUI
		void triggerReportProgress(const float& value);
};

#endif /* GUIEVENTDISPATCHER_H_ */
