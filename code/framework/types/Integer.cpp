#include "Integer.hpp"

using namespace uipf;

// returns the value of the integer
int Integer::getContent(){
	return i_;
}

// sets the value of the integer
/*
i	new integer value
*/
void Integer::setContent(int i){
	i_ = i;
}

// returns the data type of this data object: in this case: INTEGER
Type Integer::getType(){
	return INTEGER;
}
