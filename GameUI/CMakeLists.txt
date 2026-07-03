cmake_minimum_required(VERSION 3.14)
project(GameUI LANGUAGES CXX)

find_package(Qt6 COMPONENTS Widgets REQUIRED)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

add_executable(GameUI main.cpp
    mainwindow.h mainwindow.cpp)
target_link_libraries(GameUI PRIVATE Qt6::Widgets GameCore)