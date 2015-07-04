#include "Matrix.hpp"

using namespace cv;
using namespace uipf;

// constructor
/*
oMat	the content of the matrix	(matrix_ = oMat)
*/
Matrix::Matrix(Mat& oMat) :matrix_(oMat){
}


// get content (returns a cloned version of Mat by default)
// this is due to prevent overwriting accidentally
Mat Matrix::getContent(bool bAutoClone /*= true*/) const{
	if (bAutoClone)
		return matrix_.clone();
	else
		return matrix_;
}

// sets the content of the matrix
/*
m	new matrix content
*/
void Matrix::setContent(Mat& m){
	matrix_ = m;
}

// returns the data type of this data object: in this case: MATRIX
Type Matrix::getType() {
	return MATRIX;
}
