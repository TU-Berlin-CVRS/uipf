#ifndef MODULEINTERFACE_H
#define MODULEINTERFACE_H

#include <QObject>
#include <QtPlugin>
#include "Context.hpp"
#include "MetaData.hpp"
#include "InvalidConfigException.hpp"
#include "ErrorException.hpp"
#include "DataManager.hpp"

// data tyes
#include "types/Data.hpp"
#include "types/String.hpp"
#include "types/Integer.hpp"
#include "types/Float.hpp"
#include "types/Bool.hpp"
#include "types/Matrix.hpp"
#include "types/List.hpp"

namespace uipf{


// required interface for a module
class ModuleInterface {
	public:
		// constructor (can't be virtual!)
		ModuleInterface(void){};
		// destructor
		virtual ~ModuleInterface(void){};

		// method has to be implemented in the Module
		// DataManager provides access to all resources described in the module meta description (params,input,output)
		virtual void run( DataManager& data ) const = 0;

		// the name of the module, which can be referenced in the yaml configuration
		virtual std::string name() const = 0;

		// meta data that contains description of modules inputs, outputs and parameters
		virtual MetaData getMetaData() const = 0;

		// context 	is a container providing access to the current environment, allowing to open windows, write to logger etc...
		virtual void setContext(Context*) = 0;
};

} //namespace

// this is used for dynamic loading of Qt Plugins
// it allows modules to be loaded dynamically from shared library files
QT_BEGIN_NAMESPACE
#define ModuleInterface_iid "org.tu-berlin.uipf.ModuleInterface"
Q_DECLARE_INTERFACE(uipf::ModuleInterface,ModuleInterface_iid)
QT_END_NAMESPACE

#endif // MODULEINTERFACE_H
