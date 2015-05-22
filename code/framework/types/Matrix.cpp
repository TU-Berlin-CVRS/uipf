#include "Matrix.hpp"

using namespace cv;
using namespace uipf;

// gives the matrix 
/*
*/
Mat Matrix::getContent(){
	return matrix;
}

// sets the matrix
/*
m	matrix content
*/
void Matrix::setContent(Mat& m){
	matrix = m;
}
