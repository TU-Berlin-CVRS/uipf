#ifndef MYSTRING_H
#define MYSTRING_H

#include <string>
#include "Elem.hpp"

using namespace std;

// String which is a specification of Elem
class MyString : Elem {
	public:
		// constructor (can't be virtual!)
		MyString(void){};
		// destructor
		~MyString(void){};
		
		// get content
		string getContent();
		
		// set content
		void setContent(string);
		
	private:
		// content
		string str;
};


#endif
