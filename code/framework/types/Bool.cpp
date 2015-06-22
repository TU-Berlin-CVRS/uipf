#include "Bool.hpp"

using namespace std;
using namespace uipf;

// gives the float
/*
*/
bool Bool::getContent(){
	return b;
}

// sets the float
/*
f	float content
*/
void Bool::setContent(bool b){
	b_ = b;
}


// returns the data type of this data object
Type String::getType(){
	return BOOL;
}
