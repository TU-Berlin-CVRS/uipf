Modules
=======

## Interface

A module consists of a library file that contains a C++ class which extends a defined interface.

A module class has to extend from `ModuleInterface` and implement the following methods:

	virtual void run( std::map<std::string, uipf::Data::ptr& >& input, std::map<std::string, std::string >& params, std::map<std::string, uipf::Data::ptr >& output) const = 0;

- `input` is a std::map of input resources, the names are described in the module meta description
- `params` is a std::map of input parameters, the names are described in the module meta description
- `ouput` is a std::map of output resources, the names are described in the module meta description

TODO: describe meta data methods

    virtual void setContext(Context* context) = 0;
    
`context` is a container providing access to the current environment, allowing to open windows, write to logger etc...

## Input and output data

The input and output data abstraction is done by implementing wrapper classes for the data types that extend from the class `Data`.
This class defines a virtual method `getType()` which returns an instance of the enum `Type` which has the following types:

- STRING - data is represented as a `std::string`.
- INTEGER - data is represtented as a simple integer
- FLOAT - data is represtented as a simple float
- MATRIX - data is represented as an OpenCV Matrix (`cv::Mat`)

TODO: describe maps and lists.

Loading of dynamic libraries
----------------------------

The Qt Plugin feature is used to load modules dynamically.

TODO more detailed description.

### Resources

- "Dynamic Libraries in C++" <http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2002/n1418.html#manually%20load>