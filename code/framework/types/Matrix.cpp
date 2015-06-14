#include "Matrix.hpp"

using namespace cv;
using namespace uipf;

Matrix::Matrix(cv::Mat& oMat)
:matrix_(oMat)
{

}

Type Matrix::getType()
{
return MATRIX;
}

// gives the matrix 
/*
*/
Mat Matrix::getContent(bool bAutoClone /*= true*/) const{
	if (bAutoClone)
		return matrix_.clone();
	else
		return matrix_;
}

// sets the matrix
/*
m	matrix content
*/
void Matrix::setContent(Mat& m){
	matrix_ = m;
}
