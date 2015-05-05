Modules
=======

A module consists at least of a library file and a meta information file.

A library has to implement a defined interface

    void run(vector input, vector output, Context context)
    
- `input` is a std::vector of input resources, order is described in the module meta description
- `ouput` is a std::vector of output resources, order is described in the module meta description
- `context` is a container providing access to the current environment, allowing to open windows, write to logger etc...

Input and output can have the following data types (specifics to be defined):

- Image with arbitrary number of channels
  - Grayscale, RGB, CMYK, ...?
  - PolSAR
  - OpenCV Mat?
- integer
- float
- string
- list/vector of one of the above types

