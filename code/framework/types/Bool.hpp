#ifndef BOOL_H
#define BOOL_H

#include "Data.hpp"

namespace uipf{


// Bool which is a specification of Data
class Bool : public Data {
	public:
		typedef SMARTPOINTER<Bool> ptr;
		typedef const SMARTPOINTER<Bool> c_ptr;

	public:
		// constructor
		Bool(bool b) : b_(b) {};
		// destructor
		~Bool(void){};

		// returns the value of the boolean
		bool getContent();

		// sets the value of the boolean
		void setContent(bool);

		// returns the data type of this data object: in this case: BOOL
		Type getType() override;

	private:
		// value of the boolean
		bool b_;
};

} // namespace

#endif
