#ifndef DATA_H
#define DATA_H

#include "../StdIncl.hpp"

namespace uipf{

/**
The elements of Type declare all possible Type-sorts the data objects can be.
For creating a new Type-sort:
- add an enum in Type
- case: standard class:		create a new .hpp and .cpp classes, which derive from the class Data.hpp
- case: template class:		create a new .hpp template class, which derive from the class Data.hpp
*/
enum Type {

	STRING,
	INTEGER,
	FLOAT,
	BOOL,
	MATRIX,

	STRING_LIST,
	INTEGER_LIST,
	FLOAT_LIST,
	BOOL_LIST,
	MATRIX_LIST,

};

// returns the string representation of a type
std::string type2string(Type t);

// Data which represents an arbitrary element
// It is a virtual class and cant be instantiated
class Data {
	public:
		typedef SMARTPOINTER<Data> ptr;
		typedef const SMARTPOINTER<Data> c_ptr;

	public:
		// constructor (can't be virtual!)
		Data(void){};
		// destructor
		virtual ~Data(void){};

		// returns the data type of this data object
		// this is a virtual method, which has to be overwritten in the class, which derives of Data
		virtual Type getType() = 0;

};

} // namespace

#endif
