#include "MetaData.hpp"

using namespace uipf;
using namespace std;

MetaData::MetaData(string d, map<string, Type> i, map<string, Type> o){
	description = d;
	outputs = o;
	inputs = i;
}

string MetaData::getDescription(){
	return description;
}

map<string, Type> MetaData::getInputs(){
	return inputs;
}

map<string, Type> MetaData::getOutputs(){
	return outputs;
}

