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

#
# Create executeables
#

# define how the executable is built
#~ add_executable(uipf-gui main-gui.cpp) # gui
add_executable(uipf
	main-console.cpp

	framework/ModuleManager.cpp

	framework/Configuration.cpp
	framework/DataManager.cpp
	framework/Context.cpp
	framework/Logger.cpp
	framework/MetaData.cpp
    framework/Utils.cpp

	framework/types/Matrix.cpp
	framework/types/String.cpp
	framework/types/Float.cpp
	framework/types/Integer.cpp

)






# link the uipf program to opencv
#~ target_link_libraries(uipf-gui opencv_core opencv_imgproc opencv_highgui mainwindow) # gui
target_link_libraries(uipf opencv_core opencv_imgproc opencv_highgui boost_program_options ModuleBase ${YAML_CPP_LIBRARY}) # console

# find Qt library for building the GUI
find_package(Qt5Widgets REQUIRED)


#create gui-executable
add_executable(	uipf-gui
	main-gui.cpp
	gui/ProcessingStepParams.cpp
	gui/ProcessingStepInputs.cpp
	gui/MainWindow.cpp

	framework/ModuleManager.cpp

	framework/Configuration.cpp
	framework/DataManager.cpp
	framework/Context.cpp
	framework/Logger.cpp
	framework/MetaData.cpp
    framework/Utils.cpp

	framework/types/Matrix.cpp
	framework/types/String.cpp
	framework/types/Float.cpp
	framework/types/Integer.cpp

)



target_link_libraries (uipf-gui mainwindow opencv_core opencv_imgproc opencv_highgui boost_program_options ModuleBase ${YAML_CPP_LIBRARY})

# add qt modules to the gui
qt5_use_modules(uipf-gui Core Gui Widgets)
#qt for dynamic modules
qt5_use_modules(uipf Core Gui Widgets)

#manually generate ui_headers
QT5_WRAP_UI(UI_HS
    gui/mainwindow.ui
   )

# window library for the gui
add_library(mainwindow gui/MainWindow.cpp ${UI_HS})
qt5_use_modules(mainwindow Widgets)

add_library(ModuleBase SHARED framework/ModuleBase.cpp )
qt5_use_modules(ModuleBase Core )

#Dynamic Modules
add_library(DummyModule SHARED modules/DummyModule.cpp)
qt5_use_modules(DummyModule Core ) #QtCore is needed for <QPlugin>
add_library(GaussianModule SHARED modules/GaussianModule.cpp)
qt5_use_modules(GaussianModule Core ) #QtCore is needed for <QPlugin>
add_library(LoadImageModule SHARED modules/LoadImageModule.cpp)
qt5_use_modules(LoadImageModule Core ) #QtCore is needed for <QPlugin>
add_library(StoreImageModule SHARED modules/StoreImageModule.cpp)
qt5_use_modules(StoreImageModule Core ) #QtCore is needed for <QPlugin>
add_library(ShowImageModule SHARED modules/ShowImageModule.cpp)
qt5_use_modules(ShowImageModule Core ) #QtCore is needed for <QPlugin>

#link dependencies of dynamic modules
target_link_libraries(LoadImageModule opencv_core opencv_imgproc opencv_highgui ModuleBase)
target_link_libraries(ShowImageModule opencv_core opencv_imgproc opencv_highgui ModuleBase)
target_link_libraries(GaussianModule opencv_core opencv_imgproc opencv_highgui ModuleBase)
target_link_libraries(StoreImageModule opencv_core opencv_imgproc opencv_highgui ModuleBase)

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




