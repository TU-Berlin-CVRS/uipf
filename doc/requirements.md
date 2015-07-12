# Summary Requirements Unified Image Processing Framework #

----------

## General ##

* Plattform independent with linux as primary target (C++,Qt, Boost, OpenCV)
* GUI and cmd-line mode 
* Graph-/Node-based
* Realtime nice but not needed
* Focus on framework-design
* CMake

## Modules

* Capsulate functionality like Filters, wrappers for OpenCV etc.
* Have their own GUI-window
* Extensible (binary compiled plugin model)
* Are part of processing chain and can be connected
* Logging

### Datasources

* Can be all types of images, binary data, Metadata (xml)
* Should be fuseable (enhance and complement each other e.g. with Metadata)
* Efficient handling of big datasources (in terms of memory consumption)
  * Blockbased processing
* Writeable vs. Locked

### Datasinks

* Store, visualize results
* Output in arbitrary format

## Persistence

* Configfile with settings (yaml)
* Store graph and module settings
* Every module has its own settings

## Open

Coding Style
...
