#include "Context.hpp"

using namespace uipf;

// TODO implement logger
// TODO implement window opener
// ...

void Context::displayImage(const std::string strTitle, Matrix& oMat) const
{
	//2DO:Use QT
	using namespace cv;
	namedWindow( strTitle.c_str(), WINDOW_AUTOSIZE );
	imshow( strTitle.c_str(), oMat.getContent() );   
	waitKey(-1);
}
