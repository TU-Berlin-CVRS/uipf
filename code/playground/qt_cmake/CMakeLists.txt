cmake_minimum_required(VERSION 2.8.8)
project(UIPFramework)

find_package(Qt5Widgets)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_library(mainwindow mainwindow.cpp)
qt5_use_modules(mainwindow Widgets)

add_executable(UIPFramework main.cpp)
target_link_libraries (UIPFramework mainwindow)
qt5_use_modules(UIPFramework Core Gui Widgets)
