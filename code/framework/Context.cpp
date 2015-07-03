#include "Context.hpp"

using namespace uipf;

void Context::displayImage(const std::string strTitle, const Matrix& oMat, bool bBlocking) const
{
	//2DO:Use QT
	//2DO eliminate Warnings: "QApplication: Object event filter cannot be in a different thread."
	using namespace cv;
	namedWindow( strTitle.c_str(), WINDOW_AUTOSIZE );
	imshow( strTitle.c_str(), oMat.getContent() );

	if (bBlocking)
		waitKey(-1);
}

// TODO maybe implement logger here?
