cmake_minimum_required(VERSION 3.14)
project(GameUI LANGUAGES CXX)

find_package(Qt6 COMPONENTS Widgets Network REQUIRED)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

add_executable(GameUI
    main.cpp
    mainwindow.h mainwindow.cpp mainwindow.ui
    mainmenu.h mainmenu.cpp mainmenu.ui
    lobby.h lobby.cpp lobby.ui
    settings.h settings.cpp settings.ui
    game.h game.cpp game.ui
)

target_link_libraries(GameUI PRIVATE Qt6::Widgets Qt6::Network GameCore)