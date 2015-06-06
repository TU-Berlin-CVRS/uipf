#include "MetaData.hpp"

using namespace uipf;
using namespace std;

MetaData::MetaData(string description, map<string, DataDescription> inputs, map<string, DataDescription> outputs, map<string, ParamDescription> params){
	description_ = description;
	inputs_ = inputs;
	outputs_ = outputs;
	params_ = params;
}

string MetaData::getDescription(){
	return description_;
}

map<string, DataDescription> MetaData::getInputs(){
	return inputs_;
}

map<string, DataDescription> MetaData::getOutputs(){
	return outputs_;
}

map<string, ParamDescription> MetaData::getParams(){
	return params_;
}

