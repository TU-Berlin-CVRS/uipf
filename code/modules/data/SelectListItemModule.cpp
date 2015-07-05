#include "SelectListItemModule.hpp"
#include "../../framework/types/List.hpp"

using namespace std;
using namespace uipf;


// helper function to select an item from the list
template<typename T, typename I>
I selectItemFromList(DataManager& data, string inputName, int index) {

	typename List<T>::ptr list = data.getInputData< List<T> >(inputName);
	if (list->hasItem(index)) {
		return list->getItem(index)->getContent();
	} else {
		throw ErrorException("element with index " + to_string(index) + " was not found in the " + inputName + ".");
	}

}

// runs the module
/*
data	DataManager handles the input and ouput of this module
*/
void SelectListItemModule::run( DataManager& data) const
{
	using namespace cv;

	// get the index to select from the list
	int index = data.getParam<int>("index", 0);

	// select input list dependend on the type
	if (data.hasInputData("stringList")) {
		data.setOutputData("string", new String(selectItemFromList<String, string>(data, "stringList", index)));
	}
	if (data.hasInputData("integerList")) {
		data.setOutputData("integer", new Integer(selectItemFromList<Integer, int>(data, "integerList", index)));
	}
	if (data.hasInputData("floatList")) {
		data.setOutputData("float", new Float(selectItemFromList<Float, float>(data, "floatList", index)));
	}
	if (data.hasInputData("boolList")) {
		data.setOutputData("bool", new Bool(selectItemFromList<Bool, bool>(data, "boolList", index)));
	}
	if (data.hasInputData("imageList")) {
		Mat m = selectItemFromList<Matrix, Mat>(data, "imageList", index);
		data.setOutputData("image", new Matrix(m));
	}
}

// returns the meta data of this module
MetaData SelectListItemModule::getMetaData() const
{
	map<string, DataDescription> input = {
		{"stringList", DataDescription(STRING_LIST, "a list of strings.", true) },
		{"integerList", DataDescription(INTEGER_LIST, "a list of integers.", true) },
		{"floatList", DataDescription(FLOAT_LIST, "a list of floats.", true) },
		{"boolList", DataDescription(BOOL_LIST, "a list of bools.", true) },
		{"imageList", DataDescription(MATRIX_LIST, "a list of images.", true) }
	};
	map<string, DataDescription> output = {
		{"string", DataDescription(STRING, "the extracted string value, if it was a list of strings.") },
		{"integer", DataDescription(INTEGER, "the extracted integer value, if it was a list of integers.") },
		{"float", DataDescription(FLOAT, "the extracted float value, if it was a list of floats.") },
		{"bool", DataDescription(BOOL, "the extracted bool value, if it was a list of bools.") },
		{"image", DataDescription(MATRIX, "the extracted image, if it was a list of images.") }
	};
	map<string, ParamDescription> params = {
		{"index", ParamDescription("index of the element to extract from the list.") }
	};

	return MetaData(
		"Selects one item from a list of items. Set the corresponding input dependend on the type you need.",
		"Data Structures",
		input,
		output,
		params
	);
}

