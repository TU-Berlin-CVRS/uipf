#ifndef DATA_H
#define DATA_H

#include "../StdIncl.hpp"

namespace uipf{

enum Type {

	STRING,
	INTEGER,
	FLOAT,
	BOOL,
	LIST,
	MAP,
	MATRIX,
};

// Elem which represents an arbitrary element
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
		virtual Type getType() = 0;

};

}
#endif




