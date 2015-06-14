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

		//returns a typesafe smartpointer to input/output data by name if it is available
		template <typename T>
		T* getData(std::map<std::string, Data::ptr& >& mData, const std::string& strName) const;

		//returns a typesafe parameter by name if it is available. otherwise a defaultValue is used
		template <typename T>
		T getParam(std::map<std::string, std::string >& mParams, const std::string& strName, T defaultValue) const;

		//print given parameters
		void listParams( std::map < std::string, std::string >& params) const;
};


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//templates need to be implemented in headers


template <typename T>
T* ModuleBase::getData(std::map<std::string, Data::ptr& >& mData, const std::string& strName) const
{
	if (mData.find(strName) != mData.end())
	{
		//do downcasting..
		Data::ptr& ptr = mData.find(strName)->second;
		return dynamic_cast<T*>(ptr.get());
	}
	else
	{
		throw InvalidConfigException(std::string("input data '") + strName + std::string("' not found!"));
	}
}

//2DO other type specialisations e.g. float with atof() etc.
template <typename T>
T ModuleBase::getParam(std::map<std::string, std::string >& mParams, const std::string& strName, T defaultValue) const
{
	if (mParams.find(strName) != mParams.end())
	{
		return static_cast<T>(mParams[strName]);
	}
	else
	{
		return defaultValue;
	}
}

template <>
inline int ModuleBase::getParam(std::map<std::string, std::string >& mParams, const std::string& strName, int defaultValue) const
{
	if (mParams.find(strName) != mParams.end())
	{
		return atoi(mParams[strName].c_str());
	}
	else
	{
		return defaultValue;
	}
}

template <>
inline float ModuleBase::getParam(std::map<std::string, std::string >& mParams, const std::string& strName, float defaultValue) const
{
	if (mParams.find(strName) != mParams.end())
	{
		return atof(mParams[strName].c_str());
	}
	else
	{
		return defaultValue;
	}
}

template <>
inline double ModuleBase::getParam(std::map<std::string, std::string >& mParams, const std::string& strName, double defaultValue) const
{
	if (mParams.find(strName) != mParams.end())
	{
		return atof(mParams[strName].c_str());
	}
	else
	{
		return defaultValue;
	}
}


} //namespace


#endif // MODULEBASE_H




