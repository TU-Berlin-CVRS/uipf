#ifndef ERROREXCEPTION_H
#define ERROREXCEPTION_H

#include <string>
#include <map>
#include "types/Data.hpp"

namespace uipf {

class ErrorException : public std::exception{

	public:
		// constructor
		ErrorException(void) : message_("") {};
		ErrorException(std::string message) : message_(message) {};

		virtual const char* what() const noexcept { return message_.c_str(); };

	private:
		std::string message_;

};

}; // namespace

#endif

