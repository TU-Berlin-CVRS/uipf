#ifndef STRING_H
#define STRING_H

#include <string>
#include "Data.hpp"

namespace uipf{

using namespace std;

// String which is a specification of Elem
class String : Data {
	public:
		// constructor (can't be virtual!)
		String(void){};
		// destructor
		~String(void){};
		
		// get content
		string getContent();
		
		// set content
		void setContent(string);
		
	private:
		// content
		string str;
};

}

#endif
