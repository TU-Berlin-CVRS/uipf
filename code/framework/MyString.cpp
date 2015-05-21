#include <string.h>
#include "MyString.hpp"

using namespace std;

// gives the string 
/*
*/
string MyString::getContent(){
	return str;
}

// sets the string
/*
s	string content
*/
void MyString::setContent(string s){
	str = s;
}
