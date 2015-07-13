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

#c++11 support
#if(UNIX)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=gnu++0x")
#endif()

# find Qt library for building the GUI
find_package(Qt5Widgets REQUIRED)


#
# Create executeables
#

# put the core framework into a shard lib
add_library(uipf-framework SHARED

	framework/ModuleManager.cpp
	framework/Context.cpp
	framework/GUIEventDispatcher.cpp
	framework/Logger.cpp

	framework/Configuration.cpp
	framework/MetaData.cpp

	framework/DataManager.cpp
	framework/types/Data.cpp
	framework/types/String.cpp
	framework/types/Float.cpp
	framework/types/Integer.cpp
	framework/types/Bool.cpp
	framework/types/Matrix.cpp

    framework/Utils.cpp
)
# we need to set the path to libyaml-cpp.so so it can be found by linker
set(YAML_CPP_LIBRARY ${CMAKE_BINARY_DIR}/yaml-cpp/lib/libyaml-cpp.so)
target_link_libraries(uipf-framework opencv_core opencv_imgproc opencv_highgui ${YAML_CPP_LIBRARY})
qt5_use_modules(uipf-framework Core Widgets) # Qt is required for loading modules dynamically

# add one executeable for the console application
add_executable(uipf
	main-console.cpp
)
target_link_libraries(uipf boost_program_options uipf-framework)
qt5_use_modules(uipf Core Gui Widgets) # Qt is required for loading modules dynamically



# manually generate ui_headers
QT5_WRAP_UI(UI_HS
    gui/mainwindow.ui
)

# this command will generate rules that will run rcc on all listed files
# in result RC_SRCS variable will contain paths to files produced by rcc
qt5_add_resources(RC_SRCS gui/mainwindow.qrc)

# add executeable for the gui application
add_executable(uipf-gui

	main-gui.cpp
	gui/MainWindow.cpp
	${UI_HS}

	gui/ImageWindow.cpp

	gui/graph/edge.cpp
	gui/graph/node.cpp
	gui/graph/graphwidget.cpp

	gui/models/ParamsModel.cpp
	gui/models/InputsDelegate.cpp

	gui/RunWorkerThread.cpp

	${RC_SRCS}
)
target_link_libraries (uipf-gui boost_graph uipf-framework)
qt5_use_modules(uipf-gui Core Gui Widgets)


add_library(ModuleBase SHARED framework/ModuleBase.cpp )
qt5_use_modules(ModuleBase Core )

#
# Dynamic Modules
#

# dummy
add_library(DummyModule SHARED modules/dummy/DummyModule.cpp)
qt5_use_modules(DummyModule Core ) #QtCore is needed for <QPlugin>
target_link_libraries(DummyModule ModuleBase)

add_library(LongRunningDummyModule SHARED modules/dummy/LongRunningDummyModule.cpp)
qt5_use_modules(LongRunningDummyModule Core ) #QtCore is needed for <QPlugin>
target_link_libraries(LongRunningDummyModule ModuleBase)

# I/O
add_library(LoadImageModule SHARED modules/io/LoadImageModule.cpp)
qt5_use_modules(LoadImageModule Core ) #QtCore is needed for <QPlugin>
target_link_libraries(LoadImageModule opencv_core ModuleBase)

add_library(StoreImageModule SHARED modules/io/StoreImageModule.cpp)
qt5_use_modules(StoreImageModule Core ) #QtCore is needed for <QPlugin>
target_link_libraries(StoreImageModule opencv_core ModuleBase)

add_library(ShowImageModule SHARED modules/io/ShowImageModule.cpp)
qt5_use_modules(ShowImageModule Core ) #QtCore is needed for <QPlugin>
target_link_libraries(ShowImageModule opencv_core opencv_imgproc opencv_highgui ModuleBase)

add_library(ShowImageListModule SHARED modules/io/ShowImageListModule.cpp)
qt5_use_modules(ShowImageListModule Core ) #QtCore is needed for <QPlugin>
target_link_libraries(ShowImageListModule opencv_core opencv_imgproc opencv_highgui ModuleBase)

# Image Processing
add_library(GaussianModule SHARED modules/improc/GaussianModule.cpp)
qt5_use_modules(GaussianModule Core ) #QtCore is needed for <QPlugin>
target_link_libraries(GaussianModule opencv_core opencv_imgproc ModuleBase)

#for live demo...
#add_library(ResizeModule SHARED modules/improc/ResizeModule.cpp)
#qt5_use_modules(ResizeModule Core ) #QtCore is needed for <QPlugin>
#target_link_libraries(ResizeModule opencv_core opencv_imgproc ModuleBase)

add_library(ConvolutionModule SHARED modules/improc/ConvolutionModule.cpp)
qt5_use_modules(ConvolutionModule Core ) #QtCore is needed for <QPlugin>
target_link_libraries(ConvolutionModule opencv_core opencv_imgproc ModuleBase)

add_library(SplitChannelsModule SHARED modules/improc/SplitChannelsModule.cpp)
qt5_use_modules(SplitChannelsModule Core ) #QtCore is needed for <QPlugin>
target_link_libraries(SplitChannelsModule opencv_core opencv_imgproc ModuleBase)

add_library(MergeChannelsModule SHARED modules/improc/MergeChannelsModule.cpp)
qt5_use_modules(MergeChannelsModule Core ) #QtCore is needed for <QPlugin>
target_link_libraries(MergeChannelsModule opencv_core opencv_imgproc ModuleBase)


# Data structures
add_library(SelectListItemModule SHARED modules/data/SelectListItemModule.cpp)
qt5_use_modules(SelectListItemModule Core ) #QtCore is needed for <QPlugin>
target_link_libraries(SelectListItemModule opencv_core opencv_imgproc ModuleBase)

add_library(AddListItemModule SHARED modules/data/AddListItemModule.cpp)
qt5_use_modules(AddListItemModule Core ) #QtCore is needed for <QPlugin>
target_link_libraries(AddListItemModule opencv_core opencv_imgproc ModuleBase)


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

add_dependencies(uipf-framework YAML_CPP)
add_dependencies(uipf-framework YAML_CPP_LIBRARY)

