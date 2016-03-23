#include "Float.hpp"

using namespace uipf;

// returns the value of the float
float Float::getContent() const {
	return f_;
}

// sets the value of the float
/*
f	new float value
*/
void Float::setContent(float f) {
	f_ = f;
}

// returns the data type of this data object: in this case: FLOAT
Type Float::getType() const {
	return FLOAT;
}
