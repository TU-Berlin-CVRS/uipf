#ifndef INTEGER_H
#define INTEGER_H

#include "Data.hpp"

namespace uipf{


// Integer which is a specification of Data
class Integer : public Data {
	public:
		typedef SMARTPOINTER<Integer> ptr;
		typedef const SMARTPOINTER<Integer> c_ptr;

	public:
		// default constructor
		Integer() : i_(0) {};
		// constructor
		Integer(int i) : i_(i) {};
		// copy constructor
		Integer(const Integer& i) : i_(i.i_) {};
		// destructor
		~Integer(void){};

		// returns the value of the integer
		int getContent();

		// sets the value of the integer
		void setContent(int);

		// returns the data type of this data object: in this case: INTEGER
		Type getType() override;

	private:
		// value of the integer
		int i_;
};

} // namespace

#endif
