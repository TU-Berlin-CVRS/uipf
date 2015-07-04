#include "MetaData.hpp"
#include "InvalidConfigException.hpp"

using namespace uipf;
using namespace std;

MetaData::MetaData(string description, string category, map<string, DataDescription> inputs, map<string, DataDescription> outputs, map<string, ParamDescription> params)
{
	inputs_ = inputs;
	outputs_ = outputs;
	params_ = params;
	description_ = description;
	category_ = category;
}

string MetaData::getCategory() const {
	return category_;
}

string MetaData::getDescription() const {
	return description_;
}

map<string, DataDescription> MetaData::getInputs() const {
	return inputs_;
}

map<string, DataDescription> MetaData::getOutputs() const {
	return outputs_;
}

map<string, ParamDescription> MetaData::getParams() const {
	return params_;
}


// get input by name
DataDescription MetaData::getInput(const std::string& strName) const
{
	auto res = inputs_.find(strName);
	if (res != inputs_.end())
		return res->second;

	throw InvalidConfigException(std::string("input data '") + strName + std::string("' not found in Metadata!"));
}

// get output by name
DataDescription MetaData::getOutput(const std::string& strName) const
{
	auto res = outputs_.find(strName);
	if (res != outputs_.end())
		return res->second;

	throw InvalidConfigException(std::string("output data '") + strName + std::string("' not found in Metadata!"));
}

// get param by name
ParamDescription MetaData::getParam(const std::string& strName) const
{
	auto res = params_.find(strName);
	if (res != params_.end())
		return res->second;

	throw InvalidConfigException(std::string("param '") + strName + std::string("' not found in Metadata!"));
}
