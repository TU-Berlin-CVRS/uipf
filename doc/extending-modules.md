Writing modules
===============

This document describes how custom Modules can be built for the uipf.

## What are Modules?

Modules are precompiled extensions/plugins that:

 - encapsulate functionality which can be used in processing steps
 - are binary files, shareable without sourcecode (QTPlugin)
 - implement a simple interface
 - can include own libs as they need them
 - have Metadata displayed in the GUI
 
## How can I write a new Module?

### Introduction

To write your own Module you basically have to do the following steps:
 
1. Create a c++ class that derives from ModuleInterface. This mainly requires three Methods:

   ```cpp
   string name () ;
   void run ( DataManager & data ) ;
   MetaData getMetaData () ;
   ```

   For ease of use you simply can copy the `.hpp` and `.cpp` of an existing module like the DummyModule.
   Then just rename all "DummyModule" occurences with your new Modulenname.

2. Implement your logic in the `run()`-method. This method gets an object of DataManager, which provides you with the needed Inputdata as well as Parameters
   and lets you set output values. A skeleton could look like this:

   ```cpp
   void LoadImageModule :: run ( DataManager & data ) const
   {
   // (1) get inputs and params :
   // - data.getInputData ( inputName )
   // - data.getParam ( paramName , dafaultValue )
   // (2) work with them
   // (3) create output :
   // - data.setOutputData ( outputName ,outputContent ) ;
   }
   ```
   
3. Define your Metadata, that will communicate the requirements of your Module (inputs, outputS and parameters) as well as a description and a category.
   
   Here as an example is a ResizeModule, that uses a width and a height as parameters:

   ```cpp
   MetaData ResizeModule::getMetaData () const
   {
       DataDescriptionMap input = {{ " image " ,DataDescription ( MATRIX , " the image to resize . " ) }};
       DataDescriptionMap output = {{ " image " ,DataDescription ( MATRIX , " the result image . " ) }};
       ParamDescriptionMap params = {{ " width " , ParamDescription ( " new width " ) } ,{ " height " , ParamDescription ( " new height " ) }};
       return MetaData ( " Resizes an image using openCV . " ," Image Processing " , input , output , params ) ;
   }
   ```
  
4. The last step before you can build your module is to edit the `CMakeList.txt` and add your module to be built as a shared library.

   ```cpp 
   ...
   # Image Processing
   # ResizeModule
   add_library ( ResizeModule SHARED modules/improc/ResizeModule.cpp ) #your new cpp-file
   qt5_use_modules ( ResizeModule Core ) # QtCore is needed for < QPlugin >
   target_link_libraries ( ResizeModule opencv_core opencv_imgproc ModuleBase )
   ...
   ```
   
   Of course you don't need to use the `CMakeList.txt` of the whole project. You may create your own one just for your Module.

Now you can build your new module with `make` and use it in the GUI as well as the console.

### Compiling your module

If your module is not generic enough to make it part of UIPF itself, you can
will likely be working in a separte directory when developing it. 

#### Compiling with CMake

Assuming the following file structure in your module:

- `src/ExampleModule.hpp`:
  
  ```cpp
  #include <uipf/framework/ModuleBase.hpp>
  
  class ExampleModule :  public QObject, uipf::ModuleBase
  {
  	Q_OBJECT
  	Q_PLUGIN_METADATA(IID "org.tu-berlin.uipf.ExampleModule" )
  	Q_INTERFACES(uipf::ModuleInterface)
  
  public:
  	// constructor tells ModuleBase our name so we don't need to implement name()
  	ExampleModule(void) : uipf::ModuleBase("example"){};
  
  	// destructor needs to be virtual otherwise it not called due polymorphism
  	virtual ~ExampleModule(void){};
  
  	void run( uipf::DataManager& data ) const Q_DECL_OVERRIDE;
  
  	uipf::MetaData getMetaData() const Q_DECL_OVERRIDE;
  };
  ```

- `src/ExampleModule.cpp`:

  ```cpp
  #include "ExampleModule.hpp"
  
  using namespace uipf;
  
  void ExampleModule::run( DataManager& data) const
  {
  	// implement module functionality here working on input/output data and parameters using DataManager
  }
  
  MetaData ExampleModule::getMetaData() const
  {
  	using namespace std;
  
  	map<string, DataDescription> input = {
  		// describe inputs here
  	};
  	map<string, DataDescription> output = {
  		// describe outputs here
  	};
  	map<string, ParamDescription> params = {
  		// describe parameters here
  	};
  
  	return MetaData(
  			"A minimal Example module",
  			"pathSfM",
  			input,
  			output,
  			params
  	);
  }
  ```

To compile your module with [cmake](https://cmake.org/), create the following `CMakelists.txt` file in the root folder:

```cmake
cmake_minimum_required(VERSION 2.8.8)
project(example)

set(CMAKE_CXX_STANDARD 11)

# make Qt work
set(CMAKE_AUTOMOC ON)
# make sure MOC will find UIPF
SET(CMAKE_AUTOMOC_MOC_OPTIONS
-I/usr/local/include
)
# find Qt library for QPlugin
find_package(Qt5Widgets REQUIRED)
find_library(ModuleBase uipf-ModuleBase)

# build example module
add_library(ExampleModule SHARED src/ExampleModule.cpp)
qt5_use_modules(ExampleModule Core) #QtCore is needed for <QPlugin>
target_link_libraries(ExampleModule ${ModuleBase})
```

You can now compile your module using the following commands:

    mkdir build && cd build
    cmake ..
    make
    
This will create a `libExampleModule.so` file in the current directory, which is the module binary file.

#### Configure UIPF to load the module

When started, UIPF will search for a module configuration file in the following locations:

- `/usr/lib/uipf`
- `/usr/local/lib/uipf`
- Additionally to the above two paths, you can configure more
  search paths by creating a configuration file.
  The configuration file can be located in the following locations:
  
  - current working directory (`./modules.yaml`)
  - your home directory (`~/.uipf-modules.yaml`)
  - system-wide configuration (`/etc/uipf/modules.yaml`)
  
  The configuration file should contain a [YAML](http://www.yaml.org/) list of paths to search for modules, e.g.:
  
  ```yaml
  - ~/project1/uipf-modules/build
  - ~/project2/uipf-modules/build
  ```


### Logging and error handling

TBD

Logging makros

```
LOG_I(
LOG_E(
LOG_W(
```

Error handling:

To abort the run of the module you can throw any exception. There is one exception class provided which can carry a
message:
`throw InvalidConfigException("the object mesh is invalid");`




