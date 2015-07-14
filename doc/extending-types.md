# The internal type system

## Existing data types

The framework currently provides the following data types:

1. primitive data types:
    - `STRING`, a `std::string`
    - `INTEGER`, a simple `int`
    - `FLOAT`, a simple `float`
    - `BOOL`, a simple `bool`
    - `MATRIX`, wraps the [OpenCV `cv::Mat`](http://docs.opencv.org/modules/core/doc/basic_structures.html#mat)

2. complex data types:
    - `STRING_LIST`, a list of `STRING`s
    - `INTEGER_LIST`, a list of `INTEGER`s
    - `FLOAT_LIST`, a list of `FLOAT`s
    - `BOOL_LIST`, a list of `BOOL`s
    - `MATRIX_LIST`, a list of `MATRIX`s

All of these data types implement the interface [`Data`](../code/framework/types/Data.hpp).

## How to use the data types

The data types are used by the modules to specify the types of the inputs and outputs.
It is used in the `getMetadata()` method of the module, when specifying the input and output maps:

```cpp
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
```

## How to implement a new data type

- add an enum in Type

  ```cpp
  enum Type {
      NEWTYPE
  };
  ```

- case: standard class: create a new `.hpp` and `.cpp` classes, which derive from the interface [`Data.hpp`](../code/framework/types/Data.hpp).
- case: template class: create a new `.hpp` template class, which derive from the interface [`Data.hpp`](../code/framework/types/Data.hpp).

  Define the public smart pointers:
  
  ```cpp
  public:
	typedef SMARTPOINTER<Data> ptr;
	typedef const SMARTPOINTER<Data> c_ptr;
  ```

  The following virtual method has to be implemented and it should return the newly created ENUM value:
  
  ```cpp
  public:
	// returns the data type of this data object
	Type getType();
  ```

  the classes should also have a value and methods, to store the content:

  ```cpp
  public:
    	newType getContent();
    	void setContent(newType);

  private:
        newType value;
  ```
