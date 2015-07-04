#ifndef DATAMANAGER_H
#define DATAMANAGER_H


#include "StdIncl.hpp"
#include "types/Data.hpp"
#include "InvalidConfigException.hpp"
#include "Utils.hpp"

namespace uipf{

// Provides and controls access to Parameters, Input and Outputdata
class DataManager
{

	public:
		// constructor
		DataManager(
			std::map < std::string, Data::ptr& >& input, // TODO is the reference to the pointer really needed?
			std::map < std::string, std::string >& params,
			std::map < std::string, Data::ptr >& output
		) : input_(input), params_(params), output_(output) {};

		// destructor
		~DataManager() {};

		// check whether named input data is given
		bool hasInputData( const std::string& strName) const;

		// returns a typesafe readonly smartpointer to input data by name if it is available
		template <typename T>
		const typename T::ptr getInputData( const std::string& strName) const;

		// returns a typesafe smartpointer to output data by name if it is available
		template <typename T>
		typename T::ptr getOutputData( const std::string& strName) const;

		// sets a typesafe smartpointer for output data by name
		template <typename T>
		void setOutputData( const std::string& strName, T*);

		// returns a typesafe parameter by name if it is available. otherwise a defaultValue is used.
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
	auto it = input_.find(strName);
	if (it != input_.end())
	{
		//do downcasting..
		Data::ptr& ptr = it->second;
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
	auto it = output_.find(strName);
	if (it != output_.end())
	{
		//do downcasting..
		Data::ptr& ptr = it->second;
		return std::dynamic_pointer_cast<T>(ptr);
	}
	else
	{
		throw InvalidConfigException(std::string("output data '") + strName + std::string("' not found!"));
	}
}

template <typename T>
void DataManager::setOutputData( const std::string& strName, T* outputData)
{
	output_.insert (std::pair < std::string, Data::ptr >(strName, typename T::ptr(outputData)));
}


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
	if (params_.find(strName) != params_.end() && !params_[strName].empty())
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
	if (params_.find(strName) != params_.end() && !params_[strName].empty())
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
	if (params_.find(strName) != params_.end() && !params_[strName].empty())
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
	if (params_.find(strName) != params_.end() && !params_[strName].empty())
	{
		std::string lower = utils::toLower(params_[strName]);
		return lower.compare("true") == 0
			|| lower.compare("yes") == 0
			|| lower.compare("y") == 0
			|| lower.compare("t") == 0
			|| lower.compare("1") == 0;
	}
	else
	{
		return defaultValue;
	}
}


} //namespace

#endif // DATAMANAGER_H
