#ifndef MODULEBASE_H
#define MODULEBASE_H

#include "Context.hpp"
#include "StdIncl.hpp"
#include "types/Data.hpp"
#include "Logger.hpp"

namespace uipf{


// Methods and Data all Moduls share
class ModuleBase {

	public:
		ModuleBase();
		virtual ~ModuleBase();

		void setContext(Context*);

	protected:
		Context* context_;	

		//returns a typesafe smartpointer to input/output data by name if it is available
		template <typename T>
		typename T::ptr getData(std::map<std::string, Data::ptr >& mData, const std::string& strName) const;
		
		//returns a typesafe parameter by name if it is available. otherwise a defaultValue is used
		template <typename T>
		T getParam(std::map<std::string, std::string >& mParams, const std::string& strName, T defaultValue) const;
		
		//print given parameters
		void listParams( std::map < std::string, std::string >& params) const;
};


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//templates need to be implemented in headers


template <typename T>
typename T::ptr ModuleBase::getData(std::map<std::string, Data::ptr >& mData, const std::string& strName) const
{
	if (mData.find(strName) != mData.end())
	{
		//do downcasting..
		T *tmp = dynamic_cast<T*>(mData[strName].get());
		std::unique_ptr<T> derivedPointer;
		if(tmp != nullptr)
		{
		    mData[strName].release();//2DO check sideeffects
		    derivedPointer.reset(tmp);
		}
		
		return derivedPointer;
	}
	else
		return nullptr; //2DO Exception
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
		return defaultValue; 

}



} //namespace 




#endif // MODULEBASE_H




