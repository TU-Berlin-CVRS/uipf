#ifndef FLOAT_H
#define FLOAT_H

#include "Data.hpp"

namespace uipf{


// Float which is a specification of Data
class Float : public Data {
	public:
			typedef SMARTPOINTER<Float> ptr;
			typedef const SMARTPOINTER<Float> c_ptr;
	public:
		// constructor
		Float(float f) : f_(f) {};
		// destructor
		~Float(void){};

		// returns the value of the float
		float getContent();

		// sets the value of the float
		void setContent(float);

		// returns the data type of this data object: in this case: FLOAT
		Type getType() override;

	private:
		// value of the float
		float f_;
};

} // namespace

#endif
