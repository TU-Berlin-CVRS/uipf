#ifndef STRING_H
#define STRING_H

#include <string>
#include "Data.hpp"

namespace uipf{


// String which is a specification of Elem
class String : public Data {
	public:
		typedef SMARTPOINTER<String> ptr;
		typedef const SMARTPOINTER<String> c_ptr;

	public:
		// constructor (can't be virtual!)
		String(void){};
		// destructor
		~String(void){};

		// get content
		std::string getContent();

		// set content
		void setContent(std::string);

		// returns the data type of this data object
		Type getType() override;

	private:
		// content
		std::string str;
};

}

#endif
