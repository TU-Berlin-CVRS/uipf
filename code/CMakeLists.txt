cmake_minimum_required(VERSION 2.8.8)
project(UIPFramework)

find_package(Qt5Widgets)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

# define how the executable is built
#~ add_executable(uipf-gui main-gui.cpp) # gui
add_executable(uipf main-console.cpp framework/ModuleManager.cpp framework/Configuration.cpp framework/ModuleConfig.cpp) # console

# link the uipf program to opencv
#~ target_link_libraries(uipf-gui opencv_core opencv_imgproc opencv_highgui mainwindow) # gui
target_link_libraries(uipf opencv_core opencv_imgproc opencv_highgui) # console

# add qt modules to the gui
#~ qt5_use_modules(uipf-gui Core Gui Widgets)

# window library for the gui
#~ add_library(mainwindow gui/mainwindow.cpp)
#~ qt5_use_modules(mainwindow Widgets)



