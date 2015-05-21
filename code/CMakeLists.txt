cmake_minimum_required(VERSION 2.8.8)

# load CMake modules
include(ExternalProject)

# project name
project(UIPFramework)


# CMake settings
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)


#
# Create executeables
#

# define how the executable is built
#~ add_executable(uipf-gui main-gui.cpp) # gui
add_executable(uipf main-console.cpp framework/ModuleManager.cpp framework/Configuration.cpp framework/ModuleConfig.cpp) # console

# link the uipf program to opencv
#~ target_link_libraries(uipf-gui opencv_core opencv_imgproc opencv_highgui mainwindow) # gui
target_link_libraries(uipf opencv_core opencv_imgproc opencv_highgui ${YAML_CPP_LIBRARY}) # console

# add qt modules to the gui
#~ qt5_use_modules(uipf-gui Core Gui Widgets)

# window library for the gui
#~ add_library(mainwindow gui/mainwindow.cpp)
#~ qt5_use_modules(mainwindow Widgets)


#
# external dependencies
# http://www.cmake.org/cmake/help/v2.8.8/cmake.html#module%3aExternalProject
#

# find Qt library for building the GUI
find_package(Qt5Widgets REQUIRED)

# yaml-cpp
ExternalProject_Add(YAML_CPP
	GIT_REPOSITORY https://github.com/jbeder/yaml-cpp.git
	GIT_TAG release-0.5.2
	CMAKE_ARGS -DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_CURRENT_BINARY_DIR}/yaml-cpp -DBUILD_SHARED_LIBS=YES
)
include_directories(${CMAKE_CURRENT_BINARY_DIR}/yaml-cpp/include)
FIND_LIBRARY(YAML_CPP_LIBRARY yaml-cpp ${CMAKE_CURRENT_BINARY_DIR}/yaml-cpp/lib)

add_dependencies(uipf YAML_CPP)
add_dependencies(uipf YAML_CPP_LIBRARY)
#add_dependencies(uipf yaml-cpp)


