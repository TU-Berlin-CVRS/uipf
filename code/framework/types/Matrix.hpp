#ifndef MATRIX_H
#define MATRIX_H


#include "Matrix.hpp"
#include "Data.hpp"
#include <opencv2/opencv.hpp>

namespace uipf{

// Matrix which is a specification of Elem
class Matrix : Data {
	public:
		// constructor (can't be virtual!)
		Matrix(void){};
		// destructor
		~Matrix(void){};

		// get content
		cv::Mat getContent();

		// set content
		void setContent(cv::Mat&);

	private:
		// content
		cv::Mat matrix;
};

}

#endif
