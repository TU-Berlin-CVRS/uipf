#ifndef Context_H
#define Context_H

namespace uipf{


// context 	is a container providing access to the current environment, allowing to open windows, write to logger etc...
class Context {
	public:
		// constructor (can't be virtual!)
		Context(void){};
		// destructor
		~Context(void){};
		
		// TODO finalize the environment
};

}

#endif

