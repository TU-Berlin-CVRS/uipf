# The internal type system

## Existing data types

The framework provides currently the following data types:

1. primitive data types:
    - `string`
    - `integer`
    - `float`
    - `bool`
    - `matrix`

2. complex data types:
    - `list<string>`
    - `list<integer>`
    - `list<float>`
    - `list<bool>`
    - `list<matrix>`

All of this data types implement the interface `data.hpp`

## How to use the data types
The data types are used by the modules to specify the types of the inputs and outputs.
It is used in the `getMetadata()` method of the module, when specifying the input and output maps:

	map<string, DataDescription> input = {
		{"nameX", DataDescription(DATATYPE, "explanation"),
		 "nameY", DataDescription(DATATYPE, "explanation"),
		 ...}
	};
	map<string, DataDescription> output = {
		{"nameZ", DataDescription(DATATYPE, "explanation"),
		 "nameA", DataDescription(DATATYPE, "explanation"),
		 ...}
	};


## How to implement a new data type

- add an enum in Type

        enum Type {
            NEWTYPE
        };

- case: standard class:		create a new .hpp and .cpp classes, which derive from the class `Data.hpp`
- case: template class:		create a new .hpp template class, which derive from the class `Data.hpp`

        class Data {
        	public:
        		typedef SMARTPOINTER<Data> ptr;
        		typedef const SMARTPOINTER<Data> c_ptr;
        
        	public:
        		// constructor
        		Data(void){};
        		// destructor
        		virtual ~Data(void){};
        
        		// returns the data type of this data object
        		// this is a virtual method, which has to be overwritten in the class, which derives of Data
        		virtual Type getType() = 0;
        
        };

the classes should also have a value to store the content, and methods:

    public:
    	newType getContent();
    	void setContent(newType);

    private:
        newType value;
