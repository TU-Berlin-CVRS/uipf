#include <chrono>
#include <thread>

#include "Context.hpp"
#include "GUIEventDispatcher.hpp"

using namespace uipf;

void Context::displayImage(const std::string& strTitle, const Matrix& oMat, bool bBlocking, bool bAutoClose)
{
	// if we are running in the gui, open the window using Qt in the Qt MainWindow thread
	if (bHasGUI_) {

		GUIEventDispatcher::instance()->triggerCreateWindow(strTitle, oMat.getContent(false));

		// block if blocking is true
		if (bBlocking) {
			waitKey();
			if (bAutoClose)
				GUIEventDispatcher::instance()->triggerCloseWindow(strTitle);
		}


	} else {
		// otherwise use OpenCV imshow function
		cv::namedWindow( strTitle.c_str(), cv::WINDOW_AUTOSIZE );
		cv::imshow( strTitle.c_str(), oMat.getContent() );

		// block if blocking is true
		if (bBlocking) {
			waitKey();
			if (bAutoClose)
				cv::destroyWindow(strTitle.c_str());
		} else {
			// calls waitKey to actually show the window
			cv::waitKey(1);
		}


	}

}

void Context::waitKey(std::string message)
{
	if (message.empty()) {
		message = "Press any key to continue...";
	}

	if (bHasGUI_) {
		LOG_I(message);
		// when we run in the GUI we have to communicate with the GUI
		bPaused_ = true;
		// wait until pause is set to false
		while(bPaused_ && !bStopRequested_) {
			std::this_thread::sleep_for( std::chrono::milliseconds (250) );
		}
		LOG_D("Key has been pressed, not waiting anymore.");
	} else {
		// on console use opencv waitKey()
		std::cout << message << std::endl;
		cv::waitKey(0);
	}
}

// TODO maybe implement logger here?
