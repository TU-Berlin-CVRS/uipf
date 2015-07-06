#include <iostream>

#include "ImageWindow.hpp"

using namespace uipf;
using namespace std;

void ImageWindow::closeEvent(QCloseEvent *event) {

	// resume a paused chain when an image window gets closed
	mm_.resumeChain();
}

void ImageWindow::keyReleaseEvent(QKeyEvent *)
{
	// resume a paused chain on key press
	mm_.resumeChain();
}
