Unified Image Processing Framework
==================================

This project is yet to be described... :)

How to build
------------

### Dependencies

The following dependencies are needed to build:

- yaml-cpp (https://github.com/jbeder/yaml-cpp), will be built by cmake
  - [Boost serialisation](http://www.boost.org/doc/libs/release/libs/serialization/), needs to be available on the system
- [Boost program-options](http://www.boost.org/doc/libs/release/libs/program_options/)
- [Boost graph](http://www.boost.org/doc/libs/release/libs/graph/)
- [Qt 5](http://doc.qt.io/qt-5/index.html), at least 5.3

On Debian/Ubuntu you can install the above packages by running the following command:

	sudo apt-get install libboost-serialization-dev libboost-program-options-dev libboost-graph-de qtbase5-dev qtbase5-dev-tools

The following additional packages may be needed for building C++ code:

    sudo apt-get install build-essential cmake pkg-config


### Building with CMake on Linux

Run the following commands after cloning the repository in the repostiory base directory:

```
mkdir -p code/build
cd code/build
cmake ..
make
```

If something goes wrong you may run `make VERBOSE=1` for more detailed output.

### Building with CMake on Windows

TDB.