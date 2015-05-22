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
add_executable(uipf
	main-console.cpp

	framework/ModuleManager.cpp
	framework/Configuration.cpp
	framework/Context.cpp
	framework/ModuleManager.cpp

	framework/types/Matrix.cpp
	framework/types/String.cpp

	modules/DummyModule.cpp
)

# link the uipf program to opencv
#~ target_link_libraries(uipf-gui opencv_core opencv_imgproc opencv_highgui mainwindow) # gui
target_link_libraries(uipf opencv_core opencv_imgproc opencv_highgui ${YAML_CPP_LIBRARY}) # console

# find Qt library for building the GUI
find_package(Qt5Widgets REQUIRED)


#create gui-executable
add_executable(uipf-gui main-gui.cpp)
target_link_libraries (uipf-gui mainwindow)

# add qt modules to the gui
qt5_use_modules(uipf-gui Core Gui Widgets)

#manually generate ui_headers
QT5_WRAP_UI(UI_HS
    gui/mainwindow.ui
   )

# window library for the gui
add_library(mainwindow gui/mainwindow.cpp ${UI_HS})
qt5_use_modules(mainwindow Widgets)




#
# external dependencies
# http://www.cmake.org/cmake/help/v2.8.8/cmake.html#module%3aExternalProject
#


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




