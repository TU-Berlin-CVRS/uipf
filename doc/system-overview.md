System Overview
===============

The processing framework itself is independent from the user interface.
We provide two different interfaces to the framework.

1. GUI Interface which allows creating the chain
2. Console interface to run configured chains

The following is a schematic overview of the components:

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

