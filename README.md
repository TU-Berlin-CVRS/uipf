Unified Image Processing Framework
==================================

This project is yet to be described... :)

How to build
------------

### Dependencies

The following dependencies are needed to build the console application:

- yaml-cpp (https://github.com/jbeder/yaml-cpp), will be built by cmake
  - Boost serialisation, needs to be available on the system
- Boost program-options

On Debian install the following packages:

	libboost-serialization-dev libboost-program-options-dev


The following dependencies are additionally needed to build the GUI:

- Qt 5

### Building with CMake on Linux

```
cd code
mkdir build
cd build
cmake ..
make
```

If something goes wrong you may run `make VERBOSE=1` for more detailed output.

### Building with CMake on Windows

TDB.

