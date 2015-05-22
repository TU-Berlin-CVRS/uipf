#ifndef DATA_H
#define DATA_H

	// TODO make this an interface for all possible types:
	// string
	// integer
	// char
	// uchar
	// Mat with arbitrary number of channels
	// vec<Elem>
	// float
	// map

// Elem which represents an arbitrary element
class Data {
	public:
		// constructor (can't be virtual!)
		Data(void){};
		// destructor
		virtual ~Data(void){};
};


#endif




