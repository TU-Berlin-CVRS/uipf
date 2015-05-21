#ifndef ELEM_H
#define ELEM_H

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
class Elem {
	public:
		// constructor (can't be virtual!)
		Elem(void){};
		// destructor
		virtual ~Elem(void){};
};


#endif




