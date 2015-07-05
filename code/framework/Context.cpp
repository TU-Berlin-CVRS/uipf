#include "Context.hpp"
#include "GUIEventDispatcher.hpp"

using namespace uipf;

void Context::displayImage(const std::string strTitle, const Matrix& oMat, bool bBlocking) const
{
	using namespace cv;

	// if we are running in the gui, open the window using Qt in the Qt MainWindow thread
	if (bHaveGUI_)
	{
		GUIEventDispatcher::instance()->triggerCreateWindow(strTitle, oMat.getContent().clone(), bBlocking);
	}
	else
	{
		// otherwise use OpenCV imshow function
		namedWindow( strTitle.c_str(), WINDOW_AUTOSIZE );
		imshow( strTitle.c_str(), oMat.getContent() );

		// block if blocking is true
		if (bBlocking) {
			waitKey(-1);
		} else {
			waitKey(1);
		}
	}

}

// TODO maybe implement logger here?
