#ifndef STRING_H
#define STRING_H

#include <string>
#include "Data.hpp"

namespace uipf{


// String which is a specification of Data
class String : public Data {
	public:
		typedef SMARTPOINTER<String> ptr;
		typedef const SMARTPOINTER<String> c_ptr;

	public:
		// constructor
		String(std::string s) : str_(s) {};
		// destructor
		~String(void){};

		// returns the value of the string
		std::string getContent();

		// sets the value of the string
		void setContent(std::string);

		// returns the data type of this data object: in this case: STRING
		Type getType() override;

	private:
		// value of the string
		std::string str_;
};

} // namespace

#endif
