#include "AddListItemModule.hpp"

using namespace std;
using namespace uipf;


// runs the module
/*
data	DataManager handles the input and ouput of this module
*/
void AddListItemModule::run( DataManager& data) const
{
	using namespace cv;

	std::list<Data::ptr> list;

	// if there is an input list, take it, otherwise we start with an empty list
	if (data.hasInputData("list")) {
		List<Data>::c_ptr pList = data.getInputData< List<Data> >("list");
		list = pList->getContent();
	}

	// add input item dependend on the type
	if (data.hasInputData("string")) {
		string s = data.getInputData< String >("string")->getContent();
		list.push_back(String::ptr(new String(s)));
	}
	else if (data.hasInputData("integer")) {
		int i = data.getInputData< Integer >("integer")->getContent();
		list.push_back(Integer::ptr(new Integer(i)));
	}
	else if (data.hasInputData("float")) {
		float f = data.getInputData< Float >("float")->getContent();
		list.push_back(Float::ptr(new Float(f)));
	}
	else if (data.hasInputData("bool")) {
		bool b = data.getInputData< Bool >("bool")->getContent();
		list.push_back(Bool::ptr(new Bool(b)));
	}
	else if (data.hasInputData("image")) {
		Mat m = data.getInputData< Matrix >("image")->getContent();
		list.push_back(Matrix::ptr(new Matrix(m)));
	}
	LOG_W("No input was given, an empty list has been created.");
}

// returns the meta data of this module
MetaData AddListItemModule::getMetaData() const
{
	map<string, DataDescription> input = {
		{"list", DataDescription(LIST, "optional input list, if empty, a new empty list will be created.", true) },
		{"string", DataDescription(STRING, "if it is a list of strings, the string element to add.", true) },
		{"integer", DataDescription(INTEGER, "if it is a list of integers, the integer element to add.", true) },
		{"float", DataDescription(FLOAT, "if it is a list of floats, the float element to add.", true) },
		{"bool", DataDescription(BOOL, "if it is a list of bools, the bool element to add.", true) },
		{"image", DataDescription(MATRIX, "if it is a list of images, the image to add.", true) }
	};
	map<string, DataDescription> output = {
		{"list", DataDescription(LIST, "the result list with an added element.") },
	};
	map<string, ParamDescription> params = {};

	return MetaData(
		"Adds an item to a list or creates a new list containing one item.",
		"Data Structures",
		input,
		output,
		params
	);
}

