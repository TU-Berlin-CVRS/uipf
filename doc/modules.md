Modules
=======

## Interface

A module consists of a library file that contains a C++ class which extends a defined interface.

A module class has to extend from `ModuleInterface` and implement the following methods:

	virtual void run( DataManager& data) const = 0;

- `data` is a Reference to the DataManager, which holds all resources described in the module meta description.
  It enables typed access to input-, outputdata and the parameters by three specific methods:
   - getInputData&lt;Type&gt;(name),
   - setOutputData&lt;Type&gt;(name,val) and
   - getParameter&lt;Type&gt;(name,defaultVal).

   For example a readonly String resource can be retrieved as follows:
   <pre><code>String::c_ptr myString = data.getInputData<String>("myStringName");</code></pre>
    The String::c_ptr is a shortcut to 'const std::shared_ptr&lt;uipf::String&gt;' as resources are always handled by smartpointers to prevent memoryleaks.


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

TODO: describe lists.

Loading of dynamic libraries
----------------------------

The Qt Plugin feature is used to load modules dynamically.

TODO more detailed description.

### Resources

- "Dynamic Libraries in C++" <http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2002/n1418.html#manually%20load>
