#include "GUIEventDispatcher.hpp"

//Register Types for interthreadcommunication. qRegisterMetaType() needs to be called too -> ctr()
Q_DECLARE_METATYPE(uipf::Logger::LogType)
Q_DECLARE_METATYPE(std::string)

namespace uipf
{

GUIEventDispatcher* GUIEventDispatcher::instance_ = nullptr;

GUIEventDispatcher* GUIEventDispatcher::instance()
{
  static GUIEventDispatcher::Guard w;
  if( instance_ == nullptr )
    instance_ = new GUIEventDispatcher();
  return instance_;
}

GUIEventDispatcher::GUIEventDispatcher()
{
	//All custom types (even that from std::), that should be used in signals and slots that are invoked from different threads
	//have to be registered for QT in their Typesystem
	//Q_DECLARE_METATYPE(...) has to be called in advance
	qRegisterMetaType<uipf::Logger::LogType>("Logger::LogType");
	qRegisterMetaType<std::string>("std::string");
}

GUIEventDispatcher::~GUIEventDispatcher() {}


void GUIEventDispatcher::triggerReportProgress(const float& value)
{
	//send signal to GUI
	emit reportProgressEvent(value);
}

void GUIEventDispatcher::triggerLogEvent(const Logger::LogType& eLogType, const std::string& strMessage)
{
	//send signal to GUI
	emit logEvent(eLogType,strMessage);
}

void GUIEventDispatcher::triggerSelectNodesInGraphView(const std::vector<std::string>& vcProcessingStepNames,uipf::gui::GraphViewSelectionType eType, bool bUnselectOthers /*=true*/)
{
	//send signal to GUI
	emit selectNodesInGraphView(vcProcessingStepNames,eType,bUnselectOthers);
}

void GUIEventDispatcher::triggerSelectSingleNodeInGraphView(const std::string& strProcessingStepName,uipf::gui::GraphViewSelectionType eType, bool bUnselectOthers /*=true*/)
{
	std::vector<std::string> tmp;
	tmp.push_back(strProcessingStepName);
	triggerSelectNodesInGraphView(tmp,eType,bUnselectOthers);
}

void GUIEventDispatcher::triggerClearSelectionInGraphView()
{
	//send signal to GUI
	emit clearSelectionInGraphView();
}

void GUIEventDispatcher::triggerCloseWindow(const std::string& strTitle)
{
	emit closeWindow(strTitle);
}

// create windows that show images without opencv imshow()
void GUIEventDispatcher::triggerCreateWindow(const std::string& strTitle, const cv::Mat& oMat)
{
	using namespace cv;

	// lock needed to ensure only one image is displayed at a time to avoid different threads to interfere
	mutex.lock();

	//this code is inspired by opencv source /modules/highgui/src/window_QT.cpp
	//we create a standardised tmp image to support different image types (rgb,grayscale)
	//other types need to be tested...

	// when changing this code make sure tmp is in scope until the mutex is unlocked.
	// This is because it holds the data of the image that is displayed in the GUI thread.
	// If tmp falls out of scope before the image is rendered in the GUI it will delete its
	// memory because of the OpenCV internal smartpointer implementation and the image display
	// will look corrupt.
	Mat tmp = Mat(oMat.rows, oMat.cols, CV_8UC3);

	if (oMat.channels() == 3) {
		// assume RGB image for 3 channels
		cvtColor(oMat, tmp, CV_BGR2RGB);
	} else if (oMat.channels() == 1) {
		// assume Grayscale image for 1 channel
		// convert gray to RGB since gray image display in QImage is only supported since Qt 5.4
		cvtColor(oMat, tmp, CV_GRAY2RGB);
	} else {
		LOG_E("Unsupported number of channels for displaying image.");
		mutex.unlock();
		return;
	}

	image_ = QImage(tmp.data, tmp.cols, tmp.rows, tmp.step, QImage::Format_RGB888);

	// send signal to GUI
	emit createWindow(strTitle);

	// wait for the GUI to render the image so that resources of this thread are not freed before GUI can use them
	imageRendered.wait(&mutex);

	// unlock the mutex created above
	mutex.unlock();

}

} //namespace
