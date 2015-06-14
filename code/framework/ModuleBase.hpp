#ifndef MODULEBASE_H
#define MODULEBASE_H

#include "Context.hpp"
#include "StdIncl.hpp"
#include "types/Data.hpp"
#include "Utils.hpp"
#include "Logger.hpp"
#include "InvalidConfigException.hpp"
#include "MetaData.hpp"
#include "ModuleInterface.hpp"
#include "DataManager.hpp"

namespace uipf{


// Methods and Data all Moduls share
class ModuleBase : public ModuleInterface
{

	public:
		ModuleBase();
		ModuleBase(const std::string& name):name_(name){};

		virtual ~ModuleBase();

		// context 	is a container providing access to the current environment, allowing to open windows, write to logger etc...
		virtual void setContext(Context*) Q_DECL_OVERRIDE;

		// the name of the module, which can be referenced in the yaml configuration
		virtual std::string name() const Q_DECL_OVERRIDE;

	public: //interface methods that still must be implemented by subclasses

		virtual void run( DataManager& data ) const = 0;

		// meta data that contains description of modules inputs, outputs and parameters
		virtual uipf::MetaData getMetaData() const = 0 ;

	protected:
		Context* context_;
		std::string name_;

};

} //namespace


#endif // MODULEBASE_H




