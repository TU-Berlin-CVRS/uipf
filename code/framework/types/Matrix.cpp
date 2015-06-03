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
Mat& Matrix::getContent(){
	return matrix_;
}

// sets the matrix
/*
m	matrix content
*/
void Matrix::setContent(Mat& m){
	matrix_ = m;
}
