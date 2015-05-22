#include "MyMatrix.hpp"

using namespace cv;

// gives the matrix 
/*
*/
Mat MyMatrix::getContent(){
	return matrix;
}

// sets the string
/*
s	string content
*/
void MyMatrix::setContent(Mat& m){
	matrix = m;
}
