#ifndef Context_H
#define Context_H

#include "StdIncl.hpp"
#include "types/Matrix.hpp"

namespace uipf{


// context 	is a container providing access to the current environment, allowing to open windows, write to logger etc...
class Context {
	public:
		// constructor (can't be virtual!)
		Context(void){};
		// destructor
		~Context(void){};
		
		
		void displayImage(const std::string strTitle, const Matrix& oMat) const;

};

}

#endif

