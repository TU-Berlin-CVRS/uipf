#ifndef MODULEINTERFACE_H
#define MODULEINTERFACE_H

#include <QtPlugin>
#include <string>
#include <map>
#include "types/Data.hpp"
#include "Context.hpp"
#include "MetaData.hpp"
#include "InvalidConfigException.hpp"
#include "ErrorException.hpp"

namespace uipf{


// describes the m
class ModuleInterface {
	public:
		// constructor (can't be virtual!)
		ModuleInterface(void){};
		// destructor
		virtual ~ModuleInterface(void){};

		// method has to be implemented in the Module
		// input 	is a std::map of input resources, the names are described in the module meta description
		// params 	is a std::map of input parameters, the names are described in the module meta description
		// ouput 	is a std::map of output resources, the names are described in the module meta description
		virtual void run( std::map<std::string, uipf::Data::ptr& >& input,
				  std::map<std::string, std::string >& params,
		   		  std::map<std::string, uipf::Data::ptr >& output ) const = 0;

		// TODO make context part of the interface
		// context 	is a container providing access to the current environment, allowing to open windows, write to logger etc...

		// the name of the module, which can be referenced in the yaml configuration
		virtual std::string name() const = 0;

		// meta data that contains description of modules inputs, outputs and parameters
		virtual uipf::MetaData getMetaData() const = 0;

};

} //namespace

QT_BEGIN_NAMESPACE
#define ModuleInterface_iid "org.tu-berlin.uipf.ModuleInterface"
Q_DECLARE_INTERFACE(uipf::ModuleInterface,ModuleInterface_iid)
QT_END_NAMESPACE


#endif // MODULEINTERFACE_H

