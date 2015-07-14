Writing modules
===============

This document describes how custom Modules can be built for the uipf.

## What are Modules?

Modules are precompiled extensions that:

 - encapsulate functionality which can be used in processingsteps
 - are binary files, shareable without sourcecode (QTPlugin)
 - implement a simple interface
 - can include own libs as they need them
 - have Metadata displayed in the GUI
 
## How can I write a new Module?

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
