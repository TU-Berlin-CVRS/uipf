# General Concepts

The general idea of this framework is to split the functionality of complex programs into many small methods, which are called modules.

## Module

A module is a self-contained method in form of a precompiled shared library file. Each module implements the same interface, which makes the modules look all the same. Each of them have:

- name
- category

and can also have, if the module needs it: 

- inputs
- params
- outputs

The framework consists of a lot of such modules. The idea of the framework is that instead of writing a complex program, you just take the modules, the combination of which provides this wanted process and just let them interact in the way you need. This framework allows you to create nearly unlimited amount of possible programs using given modules, without any effort from your side.

You can extend the framework by [adding new modules](extending-modules.md) implementing the interface to make even more functionality available.

## Processing chain

A processing chain consists of a list of processing steps, where each step implements a certain module. The order of the execution depends on the dependencies between the single processing steps.
These dependencies are defined between the inputs and outputs of the modules.

The current version of the processing chain allows only a straight execution of the processing steps, and doesn't allow loops, if-else dependencies or any other logic. These improvements could be added in the future.

## Processing step

Each processing step uses one of the modules given by the framework. A processing step has the following properties:

- name 
- parameters
- inputs

The inputs define the dependencies between the single processing steps, because the input of one step is an output of another step.

# Storage format

The chains are stored in a [YAML](http://yaml.org/)-file. The chain consists of a list of processing steps. The order of the processing step is not relevant:
   
    stepX
    stepY
    stepZ

Each step is specified by the following syntax:

```yaml
stepName:
   module: moduleName
   input:
      inputName1: stepY.outputName1
      inputName2: stepZ.outputName1
      ...
   paramName1: paramValue
   paramName2: paramValue
   ...
```

Whitespace in the YAML file is relevant. The keywords `module` and `input` are predefined. The parameter names and the input names are defined by the MetaData of the specific module. The input content is a combination of the stepname, which provides the needed input, and the name of the output, which has to be taken, for the case the chosen module provides multiple outputs. The step name and its outputname are separated by a dot.

A simple example:

```yaml
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
```
