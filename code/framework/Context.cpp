#include "Context.hpp"
#include "GUIEventDispatcher.hpp"

using namespace uipf;

void Context::displayImage(const std::string strTitle, const Matrix& oMat, bool bBlocking) const
{
	if (bHaveGUI_)
	{
		GUIEventDispatcher::instance()->triggerCreateWindow(strTitle,oMat.getContent());
		return;
	}
	else
	{
		using namespace cv;
		namedWindow( strTitle.c_str(), WINDOW_AUTOSIZE );
		imshow( strTitle.c_str(), oMat.getContent() );

		if (bBlocking)
			waitKey(-1);
	}
}

// TODO maybe implement logger here?
