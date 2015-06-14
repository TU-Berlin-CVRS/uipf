#ifndef DATAMANAGER_H
#define DATAMANAGER_H


#include "StdIncl.hpp"
#include "types/Data.hpp"
#include "InvalidConfigException.hpp"
#include "Utils.hpp"

namespace uipf{
using namespace std;

// Provides and controls access to Parameters, Input and Outputdata
class DataManager
{

	public:
		DataManager(std::map < std::string, Data::ptr& >& input, std::map < std::string,std::string >& params, std::map < std::string, Data::ptr >& output)
	:input_(input),params_(params),output_(output)
	{}

		~DataManager(){}

		//returns a typesafe readonly smartpointer to input/output data by name if it is available
		template <typename T>
		const typename T::ptr getInputData( const std::string& strName) const;

		//returns a typesafe smartpointer to input/output data by name if it is available
		template <typename T>
		typename T::ptr getOutputData( const std::string& strName) const;

		//returns a typesafe smartpointer to input/output data by name if it is available
		template <typename T>
		void setOutputData( const std::string& strName, T*) ;

		//returns a typesafe parameter by name if it is available. otherwise a defaultValue is used
		template <typename T>
		T getParam(const std::string& strName, T defaultValue) const;

		//print given parameters
		void listParams( ) const;

	private:
		std::map < std::string, Data::ptr& >& input_;
		std::map < std::string, std::string >& params_;
		std::map < std::string, Data::ptr >& output_;


};


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//templates need to be implemented in headers


template <typename T>
const typename T::ptr DataManager::getInputData( const std::string& strName) const
{
	if (input_.find(strName) != input_.end())
	{
		//do downcasting..
		Data::ptr& ptr = input_.find(strName)->second;
		return std::dynamic_pointer_cast<T>(ptr);
	}
	else
	{
		throw InvalidConfigException(std::string("input data '") + strName + std::string("' not found!"));
	}
}


template <typename T>
typename T::ptr DataManager::getOutputData( const std::string& strName) const
{
	if (output_.find(strName) != output_.end())
	{
		//do downcasting..
		Data::ptr& ptr = input_.find(strName)->second;
		return std::dynamic_pointer_cast<T>(ptr);
	}
	else
	{
		throw InvalidConfigException(std::string("input data '") + strName + std::string("' not found!"));
	}
}

template <typename T>
void DataManager::setOutputData( const std::string& strName, T* outputData)
{
	output_.insert (pair < string, Data::ptr >(strName, typename T::ptr(outputData)));
}

//2DO other type specialisations e.g. float with atof() etc.
template <typename T>
T DataManager::getParam( const std::string& strName, T defaultValue) const
{
	if (params_.find(strName) != params_.end())
	{
		return static_cast<T>(params_[strName]);
	}
	else
	{
		return defaultValue;
	}
}

template <>
inline int DataManager::getParam( const std::string& strName, int defaultValue) const
{
	if (params_.find(strName) != params_.end())
	{
		return atoi(params_[strName].c_str());
	}
	else
	{
		return defaultValue;
	}
}

template <>
inline float DataManager::getParam(const std::string& strName, float defaultValue) const
{
	if (params_.find(strName) != params_.end())
	{
		return atof(params_[strName].c_str());
	}
	else
	{
		return defaultValue;
	}
}

template <>
inline double DataManager::getParam( const std::string& strName, double defaultValue) const
{
	if (params_.find(strName) != params_.end())
	{
		return atof(params_[strName].c_str());
	}
	else
	{
		return defaultValue;
	}
}

template <>
inline bool DataManager::getParam( const std::string& strName, bool defaultValue) const
{
	if (params_.find(strName) != params_.end())
	{
		return (utils::toLower(params_[strName]).compare("true")==0);
	}
	else
	{
		return defaultValue;
	}
}


} //namespace


#endif // DATAMANAGER_H




