# General Concepts

The general idea of this framework is to split the functionality of complex programs in many small methods, which are called modules.

## Module
A module is a self contained method in form of an precompiled external library. Each module implements the same interface, which makes the modules look all the same. Each of them have:

- name
- category

and can also have, if the module needs it: 

- inputs
- params
- outputs

The framework consists of a lot of such modules. The idea of the framework is that instead of writing a complex program, you just take the modules, the combination of which provides this wanted process and just let them interact in the way you need. This framework allows you to create nearly unlimited amount of possible programs using given modules, without any effort from your side.

## Processing chain
Processing chain consists of a list processing steps, where each step implements a certain module. The order of the execution depends on the dependencies between the single processing steps.

The current version of the processing chain allows only a straightforward execution of the processing steps, and doesn't allow loops, if-else dependencies or any other logic. This improvements could be added in the future.

## Processing step
Each processing step uses one of the modules given by the framework. It has:

- name 
- parameters, if they are mandatory
- inputs, if they are mandatory

The inputs define the dependencies between the single processing steps, because the input of one step is an output of another step.

# Storage format
The chains are stored in a `.yaml` file. The chain consists of a list of processing step. The order of the processing step is not relevant:
   
    stepX
    stepY
    stepZ

Each step is specified by the following syntax:

    stepName
       module: moduleName
       input:
          inputName1: stepY.outputName1
          inputName2: stepZ.outputName1
          ...
       paramName1: paramValue
       paramName2: paramValue
       ...
       
The spaces are of relevance. The keywords module and inputs are predefined. The paramNames and the inputNames are defined by the MetaData of the specific module. The input content is a combination of the stepname, which provides the needed input, and the name of the output, which has to be taken, for the case the chosen module provides multiple outputs. The module name and its outputname are separated by a dot.

A simple example:

    sourceImage:
      module: loadImage
      filename: ../../testdata/Vintage-Tree-Nature-Background-HD-Wallpaper.jpg
    process:
      module: gaussian
      windowSize: 5
      sigmaX: 10.5
      input:
        image: sourceImage.image
    output:
      module: storeImage
      input:
        image: process.image
      filename: output.jPeg
      quality: 30
    showProcessed:
      module: showImage
      title: "processed nice image"
      blocking: false
      input:
        image: process.image
    showUnprocessed:
      module: showImage
      title: "nice image"
      blocking: true
      input:
        image: sourceImage.image
