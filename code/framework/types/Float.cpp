#include "Float.hpp"

using namespace std;
using namespace uipf;

// gives the float
/*
*/
float Float::getContent(){
	return fl;
}

// sets the float
/*
f	float content
*/
void Float::setContent(float f){
	fl = f;
}

// returns the data type of this data object
Type Float::getType(){
	return FLOAT;
}
