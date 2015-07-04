#ifndef PROCESSINGSTEP_H
#define PROCESSINGSTEP_H

#include <string>
#include <map>

namespace uipf{

struct ProcessingStep{

		// name of this processing step in this configuration e.g. output
		std::string name;

		// name of the module that is used in this step e.g. storeImage
		std::string module;

		// a map of parameters configured for this module e.g. filename => image.png
		std::map<std::string, std::string> params;

		// inputs from other modules <inputName> => <nameOfOtherModule, outputOfOtherModule>
		std::map<std::string, std::pair<std::string, std::string> > inputs;

};

} // namespace

#endif
