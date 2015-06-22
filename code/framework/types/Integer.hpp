#ifndef INTEGER_H
#define INTEGER_H


#include "Data.hpp"

namespace uipf{


// Integer which is a specification of Elem
class Integer : public Data {
	public:
		typedef SMARTPOINTER<Integer> ptr;
		typedef const SMARTPOINTER<Integer> c_ptr;

	public:
		// constructor (can't be virtual!)
		Integer(void){};
		// destructor
		~Integer(void){};

		// get content
		int getContent();

		// set content
		void setContent(int);

		// returns the data type of this data object
		Type getType() override;

	private:
		// content
		int in;
};

}

#endif
