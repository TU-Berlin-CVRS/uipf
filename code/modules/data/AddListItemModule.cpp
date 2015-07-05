#include "AddListItemModule.hpp"

using namespace std;
using namespace uipf;

// helper function to add an item to the list
template<typename T, typename I>
List<T>* addItemToList(DataManager& data, const string& inputListName, const string& inputItemName) {

	List<T>* newList;

	// if a list has been given, use it
	if (data.hasInputData(inputListName)) {
		auto list = data.getInputData< List<T> >(inputListName)->getContent();
		newList = new List<T>(list);
	} else {
		newList = new List<T>();
	}
	if (data.hasInputData(inputItemName)) {
		typename T::c_ptr item = data.getInputData< T >(inputItemName);
		I i = item->getContent();
		newList->addItem(new T(i));
	} else {
		LOG_W("No input was given for " + inputItemName + ", no element was added to the list.");
	}
	return newList;
}

// runs the module
/*
data	DataManager handles the input and ouput of this module
*/
void AddListItemModule::run( DataManager& data) const
{
	using namespace cv;

	// select input list dependend on the type
	if (data.hasInputData("stringList") || data.hasInputData("string")) {
		data.setOutputData("stringList", addItemToList<String, string>(data, "stringList", "string"));
	}
	if (data.hasInputData("integerList") || data.hasInputData("integer")) {
		data.setOutputData("integerList", addItemToList<Integer, int>(data, "integerList", "integer"));
	}
	if (data.hasInputData("floatList") || data.hasInputData("float")) {
		data.setOutputData("floatList", addItemToList<Float, float>(data, "floatList", "float"));
	}
	if (data.hasInputData("boolList") || data.hasInputData("bool")) {
		data.setOutputData("boolList", addItemToList<Bool, bool>(data, "boolList", "bool"));
	}
	if (data.hasInputData("imageList") || data.hasInputData("image")) {
		data.setOutputData("imageList", addItemToList<Matrix, Mat>(data, "imageList", "image"));
	}
}

// returns the meta data of this module
MetaData AddListItemModule::getMetaData() const
{
	map<string, DataDescription> input = {
		{"stringList", DataDescription(STRING_LIST, "a list of strings.", true) },
		{"integerList", DataDescription(INTEGER_LIST, "a list of integers.", true) },
		{"floatList", DataDescription(FLOAT_LIST, "a list of floats.", true) },
		{"boolList", DataDescription(BOOL_LIST, "a list of bools.", true) },
		{"imageList", DataDescription(MATRIX_LIST, "a list of images.", true) },

		{"string", DataDescription(STRING, "if it is a list of strings, the string element to add.", true) },
		{"integer", DataDescription(INTEGER, "if it is a list of integers, the integer element to add.", true) },
		{"float", DataDescription(FLOAT, "if it is a list of floats, the float element to add.", true) },
		{"bool", DataDescription(BOOL, "if it is a list of bools, the bool element to add.", true) },
		{"image", DataDescription(MATRIX, "if it is a list of images, the image to add.", true) }
	};
	map<string, DataDescription> output = {
		{"stringList", DataDescription(STRING_LIST, "the result list with an added element. a list of strings.") },
		{"integerList", DataDescription(INTEGER_LIST, "the result list with an added element. a list of integers.") },
		{"floatList", DataDescription(FLOAT_LIST, "the result list with an added element. a list of floats.") },
		{"boolList", DataDescription(BOOL_LIST, "the result list with an added element. a list of bools.") },
		{"imageList", DataDescription(MATRIX_LIST, "the result list with an added element. a list of images.") }
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

