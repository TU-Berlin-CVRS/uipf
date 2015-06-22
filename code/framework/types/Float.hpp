#ifndef FLOAT_H
#define FLOAT_H


#include "Data.hpp"

namespace uipf{


// Float which is a specification of Elem
class Float : public Data {
	public:
			typedef SMARTPOINTER<Float> ptr;
			typedef const SMARTPOINTER<Float> c_ptr;
	public:
		// constructor (can't be virtual!)
		Float(void){};
		// destructor
		~Float(void){};

		// get content
		float getContent();

		// set content
		void setContent(float);

		// returns the data type of this data object
		Type getType() override;

	private:
		// content
		float fl;
};

}

#endif
