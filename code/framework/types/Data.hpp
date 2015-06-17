#ifndef DATA_H
#define DATA_H

#include "../StdIncl.hpp"

namespace uipf{
	
	// TODO make this an interface for all possible types:
	// string
	// integer
	// char
	// uchar
	// Mat with arbitrary number of channels
	// vec<Elem>
	// float
	// map

enum Type
{
	STRING,
	INTEGER,
	FLOAT,
	BOOL,
//	LIST, // TODO
//	MAP, // TODO
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




