#ifndef INVALIDCONFIGEXCEPTION_H
#define INVALIDCONFIGEXCEPTION_H

#include <string>
#include <map>
#include "types/Data.hpp"

namespace uipf {

class InvalidConfigException : public std::exception{

	public:
		// constructor
		InvalidConfigException(void) : message_("") {};
		InvalidConfigException(std::string message) : message_(message) {};

		virtual const char* what() const noexcept { return message_.c_str(); };

	private:
		std::string message_;

};

}; // namespace

#endif

