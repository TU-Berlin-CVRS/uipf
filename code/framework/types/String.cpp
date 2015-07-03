#include "String.hpp"

using namespace std;
using namespace uipf;

// returns the value of the string
string String::getContent(){
	return str_;
}

// sets the value of the string
/*
str		new string value
*/
void String::setContent(string str){
	str_ = str;
}

// returns the data type of this data object: in this case: STRING
Type String::getType(){
	return STRING;
}
