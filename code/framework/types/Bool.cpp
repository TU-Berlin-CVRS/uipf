#include "Bool.hpp"

using namespace uipf;

// returns the value of the boolean
bool Bool::getContent(){
	return b_;
}

// sets the value of the boolean
/*
b	new boolean value
*/
void Bool::setContent(bool b){
	b_ = b;
}

// returns the data type of this data object: in this case: BOOL
Type Bool::getType(){
	return BOOL;
}
