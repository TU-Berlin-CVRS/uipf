System Overview
===============

The processing framework itself is independent from the user interface.
We provide two different interfaces to the framework.

1. GUI Interface which allows creating the chain
2. Console interface to run configured chains

The following is a schematic overview of th components:

    +-----------------+     +---------------------+   
    |                 |     |                     |   
    |  GUI Interface  |     |  Console Interface  |   
    |                 |     |                     |   
    +-------+---------+     +---------+-----------+   
            |                         |               
            |                         |               
            |                         |               
            |                         |               
            v                         v               
    +-------+-------------------------+-----------+   
    |                                             |   
    |                                             |   
    |          Processing Framework               |   
    |                                             |   
    |                                             |   
    +-------+-------------------+-----------+-----+   
            |                   |           |         
            |                   |           |         
    +-------+------+       +----+---+  +----+---+     
    |              |       |        |  |        |     
    | Configration |       | Module |  | Module |  ...
    |   Storage    |       |        |  |        |     
    |              |       +--------+  +--------+     
    +--------------+                                  

GUI Interface
-------------

TDB

Console Interface
-----------------

TBD

Configuration Storage
---------------------

Configuration of a processing chain should be stored in a file in a human readable format.

- It should be possible to write configuration files per hand.
- These files should be loadable and saveable by the framework

Modules
-------

Modules provide the processing functionality of the framework.

