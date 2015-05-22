#ifndef MATRIX_H
#define MATRIX_H


#include "Matrix.hpp"
#include "Data.hpp"
#include <opencv2/opencv.hpp>

namespace uipf{

using namespace cv;

// Matrix which is a specification of Elem
class Matrix : Data {
	public:
		// constructor (can't be virtual!)
		Matrix(void){};
		// destructor
		~Matrix(void){};

		// get content
		Mat getContent();

		// set content
		void setContent(Mat&);

	private:
		// content
		Mat matrix;
};

}

#endif
