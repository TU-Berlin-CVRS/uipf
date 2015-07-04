#ifndef GUIEVENTDISPATCHER_H_
#define GUIEVENTDISPATCHER_H_

#include "StdIncl.hpp"
#include <QObject>
#include "Logger.hpp"

namespace uipf
{

//A class that handles communication between QTGUI components and module-classes
//by exposing signals.
//so module classes can trigger the GUI without the need of implementing QObject
class GUIEventDispatcher : public QObject
{
Q_OBJECT

public:
	static GUIEventDispatcher* instance();

private:
	// holds the singleton instance of the logger
	static GUIEventDispatcher *instance_;
	GUIEventDispatcher();
	GUIEventDispatcher( const GUIEventDispatcher& );

	~GUIEventDispatcher();

	// helper class to ensure logger gets deleted when the context is gone
	class Guard {
		public: ~Guard() {
			if( GUIEventDispatcher::instance_ != 0 ) {
				delete GUIEventDispatcher::instance_;
			}
		}
	};
	friend class Guard;

signals: //for QT to connect
	void reportProgressEvent(const float& val);
	void logEvent(const Logger::LogType& eType, const std::string& strMessage);
	int  createWindow(const std::string strTitle, const cv::Mat& oMat);

public: //methods for model to call and trigger GUI
	void triggerReportProgress(const float& );
	void triggerLogEvent(const Logger::LogType& eType, const std::string& strMessage);
	void triggerCreateWindow(const std::string strTitle, const cv::Mat& oMat);
};

} //namespace

#endif /* GUIEVENTDISPATCHER_H_ */
