#include "SelectListItemModule.hpp"
#include "../../framework/types/List.hpp"

using namespace std;
using namespace uipf;


// runs the module
/*
data	DataManager handles the input and ouput of this module
*/
void SelectListItemModule::run( DataManager& data) const
{
	using namespace cv;

	// get a pointers to the input data
	List<Data>::c_ptr pList = data.getInputData< List<Data> >("list");
	std::list<Data::ptr> list = pList->getContent();

	// get the index to select from the list
	int index = data.getParam<int>("index", 0);

	int i = 0;
	for(auto it = list.begin(); it != list.end(); ++it) {
		if (i == index) {
			// set outputs dependend on the type of list
			switch((*it)->getType()) {
				case STRING:
					data.setOutputData("string", new String(std::dynamic_pointer_cast<String>(*it)->getContent()) );
					break;
				case INTEGER:
					data.setOutputData("integer", new Integer(std::dynamic_pointer_cast<Integer>(*it)->getContent()) );
					break;
				case FLOAT:
					data.setOutputData("float", new Float(std::dynamic_pointer_cast<Float>(*it)->getContent()) );
					break;
				case BOOL:
					data.setOutputData("bool", new Bool(std::dynamic_pointer_cast<Bool>(*it)->getContent()) );
					break;
				case MATRIX: {
					Mat content = std::dynamic_pointer_cast<Matrix>(*it)->getContent();
					data.setOutputData("image", new Matrix(content) );
					break;
				}
				default:
					throw ErrorException("unexpected element type in the list.");
			}
			break;
		}
	}
	throw ErrorException("element with index " + to_string(index) + " was not found in the list.");
}

// returns the meta data of this module
MetaData SelectListItemModule::getMetaData() const
{
	map<string, DataDescription> input = {
		{"list", DataDescription(LIST, "the image list.") }
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
		"Selects one item from a list of items.",
		"Data Structures",
		input,
		output,
		params
	);
}

